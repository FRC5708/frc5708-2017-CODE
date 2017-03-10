/*
 * Wheel.h
 *
 *  Created on: Feb 21, 2017
 *      Author: benw
 */

#ifndef SRC_WHEEL_H_
#define SRC_WHEEL_H_

#include "WPILib.h"

// Encoder values given in full revolutions of encoder
class Wheel {
public:
	Wheel(int pin, std::vector<int> encoderPins);
	int GetDistanceTravelled();
	void ResetDistanceTravelled();
	void SetTargetSpeed(double speed);
	void UpdateSpeed();
	void PowerOut(double power);
	void PrintSpeed(llvm::StringRef name);
//private:
	Encoder* encoder;
	SpeedController* motor;
	int distanceOffset = 0;
	const double CIRCUMFERENCE = 6*3.14;
	const double CORRECTION_CONSTANT = .01;
	bool enabled = true;
	double targetSpeed = 0;
	double power = 0;

	double GetSpeed();
	double GetCorrection();

};

#endif /* SRC_WHEEL_H_ */
