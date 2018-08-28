/*
 * LibFlash.cpp
 *
 *  Created on: 2018年8月24日
 *      Author: mqvista
 */

#include <UserLib/LibFlash.h>
#define START_ADDRESS ((uint32_t)0x0800FC00)
#define FLASH_PAGE_SIZE ((uint16_t)0x400)

LibFlash::LibFlash() {
	// TODO Auto-generated constructor stub

}

void LibFlash::WriteSerialNum(uint32_t* data) {
	__disable_irq();
	// Read first
	for (uint8_t i = 0; i< SettingSize; i++)
	{
		container.Word[i] = *(uint32_t *) (START_ADDRESS + (i << 2));
	}

	// Write new Date to container;
	container.CfInfo.SerialNum = *data;

	// Save to flash
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	// Erase Flash
	FLASH_ErasePage(START_ADDRESS);

	//
	for (uint8_t i = 0; i< SettingSize; i++)
	{
		FLASH_ProgramWord(START_ADDRESS + (i << 2), container.Word[i]);
	}
	FLASH_Lock();
	__enable_irq();
}

void LibFlash::WriteDataNum(uint32_t* data) {
	__disable_irq();
	// Read first
	for (uint8_t i = 0; i< SettingSize; i++)
	{
		container.Word[i] = *(uint32_t *) (START_ADDRESS + (i << 2));
	}

	// Write new Date to container;
	container.CfInfo.MfDate = *data;

	// Save to flash
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	// Erase Flash
	FLASH_ErasePage(START_ADDRESS);

	//
	for (uint8_t i = 0; i< SettingSize; i++)
	{
		FLASH_ProgramWord(START_ADDRESS + (i << 2), container.Word[i]);
	}
	FLASH_Lock();
	__enable_irq();
}

void LibFlash::WriteSenserCali(uint8_t senNum, uint16_t* MX, uint16_t* MN,
		double* CA, double* CB) {
	// Read first
	for (uint8_t i = 0; i< SettingSize; i++)
	{
		container.Word[i] = *(uint32_t *) (START_ADDRESS + (i << 2));
	}
	switch (senNum)
	{
	case 1:
		container.CfInfo.Senser1.MX = *MX;
		container.CfInfo.Senser1.MN = *MN;
		container.CfInfo.Senser1.CA = *CA;
		container.CfInfo.Senser1.CB = *CB;
		break;
	case 2:
		container.CfInfo.Senser2.MX = *MX;
		container.CfInfo.Senser2.MN = *MN;
		container.CfInfo.Senser2.CA = *CA;
		container.CfInfo.Senser2.CB = *CB;
		break;
	default:
		return;
		break;
	}

	__disable_irq();
	// Save to flash
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	// Erase Flash
	FLASH_ErasePage(START_ADDRESS);

	//
	for (uint8_t i = 0; i< SettingSize; i++)
	{
		FLASH_ProgramWord(START_ADDRESS + (i << 2), container.Word[i]);
	}
	FLASH_Lock();
	__enable_irq();
}

void LibFlash::GetSerialNum(uint32_t* data) {
	// Read first
	for (uint8_t i = 0; i< SettingSize; i++)
	{
		container.Word[i] = *(uint32_t *) (START_ADDRESS + (i << 2));
	}

	// Write Date to pointer;
	*data = container.CfInfo.SerialNum;
}

void LibFlash::GetDataNum(uint32_t* data) {
	// Read first
	for (uint8_t i = 0; i< SettingSize; i++)
	{
		container.Word[i] = *(uint32_t *) (START_ADDRESS + (i << 2));
	}

	// Write Date to pointer;
	*data = container.CfInfo.MfDate;
}

void LibFlash::GetSenserCali(uint8_t senNum, uint16_t* MX, uint16_t* MN,
		double* CA, double* CB) {
	// Read first
	for (uint8_t i = 0; i< SettingSize; i++)
	{
		container.Word[i] = *(uint32_t *) (START_ADDRESS + (i << 2));
	}

	// Write Date to pointer;
	switch (senNum)
	{
	case 1:
		*MX = container.CfInfo.Senser1.MX;
		*MN = container.CfInfo.Senser1.MN;
		*CA = container.CfInfo.Senser1.CA;
		*CB = container.CfInfo.Senser1.CB;
		break;
	case 2:
		*MX = container.CfInfo.Senser2.MX;
		*MN = container.CfInfo.Senser2.MN;
		*CA = container.CfInfo.Senser2.CA;
		*CB = container.CfInfo.Senser2.CB;
		break;
	default:
		break;
	}
}
