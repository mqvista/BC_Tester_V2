/*
 ******************************************************************************
 File:     main.cpp
 Info:     Generated by Atollic TrueSTUDIO(R) 9.0.0   2018-08-06

 The MIT License (MIT)
 Copyright (c) 2018 STMicroelectronics

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 ******************************************************************************
 */

/* Includes */
#include <stddef.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "Moudles.h"
#include <UserLib/UTick.h>

/**
 **===========================================================================
 **
 **  Abstract: main program
 **9
 **===========================================================================
 */

int main(void) {
	uTick.Init();
	libUSB.Init();
	libPower.InitPower();
	uint32_t ttt[3];
	libUtil.GetMcuUUID(ttt);


	libADC.InitADC();
	ili9341.Init();
	ili9341.LCD_Clean(0x00);

	/*
	// display area line
	for (uint16_t i = 0; i < 240; i++) {
		ili9341.LCD_DrawPoint(10, i, 0x07E0);
	}
	for (uint16_t i = 0; i < 240; i++) {
		ili9341.LCD_DrawPoint(320, i, 0x07E0);
	}
	for (uint16_t i = 0; i < 320; i++) {
		ili9341.LCD_DrawPoint(i, 76, 0x07E0);
	}
	for (uint16_t i = 0; i < 320; i++) {
		ili9341.LCD_DrawPoint(i, 200, 0x07E0);
	}
	*/


	while (1) {
		libPower.updateBatInfo();
		libUSB.ReflushReceiveData();

		// get if is power on or off charging
		if (libPower.FlagIsON) {
			double tempReal;
			uint8_t errorCode;

			// show senser lable
			if (!libPower.FlagLastIsON)
			{
				libSenser.OpenSystemMode();
				libPower.FlagLastIsON = true;
			}
			// update show Senser info
			libSenser.UpdateSenserLable();
			// get if senser is in right instance
			if (!libSenser.GetSenserValue(&tempReal, &errorCode)) {
				if (errorCode == 0) {
					ili9341.LCD_ShowString(50, 120, 0xFFFF,
							(uint8_t*) "Too Near  ");
					continue;
					// too near
				} else if (errorCode == 1) {
					// too far
					ili9341.LCD_ShowString(50, 120, 0xFFFF,
							(uint8_t*) "Too  Far  ");
					continue;
				}
			} else {
				if (!libSenser.Is_Inch) {
					ili9341.LCD_ShowFloat(50, 120, 0xFFFF, 3, tempReal);
					ili9341.LCD_ShowString(194, 120, 0xFFFF, (uint8_t *) "  mm");
				} else {
					ili9341.LCD_ShowFloat(50, 120, 0xFFFF, 5, tempReal);
					ili9341.LCD_ShowString(242, 120, 0xFFFF, (uint8_t *) "in");
				}
			}
		}
		else
		{
			if (libPower.FlagLastIsON)
			{
				libSenser.CloseSystemMode();
				libPower.FlagLastIsON = false;
			}
		}
	}
}












/*
 * Minimal __assert_func used by the assert() macro
 * */
extern "C" void __assert_func(const char *file, int line, const char *func,
		const char *failedexpr) {
	while (1) {
	}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
extern "C" void __assert(const char *file, int line, const char *failedexpr) {
	__assert_func(file, line, NULL, failedexpr);
}
