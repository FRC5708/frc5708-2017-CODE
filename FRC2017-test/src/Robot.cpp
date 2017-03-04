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
#include "TCPCLIENT.h"
#include "BasicState.h"
#include "Commands/DriveStraight.h"
#include "Commands/Rotate.h"



// for access from static functions. ENSURE THREAD SAFETY!
class Robot;
Robot* theRobot;

class Robot: public frc::IterativeRobot {
public:
	
	Joystick* stick;
	Drivetrain *drivetrain;
	Winch *winch;

	bool driveInverted = false;

	BasicState* driveStraight;
	BasicState* rotate;
	BasicState* initState;
	BasicState* AutonState;
	
	TcpClient* client;
	std::vector<double> distances;


	void RobotInit() override {
		theRobot = this;
		
		drivetrain = new Drivetrain();
		theDrivetrain = drivetrain;
		winch = new Winch();

		driveStraight = new BasicState(new DriveStraight(100), nullptr);
		rotate = new BasicState(new Rotate(100), driveStraight);
		initState = driveStraight;
		client = new TcpClient();
		distances = std::vector<double>(2);

		
		stick = new frc::Joystick(0);
		
		
		cs::UsbCamera cam0 = CameraServer::GetInstance()->StartAutomaticCapture("front", 0);
		cam0.SetResolution(320, 240);
		cs::UsbCamera cam1 = CameraServer::GetInstance()->StartAutomaticCapture("back", 1);
		cam1.SetResolution(320, 240);
		
		
		std::thread cameraThreadObj(cameraThread);
		cameraThreadObj.detach();
		
		std::thread INetThreadObj(INetThread);
		INetThreadObj.detach();

		printf("initialized robot");
	}
	

	void DisabledInit() override {
		
	}
	
	void DisabledPeriodic() override {
		frc::Scheduler::GetInstance()->Run();
	}
	

	void AutonomousInit() override {
		AutonState = initState;
	}
	
	void AutonomousPeriodic() override {
		if(AutonState != nullptr) AutonState = AutonState->update();
		frc::Scheduler::GetInstance()->Run();
	}
	
	void TeleopInit() override {
		if (AutonState != nullptr) {
			AutonState->StopState();
		}
	}
	
	static void INetThread(){
		TcpClient* _client = theRobot->client;
		_client->conn("127.0.0.1",1337);
		string data;
		int pos;
		while (true){
			data = _client->receive(16);
			pos = data.find(" ");
			double width = std::stod(data.substr(0, pos));
			double height = std::stod(data.substr(pos+1));

			theRobot->distances = {width, height};
		}
	}

	void SendDistanceRequest(){
		client->sendData("distances please!");
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
