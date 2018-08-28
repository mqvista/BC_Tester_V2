/*
 * LibPower.h
 *
 *  Created on: 2018年8月14日
 *      Author: mqvista
 */

#ifndef USERLIB_LIBPOWER_H_
#define USERLIB_LIBPOWER_H_

#include <stm32f10x.h>


#define STDSta GPIO_Pin_10
#define ChSta GPIO_Pin_11
#define PwrKey GPIO_Pin_6
#define PwrON GPIO_Pin_5
#define Key2 GPIO_Pin_7
#define Key3 GPIO_Pin_8
#define Key4 GPIO_Pin_9

class LibPower {
public:
	LibPower();
	void InitPower();
	bool FlagCharge;
	bool FlagChFsh;
	bool FlagIsON;
	bool FlagLastIsON;
	float BatVtg;
	void converyBatVtg(uint16_t adValue);
	int8_t CurrentBatLevel;
	int8_t LastBatLevel;
	void updateBatInfo();
	bool LastLightning;
	bool CurrentLightning;
};

#endif /* USERLIB_LIBPOWER_H_ */
