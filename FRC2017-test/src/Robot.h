#ifndef ROBOT_H_
#define ROBOT_H_

#include <memory>

#include "WPILib.h"
#include "CommandBase.h"
#include "Subsystems/Drivetrain.h"
#include "Commands/Autonomous.h"
#include "Subsystems/Winch.h"

class Robot: public frc::IterativeRobot {
public:
	static std::shared_ptr<DriveTrain> drivetrain;

private:
	std::unique_ptr<Command> autonomousCommand;
	std::shared_ptr<Drivetrain> drivetrain;
	//Drivetrain *drivetrain;
	Winch *winch;
	SendableChooser *chooser;
	frc::LiveWindow* lw = frc::LiveWindow::GetInstance();

	void RobotInit() override;
	void AutonomousInit() override;
	void AutonomousPeriodic() override;
	void TeleopInit() override;
	void TeleopPeriodic() override;
	void TestPeriodic() override;
};

#endif  // ROBOT_H_
