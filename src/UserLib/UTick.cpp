/*
 * UTick.cpp
 *
 *  Created on: 20171213
 *      Author: Romeli
 */

#include <UserLib/UTick.h>

//RCC_ClocksTypeDef UTick::Clock;
//
//bool UTick::_InitFlag = false;
//volatile uint64_t UTick::_Now = 0;
//uint64_t UTick::_Last = 0;
//uint32_t UTick::_IntervalMs = 0;
//uint32_t UTick::_IntervalUs = 0;
//uint32_t UTick::_ClockInterval = 0;
//uint32_t UTick::_ClockMs = 0;
//uint32_t UTick::_ClockUs = 0;

/*
 * author Romeli
 * explain 初始化系统滴答计时
 * param us 最小时间刻度，单位微妙
 * return void
 */
UTick uTick;

void UTick::Init() {
	RCC_GetClocksFreq(&Clock);
	_IntervalMs = uint64_t(0x1000000) * 1000 / Clock.SYSCLK_Frequency;
	_IntervalUs = _IntervalMs * 1000;
	_ClockInterval = Clock.SYSCLK_Frequency / 1000 * _IntervalMs;
	_ClockMs = Clock.SYSCLK_Frequency / 1000;
	_ClockUs = _ClockMs / 1000;
	//设置SysTick中断间隔时间25mS
	SysTick_Config(_ClockInterval);
	NVIC_SetPriority(SysTick_IRQn, 0);
	_InitFlag = true;
}

/*
 * author Romeli
 * explain 等待一段时间
 * param us 等待的时间，微妙
 * return void
 */
void UTick::uWait(uint64_t us) {
	if (_InitFlag) {
		_Last = Micros();					//Record time_now
		while ((Micros() - _Last) < us)
			;
	} else {
		//Error @Romeli 系统滴答没有初始化
		UDebugOut("System tick has no be inited");
	}
}

/*
 * author Romeli
 * explain 等待一段时间
 * param ms 等待的时间，毫秒
 * return void
 */
void UTick::mWait(uint64_t ms) {
	if (_InitFlag) {
		_Last = Millis();					//Record time_now
		while ((Millis() - _Last) < ms)
			;
	} else {
		//Error @Romeli 系统滴答没有初始化
		UDebugOut("System tick has no be inited");
	}
}

/*
 * author Romeli
 * explain 获取开机到现在的计时，单位毫秒
 * return uint64_t
 */
uint64_t UTick::Millis() {
	if (_InitFlag) {
		uint32_t tick, tick2;
		uint64_t ms, ms2;

		//中断插入的时间点
		//1：中断优先级是最高的，所有数据安全 ms=ms2
		tick = SysTick->VAL;
		//2：比实际值大一个周期，ms=ms2
		ms = _Now;
		//3：所有数据安全 ms<ms2
		tick2 = SysTick->VAL;
		//4：比实际值大一个周期，ms<ms2
		ms2 = _Now;
		//5：所有数据安全 ms=ms2

		//分析下来，中断插入点只会的影响会导致计算值偏大，所以取小的那个
		if (ms == ms2) {
			//可能性2&5，第一组可能不安全，返回第二组
			return ms2 + (_IntervalMs - (tick2 / _ClockMs));
		} else if (ms < ms2) {
			//可能性3&4，第二组可能不安全，返回第一组
			return ms + (_IntervalMs - (tick / _ClockMs));
		} else {
			//Error @Romeli 不可能出现的情况
			UDebugOut("System tick unknown error");
		}
	} else {
		//Error @Romeli 系统滴答没有初始化
		UDebugOut("System tick has no be inited");
	}
	return 0;
}

/*
 * author Romeli
 * explain 获取开机到现在的计时，单位微妙
 * return uint64_t
 */
uint64_t UTick::Micros() {
	if (_InitFlag) {
		uint32_t tick, tick2;
		uint64_t ms, ms2;

		//中断插入的时间点
		//1：中断优先级是最高的，所有数据安全 ms=ms2
		tick = SysTick->VAL;
		//2：比实际值大一个周期，ms=ms2
		ms = _Now;
		//3：所有数据安全 ms<ms2
		tick2 = SysTick->VAL;
		//4：比实际值大一个周期，ms<ms2
		ms2 = _Now;
		//5：所有数据安全 ms=ms2

		//分析下来，中断插入点只会的影响会导致计算值偏大，所以取小的那个
		if (ms == ms2) {
			//可能性2&5，第一组可能不安全，返回第二组
			return ms2 * 1000 + (_IntervalUs - (tick2 / _ClockUs));
		} else if (ms < ms2) {
			//可能性3&4，第二组可能不安全，返回第一组
			return ms * 1000 + (_IntervalUs - (tick / _ClockUs));
		} else {
			//Error @Romeli 不可能出现的情况
			UDebugOut("System tick unknown error");
		}
	} else {
		//Error @Romeli 系统滴答没有初始化
		UDebugOut("System tick has no be inited");
	}
	return 0;
}

/*
 * author Romeli
 * explain 中断服务子程序，用于计时
 */
inline void UTick::IRQ() {
	_Now += _IntervalMs;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"
extern "C" void SysTick_Handler() {
	uTick.IRQ();
}
#pragma GCC diagnostic pop
