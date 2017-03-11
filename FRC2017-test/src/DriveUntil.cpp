#include <DriveUntil.h>
#include "Subsystems/Drivetrain.h"
#include "Globals.h"


DriveUntil::DriveUntil(double x, double y, double z, std::vector<double> one) {
	this->x = x; this->y = y; this->z = z; this->untilOne = one;
}

void DriveUntil::start() {
	
	// hack, not recommended
	if (waitTime != 0) frc::Wait(waitTime);
	else startingVals = theDrivetrain->getDistances();
}

bool DriveUntil::periodic() {
	if (!finished) {
		if (!shouldStop()) theDrivetrain->Drive(x, -y, z);
		else finished = true;
		return true;
	}
	else return false;
}

bool DriveUntil::shouldStop() {
	std::vector<double> encoderVals = theDrivetrain->getDistances();
	SmartDashboard::PutNumber("encoder 1", encoderVals[0]);
	SmartDashboard::PutNumber("encoder 2", encoderVals[1]);
	SmartDashboard::PutNumber("encoder 3", encoderVals[2]);
	SmartDashboard::PutNumber("encoder 4", encoderVals[3]);
	bool toReturn = false;
	for (int i = 0; i != 4; ++i) {
		double until = untilOne[i];
		if (encoderVals[i] != 0 && abs(encoderVals[i] - startingVals[i]) > abs(until)) {
			toReturn = true;
			break;
		}
	}
	return toReturn;
}


DriveUntil turnTo(double degrees) {
	double turnAmount = degrees * DEG_TO_INCH_MULTIPLIER * INCH_TO_REV_MULTIPLIER;
	return DriveUntil(0, 0, degrees > 0 ? 1 : -1, std::vector<double>(4, turnAmount));
}

