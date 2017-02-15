#ifndef DriveDist_H
#define DriveDist_H

#include <Commands/CommandGroup.h>

/**
 * The main DriveDist command to pickup and deliver the soda to the box.
 */
class DriveDist: public CommandBase {
public:
	DriveDist();
	double Distance;
	void Initialize() override;
	bool IsFinished() override;
	void End() override;
};

#endif  // DriveDist_H
