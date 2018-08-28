/*
 * LibADC.cpp
 *
 *  Created on: 2018年8月14日
 *      Author: mqvista
 */

#include <UserLib/LibADC.h>
#include "Moudles.h"

LibADC::LibADC(uint8_t overSampleTimes) {
	overSampleFlag = 0;
	// set overSample
	m_overSampleTimes = overSampleTimes;
	if (m_overSampleTimes != 0) {
		bufferSize = 1 << (m_overSampleTimes << 1);
	} else {
		bufferSize = 20;
	}

	// domnamyc set array size //
	ADValue = new uint16_t[bufferSize][2];

}

void LibADC::InitADC() {



	// Open GPIO Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	// 12M ADC
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	// GPIO Init
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// ADC Init
	ADC_DeInit(ADC1);
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_NbrOfChannel = 2;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_1Cycles5);
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	InitDMA();

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void LibADC::InitDMA() {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = bufferSize<<1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

	NVIC_InitTypeDef NVIC_DMA1Structure;
	NVIC_DMA1Structure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_DMA1Structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_DMA1Structure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_DMA1Structure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_DMA1Structure);

	DMA_Cmd(DMA1_Channel1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
}

void LibADC::OverSampling() {
	uint32_t totalVAlue = 0;
	for (int i = 0; i < bufferSize; i++)
	{
		totalVAlue +=  ADValue[i][0];
	}
	ADSenser[0] = totalVAlue >> m_overSampleTimes;
}

extern "C" {
	void DMA1_Channel1_IRQHandler(void)
	{
		if (DMA_GetFlagStatus(DMA1_FLAG_TC1))
		{
			libADC.overSampleFlag++;
			if (libADC.overSampleFlag == libADC.bufferSize-1)
			{
				libADC.OverSampling();
				libADC.overSampleFlag = 0;

				// get bat value
				uint32_t tmpBatTotal=0;
				for (uint32_t i=0; i<libADC.bufferSize; i++)
				{
					tmpBatTotal += libADC.ADValue[i][1];
				}

				libADC.ADSenser[1] = tmpBatTotal / libADC.bufferSize;
				libPower.converyBatVtg(libADC.ADSenser[1]);
			}
			DMA_ClearITPendingBit(DMA1_FLAG_TC1);
		}
	}

}


