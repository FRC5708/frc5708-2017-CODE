#include "Autonomous.h"

#include <iostream>

#include "DriveStraight.h"
#include "Rotate.h"

Autonomous::Autonomous() {
	AddSequential(new DriveStraight(1000));
	AddSequential(new Rotate(10));
}
