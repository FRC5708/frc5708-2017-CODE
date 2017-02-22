#include "Winch.h"
#include "../RobotMap.h"
#include "WPILIB.h"

Winch::Winch() : Subsystem("Winch") {

	const static int leftChannel	= 4;
    const static int rightChannel	= 5;

    const static int joystickChannel	= 0;

    mainDriveStick = new Joystick(joystickChannel);

    leftMotor = new Victor(leftChannel);
    rightMotor = new Victor(rightChannel);

    rightEncoder = new Encoder(8,9);
    leftEncoder = new Encoder(8,9);


}

void Winch::InitDefaultCommand()
{

}

double Winch::GetEncoderSpeed(Encoder* enc){
	return (1/enc->GetPeriod())/9;
}


void Winch::Drive(double speed){
	leftMotor->Set(speed);
	rightMotor->Set(speed);
}

void Winch::DriveWithJoystick(){
	if (mainDriveStick->GetRawButton(1)){
		Drive(.5);
	}else{
		Drive(0);
	}
}
