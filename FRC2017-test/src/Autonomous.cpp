/*
 * Autonomous.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: max
 */

#include "Autonomous.h"
#include "Globals.h"
#include "Subsystems/Drivetrain.h"

void Autonomous::init(std::vector<DriveUntil> instructions) {
	this->instructions = instructions;
	current = this->instructions.begin();
	current->start();
}

void Autonomous::periodic() {
	if (!finished) {
		if (!current->periodic()) {
			++current;
			if (current != instructions.end()) current->start();
			else finished = true;
		}
	}
	else theDrivetrain->Drive(0, 0, 0);
}
