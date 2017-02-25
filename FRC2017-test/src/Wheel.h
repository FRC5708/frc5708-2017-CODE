/*
 * Wheel.h
 *
 *  Created on: Feb 21, 2017
 *      Author: benw
 */

#ifndef SRC_WHEEL_H_
#define SRC_WHEEL_H_

#include "WPILib.h"

class Wheel {
public:
	Wheel(int pin, std::vector<int> encoderPins);
	double GetDistanceTravelled();
	void ResetDistanceTravelled();
	void SetTargetSpeed(double speed);
	void UpdateSpeed();
	void PowerOut(double power);
	void PrintSpeed(llvm::StringRef name);
private:
	Encoder* encoder;
	SpeedController* motor;
	double distanceOffset = 0;
	const double CIRCUMFERENCE = 6*3.14;
	const double CORRECTION_CONSTANT = .01;
	bool enabled = true;
	double targetSpeed = 0;
	double power = 0;

	double GetSpeed();
	double GetCorrection();

};

#endif /* SRC_WHEEL_H_ */
