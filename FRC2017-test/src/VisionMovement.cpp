#include "VisionMovement.hpp"



const double STRAFE_TOLERANCE = 0.5;
const double STRAFE_CORRECTION_TOLERANCE = 0.1;

const double CORRECTION_MULTIPLIER = 0.2;

VisionMovement::VisionMovement(NetworkTable* table, Drivetrain* drivetrain) {
	this->table = table; this->drivetrain = drivetrain;
}

double VisionMovement::getValue(double visionOutput, double tolerance) {
	if (visionOutput > tolerance) return 1;
	else if (visionOutput < -tolerance) return -1;
	else return 0;
}

double VisionMovement::getStickValue(double value, double tolerance, double correctionTolerance, VisionState relevantState, VisionState nextState) {
	double toReturn;
	if (state == relevantState) {
		toReturn = getValue(value, tolerance);
		if (toReturn == 0) state = nextState;
	}
	else toReturn = getValue(value, correctionTolerance)*CORRECTION_MULTIPLIER;
	
	return toReturn;
}

bool VisionMovement::periodic() {
	//TODO: timestamps.
	if (state == VisionState::Done) return true;
	
	double z = getStickValue(table->GetNumber("viewAngle", 0), 0.1 * 2*M_PI, 0.02 * 2*M_PI, VisionState::Turning, VisionState::Strafing);
	double x = getStickValue(table->GetNumber("xDist", 0), 0.5, 0.1, VisionState::Strafing, VisionState::Forward);
	double y = getStickValue(table->GetNumber("yDist", 0), 4, 0, VisionState::Forward, VisionState::Done);
	
	
	drivetrain->Drive(x, y, z);
	return false;
}
