#include Robot.h

std::shared_ptr<DriveTrain> Robot::drivetrain = std::make_shared<DriveTrain>();

void RobotInit()
{
	CommandBase::init();
	drivetrain = new Drivetrain();
	winch = new Winch();
	chooser = new SendableChooser();
	chooser->AddDefault("Default Auto", new Autonomous());
	//chooser->AddObject("My Auto", new MyAutoCommand());
	SmartDashboard::PutData("Auto Modes", chooser);
}

/**    * This function is called once each time the robot enters Disabled mode.    * You can use it to reset any subsystem information you want to clear when
 * the robot is disabled.    */

void DisabledInit()
{
}

void DisabledPeriodic()
{
	Scheduler::GetInstance()->Run();
}

void AutonomousInit()
{
	/* std::string autoSelected = SmartDashboard::GetString("Auto Selector", "Default");
	if(autoSelected == "My Auto") {
		autonomousCommand.reset(new MyAutoCommand());
	} else {
		autonomousCommand.reset(new ExampleCommand());
	} */
	autonomousCommand.reset((Command *)chooser->GetSelected());
	if (autonomousCommand != NULL)
		autonomousCommand->Start();
}

void AutonomousPeriodic()
{
	Scheduler::GetInstance()->Run();
}
void TeleopInit()
{
	// This makes sure that the autonomous stops running when
	// teleop starts running. If you want the autonomous to
	// continue until interrupted by another command, remove
	// this line or comment it out.
	if (autonomousCommand != NULL)
		autonomousCommand->Cancel();
}
void TeleopPeriodic()
{
	drivetrain->DriveWithStick();
	winch->DriveWithJoystick();
	Scheduler::GetInstance()->Run();
}

void TestPeriodic()
{
	LiveWindow::GetInstance()->Run();
}

START_ROBOT_CLASS(Robot)
