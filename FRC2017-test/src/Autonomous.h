#ifndef SRC_AUTONOMOUS_H_
#define SRC_AUTONOMOUS_H_

#include <AutonUntil.h>

class Autonomous {

	
public:
	void init(std::vector<AutonUntil*> instructions);
	void stop();
	void periodic();
	bool finished = false;
	std::vector<AutonUntil*> instructions;
	std::vector<AutonUntil*>::iterator current;
	
private:
};

#endif
