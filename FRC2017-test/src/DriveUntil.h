

#ifndef SRC_DRIVEUNTIL_H_
#define SRC_DRIVEUNTIL_H_
#include <vector>
#include <math.h>



class DriveUntil {
public:
	double x = 0, y = 0, z = 0, waitTime = 0;
	std::vector<double> untilAll;
	std::vector<double> startingVals;
	bool finished = false;
	
	DriveUntil(double x, double y, double z, std::vector<double> untilAll);
	inline DriveUntil() {finished = true; };
	inline DriveUntil(double waitTime) { this->waitTime = waitTime; finished = true; };
	
	void start();
	// returns false when finished
	bool periodic();
	bool shouldStop();
};

const double WHEEL_DIAMETER = 6;
const double DEG_TO_INCH_MULTIPLIER = 1;
const double INCH_TO_REV_MULTIPLIER = 1.0/(WHEEL_DIAMETER*M_PI);

namespace AutonCommands {
const double AUTON_SPEED = 0.5;

DriveUntil turnTo(double degrees);
inline DriveUntil driveStraight(double until) {
	return DriveUntil(0, (until > 0)? AUTON_SPEED : -AUTON_SPEED, 0, std::vector<double>(4, until*INCH_TO_REV_MULTIPLIER));
}
inline DriveUntil strafeUntil(double until) {
	double revUntil = until*INCH_TO_REV_MULTIPLIER;
	return DriveUntil((until > 0)? AUTON_SPEED : -AUTON_SPEED, 0, 0, {revUntil, -revUntil, -revUntil, revUntil});
}
inline DriveUntil waitUntil(double until) {
	return DriveUntil(until);
}

}

#endif /* SRC_DRIVEUNTIL_H_ */
