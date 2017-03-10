#include "Wheel.h"
#include "Subsystems/Drivetrain.h"

const double REVOLUTIONS_PER_PULSE = 1.0/20.0;
const double GEAR_RATIO = 1.0/13.5;

//Currently will set voltage when target speed is set, then adjust by small amounts.
Wheel::Wheel(int pin, std::vector<int> encoderPins) {
	motor = new Victor(pin);
	encoder = new Encoder(encoderPins[0], encoderPins[1]);
	encoder->SetDistancePerPulse(REVOLUTIONS_PER_PULSE*GEAR_RATIO);
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

double PROPORTIONAL_TOLERANCE = 0.5;

double Wheel::GetCorrection(){
	double correction = 0;
	double speed = GetSpeed();
	
	// actually the - of the error.
	double error = speed - targetSpeed;
	
	if (abs(error) > PROPORTIONAL_TOLERANCE) {
		correction = (error > 0) ? 1 : -1;
	}
	else {
		correction = error / PROPORTIONAL_TOLERANCE;
	}
	//error = targetSpeed - GetSpeed();
	//return error * CORRECTION_CONSTANT;
	
	return correction * (power * CORRECTION_CONSTANT);
}

double Wheel::GetDistanceTravelled(){
	return encoder->GetDistance();
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

