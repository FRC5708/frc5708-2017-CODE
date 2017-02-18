#ifndef Rotate_H
#define Rotate_H

#include "CommandBase.h"

/**
 * The main Rotate command to pickup and deliver the soda to the box.
 */
class Rotate: public CommandBase {
public:
	Rotate(double time);
	double Time;
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // Rotate_H
