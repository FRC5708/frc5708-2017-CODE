

#ifndef SRC_DRIVEUNTIL_H_
#define SRC_DRIVEUNTIL_H_
#include <vector>
#include <math.h>


class DriveUntil {
public:
	double x, y, z;
	std::vector<double> untilAll;
	std::vector<double> startingVals;
	bool finished = false;
	
	DriveUntil(double x, double y, double z, std::vector<double> untilAll);
	inline DriveUntil() {finished = true; };
	// returns false when finished
	bool periodic();
	bool shouldStop();
};

const double WHEEL_RADIUS = 3;
const double DEG_TO_INCH_MULTIPLIER = 1;
const double INCH_TO_REV_MULTIPLIER = 1/(WHEEL_RADIUS*M_PI);

DriveUntil turnTo(double degrees);
inline DriveUntil driveForward(double until) {
	return DriveUntil(0, 1, 0, std::vector<double>(4, until*INCH_TO_REV_MULTIPLIER));
}
inline DriveUntil strafeUntil(double until) {
	double revUntil = until*INCH_TO_REV_MULTIPLIER;
	return DriveUntil(1, 0, 0, {revUntil, 0, 0, 0});
}


#endif /* SRC_DRIVEUNTIL_H_ */
