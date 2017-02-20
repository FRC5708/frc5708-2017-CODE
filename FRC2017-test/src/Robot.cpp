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

#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>


#include "WPILib.h"
#include "CommandBase.h"
#include "Subsystems/Drivetrain.h"
//#include "Commands/Autonomous.h"
#include "Subsystems/Winch.h"
#include "Subsystems/DriveTrain.h"



// for access from static functions. ENSURE THREAD SAFETY!
class Robot;
Robot* theRobot;

class Robot: public frc::IterativeRobot {
public:
	
	Joystick* stick;
	Drivetrain *drivetrain;
	Winch *winch;
	bool driveInverted = false;
	
	//std::shared_ptr<Drivetrain> Robot::drivetrain = std::make_shared<Drivetrain>();

	void RobotInit() override {
		theRobot = this;
		
		//CommandBase::init();
		drivetrain = new Drivetrain();
		winch = new Winch();

		
		stick = new frc::Joystick(0);
		
		
		cs::UsbCamera cam0 = CameraServer::GetInstance()->StartAutomaticCapture("front", 0);
		cam0.SetResolution(320, 240);
		cs::UsbCamera cam1 = CameraServer::GetInstance()->StartAutomaticCapture("back", 1);
		cam1.SetResolution(320, 240);
		
		
		std::thread cameraThreadObj(cameraThread);
		cameraThreadObj.detach();
		
		printf("initialized robot");
	}
	

	void DisabledInit() override {
		
	}
	
	void DisabledPeriodic() override {
		frc::Scheduler::GetInstance()->Run();
	}
	

	void AutonomousInit() override {
		/* std::string autoSelected = frc::SmartDashboard::GetString("Auto Selector", "Default");
		 if (autoSelected == "My Auto") {
			autonomousCommand.reset(new MyAutoCommand());
		 }
		 else {
			autonomousCommand.reset(new ExampleCommand());
		 } */

		
		if (autonomousCommand.get() != nullptr) {
			autonomousCommand->Start();
		}
	}
	
	void AutonomousPeriodic() override {
		frc::Scheduler::GetInstance()->Run();
	}
	
	void TeleopInit() override {
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
			oldUsingFront = usingFront;
			
			
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

		if (driveInverted){
			drivetrain->DriveWithStick(-1);
		}else{
			drivetrain->DriveWithStick(1);
		}

		winch->DriveWithJoystick();
		Scheduler::GetInstance()->Run();
		
		frc::Scheduler::GetInstance()->Run();
		
		if (wasButtonJustPressed(3)) toggleCamera();
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
};



void TestPeriodic()
{
	LiveWindow::GetInstance()->Run();
}

START_ROBOT_CLASS(Robot)
