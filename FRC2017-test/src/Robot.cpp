#include <BasicState.h>
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
#include "BasicState.h"
#include "Commands/DriveStraight.h"
#include "Commands/Rotate.h"
#include "VisionMovement.hpp"
#include "Autonomous.h"
#include <ADXRS450_Gyro.h>



// for access from static functions. ENSURE THREAD SAFETY!
class Robot;
Robot* theRobot;

class Robot: public frc::IterativeRobot {
public:
	
	Joystick* stick;
	Drivetrain *drivetrain;
	Winch *winch;

	bool driveInverted = false;
	
	std::shared_ptr<NetworkTable> table;

	std::vector<double> distances;
	VisionMovement* vision = NULL;
	Autonomous autonomous;
	RobotDrive* drive;
	//DriveUntil hack;


	void RobotInit() override {
		theRobot = this;
		
		drivetrain = new Drivetrain();
		theDrivetrain = drivetrain;
		winch = new Winch();
		//table = NetworkTable::GetTable("Vision");
		distances = std::vector<double>(2);
		drive = new RobotDrive(9,7,8,5);

		
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
		//AutonState = initState;
		
		//change this if needed
		bool inCenter = false;

		if (inCenter) {
			autonomous.init({ 
						AutonCommands::driveStraight(111), // to peg
						AutonCommands::waitUntil(5), 		 // for lift
						AutonCommands::driveStraight(-4*12), // out of lift
						AutonCommands::strafeUntil(4*12), 
						AutonCommands::driveStraight(5*12)   // to baseline
			});
		}
		else autonomous.init({AutonCommands::driveStraight(11*12)});
	}
	
	void AutonomousPeriodic() override {
		//if(AutonState != nullptr) AutonState = AutonState->update();
		frc::Scheduler::GetInstance()->Run();
		//if (vision) vision->periodic();
		autonomous.periodic();
		//if (!hack.periodic()) drivetrain->Drive(0, 0, 0);
	}
	
	void TeleopInit() override {
	}

	void activateVision() {
		// VisionMovement::periodic() is always called.
		if (vision) delete vision;
		//vision = new VisionMovement(&*table, drivetrain);
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
		if (!(vision && vision->periodic())) {
			
			driveInverted = usingFrontCamera;

			/*Day337:
			I'm beginnig to feel the effects of dehydration on my phsyci.
			My vision is becomming blurred and its getting more and more difficult to think clearly.
			I dontnow how much longer I can last.

			Day338;
			I saw my dead mom today,the hallucination are getting more vivid and more frequent.
			Im worried soon I wont be able to discern them from reality. Then Ill have really lost my mind

			Day339:
			I should be dead by now. I mean, almost 340 days without water isnt natural.
			I dont think I can take this much longer, and im worried that when I finally decide to end this,
			I wont even be able to die by my own hand.*/


			if (driveInverted){
				drivetrain->DriveWithStick(-1);
			} else {
				drivetrain->DriveWithStick(1);
			}
		}

		//winch->DriveWithJoystick();
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
