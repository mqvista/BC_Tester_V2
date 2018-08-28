/*
 * LibFlash.h
 *
 *  Created on: 2018年8月24日
 *      Author: mqvista
 */
#include <stm32f10x.h>


#ifndef USERLIB_LIBFLASH_H_
#define USERLIB_LIBFLASH_H_
#define SettingSize (sizeof(ConfigInfo) % 4 != 0 ? \
					(sizeof(ConfigInfo) >> 2) + 1 : \
					sizeof(ConfigInfo) >> 2)


class LibFlash {
public:
	LibFlash();
	void WriteSerialNum(uint32_t *data);
	void WriteDataNum(uint32_t *data);
	void WriteSenserCali(uint8_t senNum ,uint16_t *MX, uint16_t *MN, double *CA, double *CB);

	void GetSerialNum(uint32_t *data);
	void GetDataNum(uint32_t *data);
	void GetSenserCali(uint8_t senNum ,uint16_t *MX, uint16_t *MN, double *CA, double *CB);


	struct ConfigInfo
	{
		// Serial Number
		uint32_t SerialNum;

		// Manufacture Data
		uint32_t MfDate;

		// Senser base info
		struct SenserBaseInfo
		{
			uint16_t MX;
			uint16_t MN;
			double CA;
			double CB;

		}Senser1, Senser2;
	};

	// a container for save and load config
	union Container
	{
		ConfigInfo CfInfo;
		uint32_t Word[SettingSize];
	};

	Container container;

	FLASH_Status FLASHStatus = FLASH_COMPLETE;
};

#endif /* USERLIB_LIBFLASH_H_ */
