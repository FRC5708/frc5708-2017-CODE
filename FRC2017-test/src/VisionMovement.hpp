#ifndef SRC_VISIONMOVEMENT_HPP_
#define SRC_VISIONMOVEMENT_HPP_

#include <networktables/NetworkTable.h>
#include "Subsystems/Drivetrain.h"
#include "AutonUntil.h"


class DoVision : public AutonUntil {

	typedef enum {
		Turning,
		Strafing,
		Forward,
		Done
	} VisionState;
	
	NetworkTable* table;
	
	// Drive() on the drivetrain must not be called while active.
	VisionState state = VisionState::Turning;
	
public:
	inline DoVision(NetworkTable* table) { this->table = table; }
	// returns false when finished
	void start() override;
	bool periodic();
	
private:
	double getValue(double visionOutput, double tolerance);
	double getStickValue(double value, double tolerance, double correctionTolerance, VisionState relevantState, VisionState nextState);
};



#endif /* SRC_VISIONMOVEMENT_HPP_ */
