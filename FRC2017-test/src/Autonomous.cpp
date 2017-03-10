/*
 * Autonomous.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: max
 */

#include <Autonomous.h>

void Autonomous::init() {
	current = strafeUntil(12*7);
}

void Autonomous::periodic() {
	if (!finished) {
		if (!current.periodic()) {

			switch (state) {
			case State::Strafe:
				current = driveForward(12*20);
				break;
			case State::Forward:
				finished = true;
				break;
			}
		}
	}
}
