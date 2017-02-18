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
	static std::shared_ptr<Drivetrain> drivetrain;
	static std::shared_ptr<Winch> winch;

private:
	Autonomous *autonomousCommand;
	//SendableChooser *chooser;
	frc::LiveWindow* lw = frc::LiveWindow::GetInstance();

	void RobotInit() override;
	void AutonomousInit() override;
	void AutonomousPeriodic() override;
	void TeleopInit() override;
	void TeleopPeriodic() override;
	void TestPeriodic() override;
};

#endif  // ROBOT_H_
