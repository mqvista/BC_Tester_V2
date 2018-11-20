/*
 * LibUSB.cpp
 *
 *  Created on: 2018年8月28日
 *      Author: mqvista
 */

#include <LibUSB.h>
#include "Moudles.h"

uint32_t packet_sent = 1;
uint32_t packet_receive = 1;
uint32_t Send_length;
uint8_t Send_Buffer[64];
extern __IO uint8_t Receive_Buffer[64];
extern __IO uint32_t Receive_length;

LibUSB::LibUSB() {
	// TODO Auto-generated constructor stub

}

void LibUSB::Init() {
	Set_System();
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();
}

//Fun_Typedef CheckFun(uint8_t*& str){
//	if(strcmp(get,str)){
//		str+=sizeof(get);
//	}
//}

void LibUSB::PrintData(uint16_t adValue, uint8_t* string)
{
	if (bDeviceState == CONFIGURED) {
		// send adv
		uint8_t len;
		Send_Buffer[0] = '#';
		Send_Buffer[1] = 'A';
		Send_Buffer[2] = 'D';
		Send_Buffer[3] = 'V';
		Send_Buffer[4] = ':';
		len = UConvert::byNumber(adValue, 10, Send_Buffer + 5);
		Send_Buffer[5 + len] = '\r';
		Send_Buffer[6 + len] = '\n';

		// string to user
		uint8_t i = 7 + len;
		Send_Buffer[i++] = '#';
		for (uint8_t j = 0; string[j] != '\0'; ++j) {
			Send_Buffer[i++] = string[j];
		}
		CDC_Send_DATA(Send_Buffer, i);
	}
}

void LibUSB::PrintData(uint16_t adValue, double relValue)
{

	if (bDeviceState == CONFIGURED) {
		// send adv
		uint8_t len;
		Send_Buffer[0] = '#';
		Send_Buffer[1] = 'A';
		Send_Buffer[2] = 'D';
		Send_Buffer[3] = 'V';
		Send_Buffer[4] = ':';
		len = UConvert::byNumber(adValue, 10, Send_Buffer + 5);
		Send_Buffer[5 + len] = '\r';
		Send_Buffer[6 + len] = '\n';

		// send real
		Send_Buffer[7 + len] = '#';
		Send_Buffer[8 + len] = 'R';
		Send_Buffer[9 + len] = 'E';
		Send_Buffer[10 + len] = 'L';
		Send_Buffer[11 + len] = ':';
		UConvert::byFloat(relValue, 7, Send_Buffer + 12 + len);
		Send_Buffer[19 + len] = '\r';
		Send_Buffer[20 + len] = '\n';
		CDC_Send_DATA(Send_Buffer, 21 + len);
	}
}

void LibUSB::PrintData(uint16_t data) {
	if (bDeviceState == CONFIGURED) {
		uint8_t len;
		Send_Buffer[0] = '#';
		Send_Buffer[1] = 'A';
		Send_Buffer[2] = 'D';
		Send_Buffer[3] = 'V';
		Send_Buffer[4] = ':';
		len = UConvert::byNumber(data, 10, Send_Buffer + 5);
		Send_Buffer[5 + len] = '\r';
		Send_Buffer[5 + len + 1] = '\n';\
		CDC_Send_DATA(Send_Buffer, len + 7);
	}
}

void LibUSB::PrintData(double data) {

	if (bDeviceState == CONFIGURED) {
		Send_Buffer[0] = '#';
		Send_Buffer[1] = 'R';
		Send_Buffer[2] = 'E';
		Send_Buffer[3] = 'L';
		Send_Buffer[4] = ':';
		UConvert::byFloat(data, 7, Send_Buffer + 5);
		Send_Buffer[12] = '\r';
		Send_Buffer[13] = '\n';
		CDC_Send_DATA(Send_Buffer, 14);
	}
}

void LibUSB::PrintData(uint8_t* data) {

	if (bDeviceState == CONFIGURED) {
		uint8_t i = 0;
		Send_Buffer[i++] = '#';
		for (uint8_t j = 0; data[j] != '\0'; ++j) {
			Send_Buffer[i++] = data[j];
		}
		CDC_Send_DATA(Send_Buffer, i);
	}
}

