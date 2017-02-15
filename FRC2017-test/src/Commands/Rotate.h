#ifndef Rotate_H
#define Rotate_H

#include <Commands/CommandGroup.h>

/**
 * The main Rotate command to pickup and deliver the soda to the box.
 */
class Rotate: public CommandBase {
public:
	Rotate();
	double Time;
	void Initialize() override;
	bool IsFinished() override;
	void End() override;
};

#endif  // Rotate_H
