#ifndef DriveDist_H
#define DriveDist_H

#include <Commands/CommandGroup.h>

/**
 * The main DriveDist command to pickup and deliver the soda to the box.
 */
class DriveDist: public CommandBase {
public:
	DriveDist(double distance);
	double Distance;
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // DriveDist_H
