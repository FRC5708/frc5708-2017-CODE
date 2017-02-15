#include "Autonomous.h"

#include <iostream>

#include "DriveDist.h"
#include "Rotate.h"

Autonomous::Autonomous() : frc::CommandGroup("Autonomous") {
	AddSequential(new DriveDist(100));
	AddSequential(new Rotate(18274));
}
