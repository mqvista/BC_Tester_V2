/*
 * LibPower.cpp
 *
 *  Created on: 2018年8月14日
 *      Author: mqvista
 */

#include <UserLib/LibPower.h>
#include "Moudles.h"

LibPower::LibPower() {
	// TODO Auto-generated constructor stub
	//libPower.FlagIsON = false;
	LastLightning = false;
	FlagLastIsON = false;

}

void LibPower::InitPower() {
	// Init GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = STDSta | ChSta | PwrKey | Key2 | Key3 | Key4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PwrON;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
	// for keys
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);


	// Init interrept
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line10 | EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	// for keys
	EXTI_InitStructure.EXTI_Line = EXTI_Line7 | EXTI_Line8 | EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	// Init NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Get charge status
	if (GPIO_ReadInputDataBit(GPIOB, ChSta) == RESET) {
		FlagCharge = true;
	} else {
		FlagCharge = false;
	}
	if (GPIO_ReadInputDataBit(GPIOB, STDSta) == RESET) {
		FlagChFsh = true;
	} else {
		FlagChFsh = false;
	}
	if (FlagCharge || FlagChFsh )
	{
		CurrentLightning = true;
	}

	// open system power
	GPIO_SetBits(GPIOB, PwrON);
	// Get boot status
	if (GPIO_ReadInputDataBit(GPIOB, PwrKey) == SET) {
		libPower.FlagIsON = true;
	} else {
		libPower.FlagIsON = false;
	}
}

extern "C" {
void EXTI15_10_IRQHandler(void) {
	// Get if in charge
	if (EXTI_GetITStatus(EXTI_Line11) != RESET) {
		if (GPIO_ReadInputDataBit(GPIOB, ChSta) == RESET) {
			libPower.FlagCharge = true;
		} else {
			libPower.FlagCharge = false;
		}
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	// Get if charge finish
	if (EXTI_GetITStatus(EXTI_Line10) != RESET) {
		if (GPIO_ReadInputDataBit(GPIOB, STDSta) == RESET) {
			libPower.FlagChFsh = true;
		} else {
			libPower.FlagChFsh = false;
		}
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	// Set lightning flag
	if (libPower.FlagCharge || libPower.FlagChFsh) {
		libPower.CurrentLightning = true;
	} else {
		libPower.CurrentLightning = false;
		// power off
		if (!libPower.FlagIsON)
		{
			GPIO_ResetBits(GPIOB, PwrON);
		}
	}
}

#include <UserLib/UTick.h>
void EXTI9_5_IRQHandler(void) {
	// Get if user want to switch off or on
	if (EXTI_GetITStatus(EXTI_Line6) != RESET) {
		if (GPIO_ReadInputDataBit(GPIOB, PwrKey) == SET) {
			uTick.mWait(100);
			if (GPIO_ReadInputDataBit(GPIOB, PwrKey) == SET) {
				// get if user want to close system and system is not in charge mode
				if (libPower.FlagIsON && !(libPower.FlagCharge || libPower.FlagChFsh )) {
					libPower.FlagIsON = false;
					GPIO_ResetBits(GPIOB, PwrON);
				}
				// get if user want to close system and system is in charge mode
				else if (libPower.FlagIsON && (libPower.FlagCharge || libPower.FlagChFsh ))
				{
					libPower.FlagIsON = false;
				}
				// get if user want to start system and system is in charge mode
				else if (!libPower.FlagIsON && (libPower.FlagCharge || libPower.FlagChFsh ))
				{
					libPower.FlagIsON = true;
				}
				// get if user want to start system and system is not in charge mode (may be he is debuging)
				else if (!libPower.FlagIsON && (libPower.FlagCharge || libPower.FlagChFsh ))
				{
					GPIO_SetBits(GPIOB, PwrON);
					libPower.FlagIsON = true;
				}
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}

	// set senser to inch unit
	if (EXTI_GetITStatus(EXTI_Line7) != RESET) {
		uTick.mWait(100);
		if (GPIO_ReadInputDataBit(GPIOB, Key2) == RESET) {
			if (libPower.FlagIsON) {
				libSenser.UnitConvery();
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line7);
	}

	// set senser to another senser
	if (EXTI_GetITStatus(EXTI_Line8) != RESET) {
		uTick.mWait(100);
		if (GPIO_ReadInputDataBit(GPIOB, Key3) == RESET) {
			if (libPower.FlagIsON) {
				libSenser.ChangeSenser();
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	// set offset to zero
	if (EXTI_GetITStatus(EXTI_Line9) != RESET) {
		uTick.mWait(100);
		if (GPIO_ReadInputDataBit(GPIOB, Key4) == RESET) {
			if (libPower.FlagIsON) {
				libSenser.SetLengthOffset();
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}
}

void LibPower::converyBatVtg(uint16_t adValue) {
	BatVtg = adValue * 0.001629;
	CurrentBatLevel = (BatVtg - 3.6) / 0.025;
}

void LibPower::updateBatInfo() {
	// update battery level in main loop
	if ((CurrentBatLevel != LastBatLevel) || (CurrentLightning != LastLightning))
	{
		if (CurrentBatLevel>0 && CurrentBatLevel < 23)
		{
			ili9341.LCD_ShowBattery(280, 82, 0xFFFF, CurrentBatLevel);
		}
		// if battery is full and change is finish, set the battery green
		if (CurrentBatLevel >= 23 && FlagChFsh)
		{
			ili9341.LCD_ShowBattery(280, 82, 0x07E0, 23);
		}
		// if battery is full and change is not finish yet, set the battery white
		if (CurrentBatLevel >= 23 && !FlagChFsh)
		{
			ili9341.LCD_ShowBattery(280, 82, 0xFFFF, 23);
		}
		if (CurrentBatLevel <= 0 && CurrentBatLevel >= -1)
		{
			ili9341.LCD_ShowBattery(280, 82, 0xFF00, 0);
		}
		// Power Low Auto off
		if (CurrentBatLevel < -1 && !(FlagChFsh || FlagCharge))
		{
			GPIO_ResetBits(GPIOB, PwrON);
			//ili9341.LCD_ShowBattery(280, 82, 0xFF00, 0);
		}
		LastBatLevel = CurrentBatLevel;
	}

	// update lightning logo in main loop
	if (CurrentLightning != LastLightning)
	{
		if (CurrentLightning)
		{
			ili9341.LCD_ShowLightning(264, 82, 0xFFFF, true);
		}
		else
		{
			ili9341.LCD_ShowLightning(264, 82, 0xFFFF, false);
		}
		LastLightning = CurrentLightning;
	}
}
