#ifndef SRC_VISIONMOVEMENT_HPP_
#define SRC_VISIONMOVEMENT_HPP_

#include <networktables/NetworkTable.h>
#include "Subsystems/Drivetrain.h"



class VisionMovement {
	
	typedef enum {
		Turning,
		Strafing,
		Forward,
		Done
	} VisionState;
	
	NetworkTable* table;
	
	// Drive() on the drivetrain must not be called while active.
	Drivetrain* drivetrain;
	VisionState state = VisionState::Turning;
	
public:
	VisionMovement(NetworkTable* table, Drivetrain* drivetrain);
	// returns false when finished
	bool periodic();
	
private:
	double getValue(double visionOutput, double tolerance);
	double getStickValue(double value, double tolerance, double correctionTolerance, VisionState relevantState, VisionState nextState);
	
	
};


#endif /* SRC_VISIONMOVEMENT_HPP_ */
