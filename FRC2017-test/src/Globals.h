#ifndef Globals_H
#define Globals_H
#include "WPILib.h"


class Drivetrain;
extern Drivetrain* theDrivetrain;
extern AnalogGyro* theGyro;

#define FOREACH(array, i) for(auto i = (array).begin(); i != (array).end(); ++i)

#include <chrono>
inline std::chrono::milliseconds getTime() {
		return std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now().time_since_epoch());
	}

#endif