void LibUSB::ReflushReceiveData() {
	if (bDeviceState == CONFIGURED) {
		CDC_Receive_DATA();
		if (Receive_length != 0) {
			// create a new data
			uint8_t *RecData;
			RecData = new uint8_t[Receive_length];
			memcpy(RecData, (uint8_t*) Receive_Buffer, Receive_length);
			libComm.Communication(RecData, Receive_length, Send_Buffer,
					Send_length);
			CDC_Send_DATA(Send_Buffer, Send_length);
			//delete[] RecData;

			/*
			 if (strcmp((char*)tmpData, "#Help") == 0)
			 {
			 CDC_Send_DATA((uint8_t*)"QQWWEEEEEEE", sizeof("QQWWEEEEEEE"));
			 }
			 // get device serial number
			 // send srialNumber by hex
			 else if (strcmp((char*)tmpData, "#GetSerialNum") == 0)
			 {
			 uint32_t sn;
			 libFlash.GetSerialNum(&sn);
			 char tmpStr[9];
			 sprintf(tmpStr, "%lx", sn);
			 CDC_Send_DATA((uint8_t*)tmpStr, sizeof(tmpStr));
			 }
			 // Set device serial number
			 // Set srialNumber by hex
			 // Receive length == 22 (without \0)
			 else if (Receive_length == 22)
			 {
			 uint8_t tmpHeader[14];
			 uint8_t tmpTail[9];
			 for (uint8_t i=0; i<13; i++)
			 {
			 tmpHeader[i] = tmpData[i];
			 }
			 tmpHeader[13] = '\0';
			 for (uint8_t i=0; i<8; i++)
			 {
			 tmpTail[i] = tmpData[i+14];
			 }
			 tmpTail[8] = '\0';
			 if (strcmp((char*)tmpHeader, "#SetSerialNum") == 0)
			 {
			 uint32_t tmpSN;
			 tmpSN = strtoul((char*)tmpTail, '\0', 16);
			 libFlash.WriteSerialNum(&tmpSN);
			 CDC_Send_DATA((uint8_t*)"OK", 3);
			 }
			 }

			 // get device Mf date
			 // send Mf date by dec
			 else if (strcmp((char*)tmpData, "#GetMFDate") == 0)
			 {
			 uint32_t MFDate;
			 libFlash.GetDataNum(&MFDate);
			 char tmpStr[11];
			 sprintf(tmpStr, "%lu", MFDate);
			 // get where is '\0'
			 uint8_t i = 0;
			 bool whileFlag = true;
			 while (whileFlag)
			 {
			 if(tmpStr[i] == '\0')
			 {
			 break;
			 }
			 i++;
			 if (i>10)
			 {
			 i=10;
			 whileFlag = false;
			 }
			 }
			 /////////////////////
			 CDC_Send_DATA((uint8_t*)tmpStr, i+1);
			 }
			 // Set device Mf date
			 // Set srialNumber by dec
			 // Receive length == 17 (without \0)
			 else if (Receive_length == 17)
			 {
			 uint8_t tmpHeader[11];
			 uint8_t tmpTail[7];
			 for (uint8_t i=0; i<10; i++)
			 {
			 tmpHeader[i] = tmpData[i];
			 }
			 tmpHeader[10] = '\0';
			 for (uint8_t i=0; i<8; i++)
			 {
			 tmpTail[i] = tmpData[i+11];
			 }
			 tmpTail[6] = '\0';
			 if (strcmp((char*)tmpHeader, "#SetMFDate") == 0)
			 {
			 uint32_t tmpMFDate;
			 tmpMFDate = strtoul((char*)tmpTail, '\0', 10);
			 libFlash.WriteDataNum(&tmpMFDate);
			 CDC_Send_DATA((uint8_t*)"OK", 3);
			 }
			 }

			 //
			 //
			 else if (strcmp((char*)tmpData, "#GetSenser1Cali") == 0)
			 {
			 uint16_t MX, MN;
			 double CA, CB;
			 libFlash.GetSenserCali(1, &MX, &MN, &CA, &CB);
			 uint8_t aaa =sizeof(CA);
			 aaa++;
			 }
			 */

			Receive_length = 0;
		}
	}
}

/*extern "C" void CTR_Callback(void)
{
	libUSB.ctrFlag = false;
}*/
