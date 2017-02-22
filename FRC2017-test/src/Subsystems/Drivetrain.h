#ifndef Drivetrain_H
#define Drivetrain_H

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include <PIDOutput.h>
#include <PIDSource.h>
#include <vector>

class Drivetrain: public Subsystem
{
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	SpeedController *frontLeftMotor;
	SpeedController *rearLeftMotor;
	SpeedController *frontRightMotor;
	SpeedController *rearRightMotor;


	//all measurements in inches.
	double encoderOffset = 0;
	double wheelWidth = 14.5;
	double wheelLength = 10;
	double wheelRadius = 3;
	double wheelCircumference = 2*wheelRadius*3.1415;
	double MAX_SPEED = 1;


	Joystick *mainDriveStick;

	Encoder *frontLeftEncoder;
	Encoder *rearLeftEncoder;
	Encoder *frontRightEncoder;
	Encoder *rearRightEncoder;


	PIDController *frontLeftController;
	PIDController *rearLeftController;
	PIDController *frontRightController;
	PIDController *rearRightController;

	std::vector<Encoder*> encoders;
	std::vector<SpeedController*> motors;
	std::vector<PIDController*> controllers;

public:
	Drivetrain();
	void InitDefaultCommand();
	std::vector<double> CalculateOmegas(double x,double y,double z);
	void Drive(double x,double y,double z);
	double GetEncoderSpeed(int index);
	double GetEncoderCount(int index);
	double GetEncoderDistance(int index);
	void InitEncoders();
	void DriveMotor(int index, double speed);
	void DriveWithStick(int facing);
	void InitPids();
};

class frontLeftPIDSource: public PIDSource {
public:
	virtual ~frontLeftPIDSource() = default;
	virtual double PIDGet() override;
};

class frontLeftPIDOutput: public PIDOutput {
public:
	virtual ~frontLeftPIDOutput() = default;
	virtual void PIDWrite(double d) override;
};
class rearLeftPIDSource: public PIDSource {
public:
	virtual ~rearLeftPIDSource() = default;
	virtual double PIDGet() override;
};

class rearLeftPIDOutput: public PIDOutput {
public:
	virtual ~rearLeftPIDOutput() = default;
	virtual void PIDWrite(double d) override;
};
class frontRightPIDSource: public PIDSource {
public:
	virtual ~frontRightPIDSource() = default;
	virtual double PIDGet() override;
};

class frontRightPIDOutput: public PIDOutput {
public:
	virtual ~frontRightPIDOutput() = default;
	virtual void PIDWrite(double d) override;
};
class rearRightPIDSource: public PIDSource {
public:
	virtual ~rearRightPIDSource() = default;
	virtual double PIDGet() override;
};

class rearRightPIDOutput: public PIDOutput {
public:
	virtual ~rearRightPIDOutput() = default;
	virtual void PIDWrite(double d) override;
};

#endif
