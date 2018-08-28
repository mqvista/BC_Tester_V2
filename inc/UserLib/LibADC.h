/*
 * LibADC.h
 *
 *  Created on: 2018年8月14日
 *      Author: mqvista
 */

#ifndef USERLIB_LIBADC_H_
#define USERLIB_LIBADC_H_

#include <stm32f10x.h>

class LibADC {
public:
	LibADC(uint8_t overSampleTimes);
	void InitADC();
	void InitDMA();

	uint16_t (*ADValue)[2];
	uint16_t ADSenser[2];
	uint8_t overSampleFlag;
	uint16_t bufferSize;
	void OverSampling();

private:
	uint8_t m_overSampleTimes;
};

#endif /* USERLIB_LIBADC_H_ */
