#ifndef Drivetrain_H
#define Drivetrain_H

#include "Commands/Subsystem.h"
#include "WPILib.h"

class Drivetrain: public Subsystem
{
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	SpeedController *frontLeftMotor;
	SpeedController *rearLeftMotor;
	SpeedController *frontRightMotor;
	SpeedController *rearRightMotor;

	double encoderOffset = 0;
	double wheelCircumference = 6*3.1415;


	Joystick *mainDriveStick;

	Encoder *frontLeftEncoder;
	Encoder *rearLeftEncoder;
	Encoder *frontRightEncoder;
	Encoder *rearRightEncoder;

public:
	Drivetrain();
	void InitDefaultCommand();
	void Drive(float x,float y,float z);
	double GetEncoderSpeed(Encoder *enc);
	double GetEncoderCount();
	double GetEncoderDistance();
	void InitEncoders();
	void DriveWithStick(int facing);
};

#endif
