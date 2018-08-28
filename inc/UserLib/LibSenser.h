/*
 * LibSenser.h
 *
 *  Created on: 2018年8月16日
 *      Author: mqvista
 */

#ifndef USERLIB_LIBSENSER_H_
#define USERLIB_LIBSENSER_H_
#include "stm32f10x.h"

class LibSenser {
public:
	LibSenser();
	bool GetSenserValue(double* length, uint8_t* errorNum);
	uint8_t lastSenser;
	uint8_t currentSenser;
	void SetLengthOffset();
	void UnitConvery();
	bool Is_Inch;
	void ChangeSenser();
	void UpdateSenserLable();
	// when you want to close system, it will help you clean the screen
	void CloseSystemMode();
	void OpenSystemMode();

private:

	struct SenserBaseInfo
	{
		uint16_t MX;
		uint16_t MN;
		double CA;
		double CB;

	}Senser1, Senser2;

	double m_OffsetValue;
	double m_InchCoefficient;
};

#endif /* USERLIB_LIBSENSER_H_ */
