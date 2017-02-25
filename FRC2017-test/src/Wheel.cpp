/*
 * Wheel.cpp
 *
 *  Created on: Feb 21, 2017
 *      Author: benw
 */

#include "Wheel.h"
#include "Subsystems/Drivetrain.h"

double min_speed = 0.1*TOP_SPEED;

//Currently will set voltage when target speed is set, then adjust by small amounts.
Wheel::Wheel(int pin, std::vector<int> encoderPins) {
	motor = new Victor(pin);
	motorNum = pin;
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

double PROPORTIONAL_TOLERANCE = 0.1*TOP_SPEED;

double Wheel::GetCorrection(){
	double correction = 0;
	double speed = GetSpeed();
	
	double error = targetSpeed - speed;
	
	/*if (abs(error) > PROPORTIONAL_TOLERANCE) {
		correction = (error > 0) ? 1 : -1;
	}
	else {
		correction = error / PROPORTIONAL_TOLERANCE;
	}*/

	if (abs(targetSpeed) < min_speed) correction = 0;
	else if (abs(error) < PROPORTIONAL_TOLERANCE) {
		correction = 0;
	}
	else {
		correction = error;
	}

	//error = targetSpeed - GetSpeed();
	//return correction * (power * CORRECTION_CONSTANT);

	char* string = (char*)alloca(40);
	sprintf(string, "wheel correction %d", motorNum);
	SmartDashboard::PutNumber(string,  correction * CORRECTION_CONSTANT / TOP_SPEED);
	return correction * CORRECTION_CONSTANT / TOP_SPEED;
}

double Wheel::GetDistanceTravelled(){
	return encoder->Get()-distanceOffset;
}

void Wheel::ResetDistanceTravelled(){
	distanceOffset = GetDistanceTravelled();
}


void Wheel::SetTargetSpeed(double speed){

	if (abs(speed) < min_speed) power = 0;
	else if (targetSpeed != 0 && power != 0) power *= (speed / targetSpeed);
	else power = speed / TOP_SPEED;
	targetSpeed = speed;
}

void Wheel::UpdateSpeed(){
	double correction = GetCorrection();
	power += correction;
	if (power < -1) power = -1;
	if (power > 1) power = 1;

	char* string = (char*)alloca(40);
	sprintf(string, "wheel target %d", motorNum);
	SmartDashboard::PutNumber(string, targetSpeed);
	PowerOut(power);
	sprintf(string, "wheel encoder %d", motorNum);
	SmartDashboard::PutNumber(string, GetSpeed());
	sprintf(string, "wheel power %d", motorNum);
	SmartDashboard::PutNumber(string, power);

}

