/*
 * UTick.h
 *
 *  Created on: 2017年12月30日
 *      Author: Romeli
 */

#ifndef UTICK_H_
#define UTICK_H_

#include <stm32f10x.h>

#define UDebugOut(x)

class UTick {
public:
	RCC_ClocksTypeDef Clock;

	void Init();
	void uWait(uint64_t us);
	void mWait(uint64_t ms);
	inline void Wait(uint64_t s) {
		mWait(s * 1000);
	}
	inline void Tick(uint16_t t) {
		while (t--)
			;
	}
	void WaitOne();
	uint64_t Millis();
	uint64_t Micros();
	void IRQ();
private:
	bool _InitFlag;
	volatile uint64_t _Now;
	uint64_t _Last;
	uint32_t _IntervalMs;
	uint32_t _IntervalUs;
	uint32_t _ClockInterval;
	uint32_t _ClockMs;
	uint32_t _ClockUs;
};

extern UTick uTick;
#endif /* UTICK_H_ */
