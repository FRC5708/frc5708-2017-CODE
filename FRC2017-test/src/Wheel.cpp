/*
 * Wheel.cpp
 *
 *  Created on: Feb 21, 2017
 *      Author: benw
 */

#include <Wheel.h>

Wheel::Wheel(int pin, std::vector<int> encoderPins) {
	motor = new Victor(pin);
	encoder = new Encoder(encoderPins[0], encoderPins[1]);
}

double Wheel::GetSpeed(){
	return 2/(encoder->GetPeriod()/9);
}

void Wheel::PowerOut(double power){
	motor->Set(power);
}

double Wheel::GetCorrection(){
	double error = targetSpeed - GetSpeed();
	return error * CORRECTION_CONSTANT;
}

double Wheel::GetDistanceTravelled(){
	return encoder->Get()-distanceOffset;
}

void Wheel::ResetDistanceTravelled(){
	distanceOffset = GetDistanceTravelled();
}
void Wheel::SetTargetSpeed(double speed){
	targetSpeed = speed;
}

void Wheel::UpdateSpeed(){
	double correction = GetCorrection();
	power += correction;
	PowerOut(power);
}

