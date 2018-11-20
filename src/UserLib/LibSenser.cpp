/*
 * LibSenser.cpp
 *
 *  Created on: 2018年8月16日
 *      Author: mqvista
 */

#include <UserLib/LibSenser.h>
#include "Moudles.h"

LibSenser::LibSenser() {
	// TODO Auto-generated constructor stub

	Senser1.CA = 0.0569343809311725;
	Senser1.CB = 1.3924236738910398;
	Senser1.MN = 3220;
	Senser1.MX = 10000;

	Senser2.CA = 0.00021814759090782652;
	Senser2.CB = - 0.31080097783673966;
	Senser2.MN = 1000;
	Senser2.MX = 10500;

	lastSenser = 0;
	currentSenser = 1;
	m_OffsetValue = 0;
	m_InchCoefficient = 1;

	Is_Inch = false;
}

bool LibSenser::GetSenserValue(double* length, uint16_t* adcValue, uint8_t* errorNum) {
	uint16_t tempSenser = libADC.ADSenser[0];
	if (currentSenser == 1) {
		if (tempSenser < Senser1.MN) {
			length = 0;
			*errorNum = 0;
			*adcValue = tempSenser;
			return false;
		} else if (tempSenser > Senser1.MX) {
			length = 0;
			*errorNum = 1;
			*adcValue = tempSenser;
			return false;
		} else {
			*length = (Senser1.CA * (tempSenser - Senser1.MN) + Senser1.CB - m_OffsetValue) / 1000 * m_InchCoefficient;
			errorNum = 0;
			*adcValue = tempSenser;
			return true;
		}
	}
	if (currentSenser == 2) {
		if (tempSenser < Senser2.MN) {
			length = 0;
			errorNum = 0;
			*adcValue = tempSenser;
			return false;
		} else if (tempSenser > Senser2.MX) {
			length = 0;
			*errorNum = 1;
			*adcValue = tempSenser;
			return false;
		} else {
			*length = (Senser2.CA * (tempSenser - Senser2.MN) + Senser2.CB - m_OffsetValue) * m_InchCoefficient;
			errorNum = 0;
			*adcValue = tempSenser;
			return true;
		}
	}
	*errorNum = 2;
	return false;
}

void LibSenser::SetLengthOffset() {
	if (m_OffsetValue != 0)
	{
		m_OffsetValue = 0;
		return;
	}
	else
	{
		uint16_t tempSenser = libADC.ADSenser[0];
		if (currentSenser == 1)
		{
			m_OffsetValue = Senser1.CA  * (tempSenser - Senser1.MN) + Senser1.CB;
			return;
		}
		else if (currentSenser == 2)
		{
			m_OffsetValue = Senser2.CA * (tempSenser - Senser2.MN) + Senser2.CB;
		}
	}
}

void LibSenser::UnitConvery() {
	if (m_InchCoefficient == 1)
	{
		m_InchCoefficient =  0.0393700787;
		Is_Inch = true;
	}
	else
	{
		m_InchCoefficient =  1;
		Is_Inch = false;
	}
}

void LibSenser::ChangeSenser() {
	if (currentSenser == 1)
	{
		currentSenser = 2;
	}
	else
	{
		currentSenser = 1;
	}
}

void LibSenser::UpdateSenserLable() {

	if (currentSenser != lastSenser)
	{
		lastSenser = currentSenser;
		if (currentSenser == 1)
		{
			ili9341.LCD_ShowString_1608(30, 82, "Senser1", 0x00, 0xFFFF);
		}
		else
		{
			ili9341.LCD_ShowString_1608(30, 82, "Senser2", 0x00, 0xFFFF);
		}
	}
}

void LibSenser::CloseSystemMode() {
	// clean the senser lable
	lastSenser = 0;
	ili9341.LCD_ShowSenserLable(20, 82, 58, 0x00);

	// clean senser value
	ili9341.LCD_Fill(50, 290, 120, 168, 0x00);
}

void LibSenser::OpenSystemMode() {
	// show the senser lable
	ili9341.LCD_ShowSenserLable(20, 82, 58, 0xFFFF);
}
