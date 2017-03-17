

#ifndef SRC_AUTONUNTIL_H_
#define SRC_AUTONUNTIL_H_
#include <vector>
#include <math.h>


class AutonUntil {
public:
	bool finished = false;
	virtual void start() = 0;
	virtual bool periodic() = 0;
	inline AutonUntil() { finished = true; };
	virtual ~AutonUntil(){};
};

class DriveUntil : public AutonUntil {
public:
	double x = 0, y = 0, z = 0;
	std::vector<double> untilOne;
	std::vector<double> startingVals;
	bool finished = false;
	
	DriveUntil(double x, double y, double z, std::vector<double> untilOne);
	
	
	virtual void start() override;
	// returns false when finished
	virtual bool periodic() override;
	virtual bool periodicWithCorrection(int correctionVector[4]);
	bool shouldStop();
};

class WaitUntil : public AutonUntil {
	double seconds = 0;
	inline WaitUntil(double seconds) { this->seconds = 0; }
	virtual bool periodic() override;
	virtual void start() override;
};


const double WHEEL_RADIUS = 3;
// set this experimentally
const double DEG_TO_INCH_MULTIPLIER = 1;
const double INCH_TO_REV_MULTIPLIER = 1.0/(WHEEL_RADIUS*M_PI);

namespace AutonCommands {
const double AUTON_SPEED = 0.5;

DriveUntil* turnTo(double degrees);
DriveUntil* driveStraight(double until);
DriveUntil* strafeUntil(double until);

}

#endif /* SRC_AUTONUNTIL_H_ */
