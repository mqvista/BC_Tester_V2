/*
 * LibSPI.h
 *
 *  Created on: 2018年2月23日
 *      Author: mqvista
 */

#ifndef LIBSPI_H_
#define LIBSPI_H_
#include "stm32f10x_spi.h"

class LibSPI {
public:
	LibSPI();
	void SPI2Init();
	uint8_t SPI2ReadWriteByte(uint8_t TxData);




};

#endif /* LIBSPI_H_ */
