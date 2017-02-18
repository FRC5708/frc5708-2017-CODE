#include "DriveDist.h"

#include <iostream>

#include "../Subsystems/Drivetrain.h"
#include "Robot.h"
#include "DriveDist.h"

DriveDist::DriveDist(double distance)
{
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(chassis);
	Distance = distance;
}

// Called just before this Command runs the first time
void DriveDist::Initialize()
{
	Robot::drivetrain->InitEncoders();
}

// Called repeatedly when this Command is scheduled to run
void DriveDist::Execute()
{
	Robot::drivetrain->Drive(0,.2,0);
}

// Make this return true when this Command no longer needs to run execute()
bool DriveDist::IsFinished()
{
	return (Robot::drivetrain->GetEncoderDistance());
}

// Called once after isFinished returns true
void DriveDist::End()
{
	Robot::drivetrain->Drive(0,0,0);
	Distance = 0;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveDist::Interrupted()
{
}

