/*
 * Autonomous.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: max
 */

#include <Autonomous.h>

void Autonomous::init() {
	current = driveForward(0);
}

void Autonomous::periodic() {
	if (!finished) {
		if (!current.periodic()) {

			switch (state) {
			case State::Straight1:
				current = turnTo(0);
				break;
			case State::Turning:
				finished = true;
				break;
			}
		}
	}
}
