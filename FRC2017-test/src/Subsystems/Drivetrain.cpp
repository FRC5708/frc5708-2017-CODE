#include <Subsystems/Drivetrain.h>
#include "../RobotMap.h"
#include "WPILib.h"
#include <vector>
#include "Globals.h"

Drivetrain::Drivetrain() : Subsystem("DriveTrain") {

	frc::GetClock();
	const static int frontLeftChannel	= 0;
    const static int rearLeftChannel	= 1;
    const static int frontRightChannel	= 2;
    const static int rearRightChannel	= 3;

    const static int joystickChannel	= 0;

    mainDriveStick = new Joystick(joystickChannel);

    frontLeftMotor = new Victor(frontLeftChannel);
    rearLeftMotor = new Victor(rearLeftChannel);
    frontRightMotor = new Victor(frontRightChannel);
    rearRightMotor = new Victor(rearRightChannel);

    frontLeftEncoder = new Encoder(0,1);
    rearLeftEncoder = new Encoder(2,3);
    frontRightEncoder = new Encoder(4,5);
    rearRightEncoder = new Encoder(6,7);


    frontLeftController = new PIDController(4, 0, 0, new frontLeftPIDSource(),
			new frontLeftPIDOutput());
    rearLeftController = new PIDController(4, 0, 0, new rearLeftPIDSource(),
    		new rearLeftPIDOutput());
    frontRightController = new PIDController(4, 0, 0, new frontRightPIDSource(),
    		new frontRightPIDOutput());
    rearRightController = new PIDController(4, 0, 0, new rearRightPIDSource(),
    		new rearRightPIDOutput());

    encoders = std::vector<Encoder*>();
    encoders = {frontLeftEncoder, rearLeftEncoder,
    			frontRightEncoder, rearRightEncoder};

    motors = std::vector<SpeedController*>();
    motors = {frontLeftMotor, rearLeftMotor,
        	  frontRightMotor, rearRightMotor};

    controllers = std::vector<PIDController*>();
    controllers = {frontLeftController, rearLeftController,
                   frontRightController, rearRightController};


}

void Drivetrain::InitDefaultCommand()
{

}

double Drivetrain::GetEncoderSpeed(int index){
	Encoder *enc = encoders[index];
	return (1/enc->GetPeriod())/9;
}

double Drivetrain::GetEncoderCount(int index){
	Encoder *enc = encoders[index];
	return (enc->Get()/13.521)-encoderOffset;
}

double Drivetrain::GetEncoderDistance(int index){
	return wheelCircumference*GetEncoderCount(index);
}

void Drivetrain::InitEncoders(){
	encoderOffset = GetEncoderDistance(0);
}

void Drivetrain::DriveMotor(int index, double speed){
	SpeedController *motor = motors[index];
	motor->Set(speed);
}

std::vector<double> Drivetrain::CalculateOmegas(double x, double y, double z){
	std::vector<double> omegas;
	double scalar = -(1/wheelRadius);
	omegas[0] = (y-x-(wheelLength+wheelWidth))*scalar;
	omegas[1] = (y+x+(wheelLength+wheelWidth))*scalar;
	omegas[2] = (y+x-(wheelLength+wheelWidth))*scalar;
	omegas[3] = (y-x+(wheelLength+wheelWidth))*scalar;
	return omegas;
}


void Drivetrain::Drive(double x,double y,double z){
	std::vector<double> omegas = CalculateOmegas(x,y,z);
	for(int i = 0; i < 4; i++){
		PIDController* c = controllers[i];
		c->Disable();
		c->SetSetpoint(omegas[i]);
		c->Enable();
	}
}

void Drivetrain::DriveWithStick(int facing){
	float x = mainDriveStick->GetX();
	float y = mainDriveStick->GetY();
	float z = mainDriveStick->GetZ();
	Drive(-x*facing,y*facing,-z);
}

double frontLeftPIDSource::PIDGet() {
	return theDrivetrain->GetEncoderDistance(0);
}

void frontLeftPIDOutput::PIDWrite(double d) {
	theDrivetrain->DriveMotor(0,d);
}
double rearLeftPIDSource::PIDGet() {
	return theDrivetrain->GetEncoderDistance(1);
}

void rearLeftPIDOutput::PIDWrite(double d) {
	theDrivetrain->DriveMotor(1,d);
}
double frontRightPIDSource::PIDGet() {
	return theDrivetrain->GetEncoderDistance(2);
}

void frontRightPIDOutput::PIDWrite(double d) {
	theDrivetrain->DriveMotor(0,2);
}
double rearRightPIDSource::PIDGet() {
	return theDrivetrain->GetEncoderDistance(3);
}

void rearRightPIDOutput::PIDWrite(double d) {
	theDrivetrain->DriveMotor(3,d);
}
