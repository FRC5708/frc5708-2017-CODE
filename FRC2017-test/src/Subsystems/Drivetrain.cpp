#include <Subsystems/Drivetrain.h>
#include "../RobotMap.h"
#include "WPILib.h"
#include <vector>
#include "Globals.h"

Drivetrain::Drivetrain() : Subsystem("DriveTrain") {

	frc::GetClock();
	const static int frontLeftChannel	= 9;
    const static int rearLeftChannel	= 5;
    const static int frontRightChannel	= 8;
    const static int rearRightChannel	= 7;

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

    double p = 0.1;
    double i = 0.001;
    double d = 0;

    frontLeftController = new PIDController(p, i, d, new frontLeftPIDSource(),
			new frontLeftPIDOutput());
    rearLeftController = new PIDController(p, i, d, new rearLeftPIDSource(),
    		new rearLeftPIDOutput());
    frontRightController = new PIDController(p, i, d, new frontRightPIDSource(),
    		new frontRightPIDOutput());
    rearRightController = new PIDController(p, i, d, new rearRightPIDSource(),
    		new rearRightPIDOutput());

    encoders = std::vector<Encoder*>();
    encoders = {frontLeftEncoder, rearLeftEncoder,
    			frontRightEncoder, rearRightEncoder};

    //motors = std::vector<SpeedController*>();
    motors = {frontLeftMotor, rearLeftMotor,
        	  frontRightMotor, rearRightMotor};

    controllers = std::vector<PIDController*>();
    controllers = {frontLeftController, rearLeftController,
                   frontRightController, rearRightController};
}

void Drivetrain::InitPids() {
	for (int i = 0; i < 4; i++){
	    	PIDController *p = controllers[i];
	    	p->SetAbsoluteTolerance(0.01);
	    	p->Reset();
	    	p->Enable();
	    }
}

void Drivetrain::InitDefaultCommand()
{

}

double Drivetrain::GetEncoderSpeed(int index){
	Encoder *enc = encoders[index];
	char* string = (char*)malloc(12);
	sprintf(string, "encoder %d:", index);
	double toReturn = (1/enc->GetPeriod())/9;
	SmartDashboard::PutNumber(string, toReturn);
	return toReturn;
	return 0;
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
	omegas = {
		(x+z+y)*MAX_SPEED,
		(y+z-x)*MAX_SPEED,
		(y-z-x)*MAX_SPEED,
		(y-z+x)*MAX_SPEED
	};
	return omegas;
}


void Drivetrain::Drive(double x,double y,double z){
	std::vector<double> omegas = CalculateOmegas(x,y,z);
	for(int i = 0; i < 4; i++){
		PIDController* c = controllers[i];
		c->SetSetpoint(omegas[i]);
	}
}

void Drivetrain::DriveWithStick(int facing){
	/*int pov = mainDriveStick->GetPOV();
	if (pov != 0) {

		int sneak = .25;
		int x = 0;
		int y = 0;

		switch(pov) {
		case 0 : x = 0; y = sneak; break;
		case 1 : x = sneak; y = sneak; break;
		case 2 : x = sneak; y = 0; break;
		case 3 : x = sneak; y = -sneak; break;
		case 4 : x = 0; y = -sneak; break;
		case 5 : x = -sneak; y = -sneak; break;
		case 6 : x = -sneak; y = 0; break;
		case 7 : x = -sneak; y = sneak; break;
		}
		
		Drive(-x*facing, y*facing, -mainDriveStick->GetZ());
		
	}
	else {*/

		float x = mainDriveStick->GetX();
		float y = mainDriveStick->GetY();
		float z = mainDriveStick->GetZ();
		Drive(-x*facing,y*facing,-z);
		SmartDashboard::PutNumber("joystickX",x);
		SmartDashboard::PutNumber("joystickY",y);
		SmartDashboard::PutNumber("joystickZ",z);
	//}
}

double frontLeftPIDSource::PIDGet() {
	return theDrivetrain->GetEncoderSpeed(0);
}

void frontLeftPIDOutput::PIDWrite(double d) {
	theDrivetrain->DriveMotor(0,d);
	SmartDashboard::PutNumber("FLout",d);
}
double rearLeftPIDSource::PIDGet() {
	return theDrivetrain->GetEncoderSpeed(1);
}

void rearLeftPIDOutput::PIDWrite(double d) {
	theDrivetrain->DriveMotor(1,d);
	SmartDashboard::PutNumber("RLout",d);
}
double frontRightPIDSource::PIDGet() {
	return theDrivetrain->GetEncoderSpeed(2);
}

void frontRightPIDOutput::PIDWrite(double d) {
	theDrivetrain->DriveMotor(2,d);
	SmartDashboard::PutNumber("FRout",d);
}
double rearRightPIDSource::PIDGet() {
	return theDrivetrain->GetEncoderSpeed(3);
}

void rearRightPIDOutput::PIDWrite(double d) {
	theDrivetrain->DriveMotor(3,d);
	SmartDashboard::PutNumber("RRout",d);
}
