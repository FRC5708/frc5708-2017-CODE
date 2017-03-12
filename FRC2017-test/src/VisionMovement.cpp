#include "VisionMovement.hpp"
#include "Globals.h"

const double CORRECTION_MULTIPLIER = 0.2;
const unsigned VISION_ERROR_DONOTHING_TIMEOUT = 200;


DoVision::DoVision(std::shared_ptr<NetworkTable> table, unsigned timeout) {
	this->table = table; this->timeout = timeout;
}

double DoVision::getValue(double visionOutput, double tolerance) {
	if (visionOutput > tolerance) return .5;
	else if (visionOutput < -tolerance) return -.5;
	else return 0;
}

double DoVision::getStickValue(double value, double tolerance, double correctionTolerance, VisionState relevantState, VisionState nextState) {
	double toReturn;
	if (state == relevantState) {
		toReturn = getValue(value, tolerance);
		if (toReturn == 0) state = nextState;
	}
	else toReturn = getValue(value, correctionTolerance)*CORRECTION_MULTIPLIER;
	
	return toReturn;
}

bool DoVision::periodic() {
	//TODO: timestamps.
	if (finished == true) return false;
	if (state == VisionState::Done) {
		finished = true;
		return false;
	}
	if (table->GetBoolean("succeeded"), false) {
		timeoutStart = -1;

		double z = getStickValue(table->GetNumber("viewAngle", 0), 0.1 * 2*M_PI, 0.02 * 2*M_PI, VisionState::Turning, VisionState::Strafing);
		double x = getStickValue(table->GetNumber("xDist", 0), 0.5, 0.1, VisionState::Strafing, VisionState::Forward);
		double y = getStickValue(table->GetNumber("yDist", 0), 4, 0, VisionState::Forward, VisionState::Done);
		//       This value in particular should be tested for ^
		theDrivetrain->Drive(x, y, z);
	}
	else {
		unsigned time = getTime().count();
		if (timeoutStart == -1) {
			timeoutStart = time;
		}
		else if (time - timeoutStart > VISION_ERROR_DONOTHING_TIMEOUT) {
			if (time - timeoutStart > timeout) finished = true;
			else theDrivetrain->Drive(0, 0, 0);
		}
		// else do nothing
	}
	return true;
}

void DoVision::start() {
	
}
