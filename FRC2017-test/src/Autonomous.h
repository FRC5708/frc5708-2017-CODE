#ifndef SRC_AUTONOMOUS_H_
#define SRC_AUTONOMOUS_H_

#include "DriveUntil.h"

class Autonomous {

	
public:
	void init(std::vector<DriveUntil> instructions);
	void periodic();
	bool finished = false;
	std::vector<DriveUntil> instructions;
	std::vector<DriveUntil>::iterator current;
	
private:
};

#endif
