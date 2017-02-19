#include "Rotate.h"

#include <iostream>

#include "../Subsystems/Drivetrain.h"
#include "Robot.h"
#include "Rotate.h"

Rotate::Rotate(double time)
{
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(chassis);
	//Requires(Robot::drivetrain);
	Time = time;
}

// Called just before this Command runs the first time
void Rotate::Initialize()
{
	Robot::drivetrain->InitEncoders();
}

// Called repeatedly when this Command is scheduled to run
void Rotate::Execute()
{
	Robot::drivetrain->Drive(0,0,.2);
	Time -= 1;
}

// Make this return true when this Command no longer needs to run execute()
bool Rotate::IsFinished()
{
	return (Time <= 0);
}

// Called once after isFinished returns true
void Rotate::End()
{
	Robot::drivetrain->Drive(0,0,0);
	Time = 0;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Rotate::Interrupted()
{
}
