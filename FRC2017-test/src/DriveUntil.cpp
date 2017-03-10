#include <DriveUntil.h>
#include "Subsystems/Drivetrain.h"
#include "Globals.h"


DriveUntil::DriveUntil(double x, double y, double z, std::vector<int> all) {
	this->x = x; this->y = y; this->z = z; this->untilAll = all;
}

bool DriveUntil::periodic() {
	if (!finished) {
		if (!shouldStop()) theDrivetrain->Drive(x, y, z);
		else finished = true;
		return true;
	}
	else return false;
}

bool DriveUntil::shouldStop() {
	std::vector<int> encoderVals = theDrivetrain->getDistances();
	bool toReturn = true;
	for (int i = 0; i != 4; ++i) {
		if (encoderVals[i] - startingVals[i] < untilAll[i]) {
			toReturn = false;
			break;
		}
	}
	return toReturn;
}


DriveUntil turnTo(double degrees) {
	double turnAmount = degrees * DEG_TO_INCH_MULTIPLIER * INCH_TO_REV_MULTIPLIER;
	return DriveUntil(0, 0, degrees > 0 ? 1 : -1, std::vector<int>(4, turnAmount));
}

