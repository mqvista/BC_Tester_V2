/*
 * Delay.cpp
 *
 *  Created on: 2018年2月24日
 *      Author: mqvista
 */

#include <UserLib/Delay.h>

Delay::Delay() {
	// TODO Auto-generated constructor stub

}

void Delay::wait(unsigned long n) {
	do {
		n--;
	} while (n);
}
