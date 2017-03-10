#ifndef Drivetrain_H
#define Drivetrain_H

#include <Commands/Subsystem.h>
#include "WPILib.h"
#include "Wheel.h"

class Drivetrain: public Subsystem
{
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	
	Wheel *frontLeftWheel;
	Wheel *rearLeftWheel;
	Wheel *frontRightWheel;
	Wheel *rearRightWheel;

	Joystick *mainDriveStick;
	
	frc::RobotDrive* robotDrive;
public:
	Drivetrain();
	void Drive(float x,float y,float z);
	double GetEncoderDistance();
	void ResetDistances();
	void DriveWithStick(int facing);
	std::vector<double> getDistances();

	constexpr static double TOP_SPEED = 100;
};

#endif
