/*
 * Wheel.cpp
 *
 *  Created on: Feb 21, 2017
 *      Author: benw
 */

#include "Wheel.h"
#include "Subsystems/Drivetrain.h"


//Currently will set voltage when target speed is set, then adjust by small amounts.
Wheel::Wheel(int pin, std::vector<int> encoderPins) {
	motor = new Victor(pin);
	encoder = new Encoder(encoderPins[0], encoderPins[1]);
}

double Wheel::GetSpeed(){
	return encoder->GetRate();
}

void Wheel::PowerOut(double power){
	motor->Set(power);
}

void Wheel::PrintSpeed(llvm::StringRef name){
	SmartDashboard::PutNumber(name, GetSpeed());
}

double Wheel::GetCorrection(){
	double error = 0;
	if (GetSpeed()-targetSpeed != 0){
		if (GetSpeed()-targetSpeed > 0){
			error = -1;
		}else {
			error = 1;
		}
	}

	//error = targetSpeed - GetSpeed();
	//return error * CORRECTION_CONSTANT;
	
	return error * (power * CORRECTION_CONSTANT);
}

double Wheel::GetDistanceTravelled(){
	return encoder->Get()-distanceOffset;
}

void Wheel::ResetDistanceTravelled(){
	distanceOffset = GetDistanceTravelled();
}
void Wheel::SetTargetSpeed(double speed){

	if (targetSpeed != 0) power *= (speed / targetSpeed);
	else power = speed / Drivetrain::TOP_SPEED;
	targetSpeed = speed;
}

void Wheel::UpdateSpeed(){
	double correction = GetCorrection();
	power += correction;
	PowerOut(power);
}

