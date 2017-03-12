#include <AutonUntil.h>
#include "Subsystems/Drivetrain.h"
#include "Globals.h"


DriveUntil::DriveUntil(double x, double y, double z, std::vector<double> one) {
	this->x = x; this->y = y; this->z = z; this->untilOne = one;
}

void DriveUntil::start() {
	startingVals = theDrivetrain->getDistances();
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

void WaitUntil::start() {
	frc::Wait(seconds);
}
bool WaitUntil::periodic() {
	return false;
}

namespace AutonCommands {

DriveUntil* driveStraight(double until) {
	return new DriveUntil(0, (until > 0)? AUTON_SPEED : -AUTON_SPEED, 0, std::vector<double>(4, until*INCH_TO_REV_MULTIPLIER));
}
DriveUntil* turnTo(double degrees) {
	double turnAmount = degrees * DEG_TO_INCH_MULTIPLIER * INCH_TO_REV_MULTIPLIER;
	return new DriveUntil(0, 0, degrees > 0 ? 1 : -1, std::vector<double>(4, turnAmount));
}
DriveUntil* strafeUntil(double until) {
	double revUntil = until*INCH_TO_REV_MULTIPLIER;
	return new DriveUntil((until > 0)? AUTON_SPEED : -AUTON_SPEED, 0, 0, {revUntil, -revUntil, -revUntil, revUntil});
}

}
