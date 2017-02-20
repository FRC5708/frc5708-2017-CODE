#include "Autonomous.h"

#include <iostream>

#include "DriveStraight.h"
#include "Rotate.h"

Autonomous::Autonomous() {
	AddSequential(new DriveStraight(100));
	AddSequential(new Rotate(18274));
}
