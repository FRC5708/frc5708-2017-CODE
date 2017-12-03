#include "Drivetrain.h"
#include "../RobotMap.h"
#include "WPILib.h"

Drivetrain::Drivetrain() : Subsystem("DriveTrain") {

	const static int frontLeftChannel	= 9;
    const static int rearLeftChannel	= 5;
    const static int frontRightChannel	= 8;
    const static int rearRightChannel	= 7;

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

double Drivetrain::GetEncoderCount(){
	return (frontLeftEncoder->Get()/13.521)-encoderOffset;
}

double Drivetrain::GetEncoderDistance(){
	return wheelCircumference*GetEncoderCount();
}

void Drivetrain::InitEncoders(){
	encoderOffset = GetEncoderDistance();
}


void Drivetrain::Drive(float x,float y,float z){
	float FL_speed = x+z+y;
	float FR_speed = y-z-x;
	float RR_speed = y-z+x;
	float RL_speed = y+z-x;
	frontLeftMotor->Set(-FL_speed);
	frontRightMotor->Set(FR_speed);
	rearRightMotor->Set(RR_speed);
	rearLeftMotor->Set(-RL_speed);
}

void Drivetrain::DriveWithStick(int facing){
	float x = mainDriveStick->GetX();
	float y = mainDriveStick->GetY();
	float z = mainDriveStick->GetZ();
	Drive(-x,y*facing,-z);
}
