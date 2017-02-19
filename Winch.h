#ifndef Winch_H
#define Winch_H

#include "Commands/Subsystem.h"
#include "WPILib.h"

class Winch: public Subsystem
{
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	SpeedController *leftMotor;
	SpeedController *rightMotor;

	Joystick *mainDriveStick;

	Encoder *leftEncoder;
	Encoder *rightEncoder;

public:
	Winch();
	void InitDefaultCommand();
	void Drive(double speed);
	double GetEncoderSpeed(Encoder *enc);
	void DriveWithJoystick();
};

#endif
