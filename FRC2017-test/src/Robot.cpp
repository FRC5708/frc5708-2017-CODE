#include <memory>
#include <vector>
#include <thread>

#include <Commands/Command.h>
#include <Commands/Scheduler.h>
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <CameraServer.h>
#include <Joystick.h>

#include "Commands/ExampleCommand.h"

#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>


#include "WPILib.h"
//#include "CommandBase.h"
#include "Subsystems/Drivetrain.h"
//#include "Commands/Autonomous.h"
#include "Subsystems/Winch.h"


// for access from static functions. ENSURE THREAD SAFETY!
class Robot;
Robot* theRobot;

class Robot: public frc::IterativeRobot {
public:
	
	frc::Joystick* stick;
	Drivetrain *mainDrivetrain;
	Winch *winch;
	bool driveInverted = false;
	
	//std::shared_ptr<Drivetrain> Robot::drivetrain = std::make_shared<Drivetrain>();

	void RobotInit() override {
		theRobot = this;
		
		//CommandBase::init();
		mainDrivetrain = new Drivetrain();
		winch = new Winch();
		//chooser = new SendableChooser();
		//chooser->AddDefault("Default Auto", new Autonomous());
		//chooser->AddObject("My Auto", new MyAutoCommand());
		//SmartDashboard::PutData("Auto Modes", chooser);

		
		stick = new frc::Joystick(0);
		
		
		cs::UsbCamera cam0 = CameraServer::GetInstance()->StartAutomaticCapture("front", 0);
		cam0.SetResolution(320, 240);
		cs::UsbCamera cam1 = CameraServer::GetInstance()->StartAutomaticCapture("back", 1);
		cam1.SetResolution(320, 240);
		
		
		std::thread cameraThreadObj(cameraThread);
		cameraThreadObj.detach();
		
		printf("initialized robot");
	}
	
	/**
	 * This function is called once each time the robot enters Disabled mode.
	 * You can use it to reset any subsystem information you want to clear when
	 * the robot is disabled.
	 */
	void DisabledInit() override {
		
	}
	
	void DisabledPeriodic() override {
		frc::Scheduler::GetInstance()->Run();
	}
	
	/**
	 * This autonomous (along with the chooser code above) shows how to select
	 * between different autonomous modes using the dashboard. The sendable
	 * chooser code works with the Java SmartDashboard. If you prefer the
	 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
	 * GetString code to get the auto name from the text box below the Gyro.
	 *
	 * You can add additional auto modes by adding additional commands to the
	 * chooser code above (like the commented example) or additional comparisons
	 * to the if-else structure below with additional strings & commands.
	 */
	void AutonomousInit() override {
		/* std::string autoSelected = frc::SmartDashboard::GetString("Auto Selector", "Default");
		 if (autoSelected == "My Auto") {
			autonomousCommand.reset(new MyAutoCommand());
		 }
		 else {
			autonomousCommand.reset(new ExampleCommand());
		 } */
		
		autonomousCommand.reset(chooser.GetSelected());
		
		if (autonomousCommand.get() != nullptr) {
			autonomousCommand->Start();
		}
	}
	
	void AutonomousPeriodic() override {
		frc::Scheduler::GetInstance()->Run();
	}
	
	void TeleopInit() override {
		// This makes sure that the autonomous stops running when
		// teleop starts running. If you want the autonomous to
		// continue until interrupted by another command, remove
		// this line or comment it out.
		if (autonomousCommand != nullptr) {
			autonomousCommand->Cancel();
		}
	}
	
	
	static void cameraThread() {
		
		cs::CvSource output = CameraServer::GetInstance()->PutVideo("Camera", 320, 240);
		
		cs::CvSink frontSink = CameraServer::GetInstance()->GetVideo("front");
		cs::CvSink backSink = CameraServer::GetInstance()->GetVideo("back");
		
		cv::Mat mat;
		bool oldUsingFront = theRobot->usingFrontCamera;
		while(true) {
			// access once for thread safety
			volatile bool usingFront = theRobot->usingFrontCamera;
			if (usingFront != oldUsingFront) {
				if (usingFront) {
					backSink.SetEnabled(false);
					frontSink.SetEnabled(true);
				}
				else {
					frontSink.SetEnabled(false);
					backSink.SetEnabled(true);
				}
			}
			oldUsingFront = theRobot->usingFrontCamera;
			
			
			if (usingFront) {
				pushFrame(frontSink, output, mat);
			}
			else {
				pushFrame(backSink, output, mat);
			}
		}
	}
	static void pushFrame(cs::CvSink& sink, cs::CvSource& output, cv::Mat& reusableMat) {
		if (sink.GrabFrame(reusableMat) == 0) {
			// Send the output the error.
			output.NotifyError(sink.GetError());
			// skip the rest of the current iteration
			return;
		}
		
		output.PutFrame(reusableMat);
	}
	
	
	
	// system for button handlers.
	// call from TeleopPeriodic(). If true, call function associated with button.
	
	std::vector<bool> buttons = std::vector<bool>(12);
	bool wasButtonJustPressed(int button) {
		bool isPressed = stick->GetRawButton(button);
		
		if (isPressed) {
			if (!buttons[button - 1]) {
				buttons[button - 1] = true;
				return true;
			}
		}
		else buttons[button - 1] = false;
		return false;
	}
	
	void TeleopPeriodic() override {
		driveInverted = usingFrontCamera;
		
		if (!driveInverted){
			mainDrivetrain->DriveWithStick(1);
		}else{
			mainDrivetrain->DriveWithStick(-1);
		}

		winch->DriveWithJoystick();
		Scheduler::GetInstance()->Run();
		
		frc::Scheduler::GetInstance()->Run();
		
		if (wasButtonJustPressed(3)){
			toggleCamera();
			driveInverted = !driveInverted;
		}
	}
	
	void TestPeriodic() override {
		frc::LiveWindow::GetInstance()->Run();
	}
	
	
	
	volatile bool usingFrontCamera = true;
	void toggleCamera() {
		usingFrontCamera = !usingFrontCamera;
		
		printf("toggling cameras");
	}
	
private:
	std::unique_ptr<frc::Command> autonomousCommand;
	frc::SendableChooser<frc::Command*> chooser;
};







void TestPeriodic()
{
	LiveWindow::GetInstance()->Run();
}

START_ROBOT_CLASS(Robot)
