#include "Drivetrain.h"
#include "../RobotMap.h"

Drivetrain::Drivetrain() : Subsystem("DriveTrain") {

	const static int frontLeftChannel	= 0;
    const static int rearLeftChannel	= 1;
    const static int frontRightChannel	= 2;
    const static int rearRightChannel	= 3;

    const static int joystickChannel	= 0;

    mainDriveStick = new Joystick(joystickChannel);

    frontLeftMotor = new Victor(frontLeftChannel);
    rearLeftMotor = new Victor(rearLeftChannel);
    frontRightMotor = new Victor(frontRightChannel);
    rearRightMotor = new Victor(rearRightChannel);

    frontLeftEncoder = new Encoder(0,1);
    rearLeftEncoder = new Encoder(2,3);
    frontRightEncoder = new Encoder(4,5);
    rearRightEncoder = new Encoder(6,7);


}

void Drivetrain::InitDefaultCommand()
{

}

double Drivetrain::GetEncoderSpeed(Encoder* enc){
	return (1/enc->GetPeriod())/9;
}


void Drivetrain::Drive(float x,float y,float z){
	float FL_speed = x+z+y;
	float FR_speed = y-z-x;
	float RR_speed = y-z+x;
	float RL_speed = y+z-x;
	frontLeftMotor->Set(FL_speed);
	frontRightMotor->Set(FR_speed);
	rearRightMotor->Set(RR_speed);
	rearLeftMotor->Set(RL_speed);
}

void Drivetrain::DriveWithStick(){
	float x = mainDriveStick->GetX();
	float y = mainDriveStick->GetY();
	float z = mainDriveStick->GetZ();
	Drive(x,y,z);
}
