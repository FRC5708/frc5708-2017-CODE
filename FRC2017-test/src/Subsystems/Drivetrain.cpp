#include "Drivetrain.h"
#include "../RobotMap.h"
#include "WPIlib.h"

Drivetrain::Drivetrain() : Subsystem("DriveTrain") {

	const static int frontLeftChannel	= 9;
    const static int rearLeftChannel	= 7;
    const static int frontRightChannel	= 8;
    const static int rearRightChannel	= 5;

    const static std::vector<int> frontLeftEnc = {0,1};
    const static std::vector<int> rearLeftEnc = {2,3};
    const static std::vector<int> frontRightEnc = {4,5};
    const static std::vector<int> rearRightEnc = {6,7};

    const static int joystickChannel	= 0;

    mainDriveStick = new Joystick(joystickChannel);

    frontLeftWheel = new Wheel(frontLeftChannel, frontLeftEnc);
    rearLeftWheel = new  Wheel(rearLeftChannel, rearLeftEnc);
    frontRightWheel = new Wheel(frontRightChannel, frontRightEnc);
    rearRightWheel = new Wheel(rearRightChannel, rearRightEnc);
    
    robotDrive = new frc::RobotDrive(frontLeftWheel->motor, rearLeftWheel->motor,frontRightWheel->motor,rearRightWheel->motor);
    robotDrive->SetExpiration(0.1);

    robotDrive->SetInvertedMotor(RobotDrive::MotorType::kFrontRightMotor, true);
    robotDrive->SetInvertedMotor(RobotDrive::MotorType::kRearRightMotor, true);

}

// what? why? who knows.
double Drivetrain::GetEncoderDistance(){
	double avg = (frontLeftWheel->GetDistanceTravelled()
			      + rearLeftWheel->GetDistanceTravelled()
				  + frontRightWheel->GetDistanceTravelled()
				  + rearRightWheel->GetDistanceTravelled());
	avg /= 8;
	return avg;
}

std::vector<double> Drivetrain::getDistances() {
	return { frontLeftWheel->GetDistanceTravelled(),
	      rearLeftWheel->GetDistanceTravelled(),
		  -frontRightWheel->GetDistanceTravelled(),
		  -rearRightWheel->GetDistanceTravelled() };
}

void Drivetrain::ResetDistances(){
	frontLeftWheel->ResetDistanceTravelled();
	rearLeftWheel->ResetDistanceTravelled();
	frontRightWheel->ResetDistanceTravelled();
	rearRightWheel->ResetDistanceTravelled();
}


void Drivetrain::Drive(float x,float y,float z){
	/*float FL_speed = (x+z+y) * TOP_SPEED;
	float FR_speed = (y-z-x) * TOP_SPEED;
	float RR_speed = (y-z+x) * TOP_SPEED;
	float RL_speed = (y+z-x) * TOP_SPEED;

	frontLeftWheel->SetTargetSpeed(-FL_speed);
	frontRightWheel->SetTargetSpeed(FR_speed);
	rearRightWheel->SetTargetSpeed(RR_speed);
	rearLeftWheel->SetTargetSpeed(-RL_speed);

	frontLeftWheel->UpdateSpeed();
	frontRightWheel->UpdateSpeed();
	rearRightWheel->UpdateSpeed();
	rearLeftWheel->UpdateSpeed();

	frontLeftWheel->PrintSpeed("FL");
	frontLeftWheel->PrintSpeed("RL");
	frontLeftWheel->PrintSpeed("FR");
	frontLeftWheel->PrintSpeed("RR");*/
	 robotDrive->SetSafetyEnabled(false);
	robotDrive->MecanumDrive_Cartesian(x, -y, z);

	//SmartDashboard::PutNumber("x", x);

	//frontRightWheel->PowerOut(1);
}

void Drivetrain::DriveWithStick(int facing){
	float x = mainDriveStick->GetX();
	float y = mainDriveStick->GetY();
	float z = mainDriveStick->GetZ();
	float SPEED = 1.2;
	Drive(-x*SPEED,y*SPEED,z*SPEED);
}
