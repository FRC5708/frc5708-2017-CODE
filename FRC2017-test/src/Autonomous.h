#ifndef SRC_AUTONOMOUS_H_
#define SRC_AUTONOMOUS_H_

#include "DriveUntil.h"

class Autonomous {
	typedef enum {
		Strafe,
		Forward
	} State;
	
public:
	void init();
	void periodic();
	bool finished = false;
	
private:
	DriveUntil current;
	State state;
};

#endif
