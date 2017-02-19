#ifndef ROBOT_H_
#define ROBOT_H_

#include <memory>

#include "WPILib.h"
#include "CommandBase.h"
#include "Subsystems/Drivetrain.h"
#include "Commands/Autonomous.h"
#include "Subsystems/Winch.h"
#include "Subsystems/DriveTrain.h"

class Robot: public frc::IterativeRobot {
public:
	static std::shared_ptr<Drivetrain> Robot::drivetrain;
	std::unique_ptr<Command> autonomousCommand;
	Drivetrain *mainDrivetrain;
	Winch *winch;
	//SendableChooser *chooser;
	frc::LiveWindow* lw = frc::LiveWindow::GetInstance();

private:

	void RobotInit() override;
	void AutonomousInit() override;
	void AutonomousPeriodic() override;
	void TeleopInit() override;
	void TeleopPeriodic() override;
	void TestPeriodic() override;
};

#endif  // ROBOT_H_
