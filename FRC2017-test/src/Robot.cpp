#include <memory>
#include <vector>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

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
#include "Subsystems/Drivetrain.h"
#include "WPILib.h"
#include "CommandBase.h"
#include "Subsystems/Winch.h"
#include "Globals.h"
#include "Commands/DriveStraight.h"
#include "Commands/Rotate.h"
#include "VisionMovement.hpp"
#include "Autonomous.h"



// for access from static functions. ENSURE THREAD SAFETY!
class Robot;
Robot* theRobot;

class Robot: public frc::IterativeRobot {
public:
	
	Joystick* stick;
	Drivetrain *drivetrain;
	Winch *winch;
	AnalogGyro* gyro;

	bool driveInverted = false;
	
	std::shared_ptr<NetworkTable> table;

	std::vector<double> distances;
	Autonomous autonomous;
	DoVision* teleopVision = NULL;


	void RobotInit() override {
		theRobot = this;
		
		drivetrain = new Drivetrain();
		gyro = new AnalogGyro(1);
		theDrivetrain = drivetrain;
		theGyro = gyro;
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
		
		// basic
		//autonomous.init({AutonCommands::driveStraight(12*12)});
		
		// in center
		autonomous.init({
			AutonCommands::driveStraight(47),
			new DoVision(table, 15000)
		});
	}
	
	void AutonomousPeriodic() override {
		//if(AutonState != nullptr) AutonState = AutonState->update();
		frc::Scheduler::GetInstance()->Run();
		//if (vision) vision->periodic();
		autonomous.periodic();
		//if (!hack.periodic()) drivetrain->Drive(0, 0, 0);
	}
	
	void TeleopInit() override {
		autonomous.stop();
	}

	void activateVision() {
		if (teleopVision) delete teleopVision;
		teleopVision = new DoVision(table, 1500);
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
		if (!(teleopVision && teleopVision->periodic())) {
			
			driveInverted = usingFrontCamera;

			if (driveInverted){
				drivetrain->DriveWithStick(-1);
			} else {
				drivetrain->DriveWithStick(1);
			}
		}

		winch->DriveWithJoystick();
		Scheduler::GetInstance()->Run();

		frc::Scheduler::GetInstance()->Run();

		if (wasButtonJustPressed(3)) toggleCamera();
		//if (wasButtonJustPressed(6)) activateVision();

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
