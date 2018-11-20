/*
 * LibComm.cpp
 *
 *  Created on: 2018年9月4日
 *      Author: mqvista
 */

#include <UserLib/LibComm.h>

#define UnknownStr "Unknown parameter\r\n"
#define SetStr "Set"
#define GetStr "Get"
#define SerialNumStr "SerialNum"
#define MFDateStr "MFDate"



LibComm::LibComm() {
	// TODO Auto-generated constructor stub

}

void LibComm::Communication(uint8_t *&RecData, const uint32_t RecLength, uint8_t *SendData, uint32_t &SendLength)
{
	// Decide paramer is right or not
	if (!((RecData[0] == '#') && (RecData[RecLength - 1] == '\n') && (RecData[RecLength - 2] == '\r')))
	{
		SendLength = sizeof(UnknownStr)-1;
		memcpy(SendData, UnknownStr, SendLength);
		return;
	}
	RecData++;

	// Set or get
	switch (CheckAction(RecData))
	{
	case Action_Set:


		break;
	case Action_Get:

		break;
	case Action_Unknown:
		// back to cdc param error
		SendLength = sizeof(UnknownStr)-1;
		memcpy(SendData, UnknownStr, SendLength);
		break;
	}
}





// check is set or get
Action_Typedef LibComm::CheckAction(uint8_t*& RecData) {
	if (Str2Cmp((char*)RecData, SetStr) == 0)
	{
		// 偏移到后面一个参数
		RecData+=sizeof(SetStr);
		return Action_Set;
	}
	if (Str2Cmp((char*)RecData, GetStr) == 0)
	{
		// 偏移到后面一个参数
		RecData+=sizeof(GetStr);
		return Action_Get;
	}
	return Action_Unknown;
}

uint8_t LibComm::Str2Cmp(const char* a, const char* b) {
	while(*a == *b)
	{
		a++;
		b++;
	}
	if ((*a == ';') || (*a == '\r') || (*b == ';') || (*b == '\r'))
	{
		return 0;
	}
	if (*a - *b > 0)
	{
		return 1;
	}
	else
		return -1;
}

Instruct_Typedef LibComm::CheckInstruct(uint8_t*& RecData) {
	if (Str2Cmp((char*)RecData, SerialNumStr) == 0)
	{
		// 偏移到后面一个参数
		RecData+=sizeof(SerialNumStr);
		return Instruct_SerialNum;
	}
	else if (Str2Cmp((char*)RecData, SerialNumStr) == 0)
	{
		// 偏移到后面一个参数
		RecData+=sizeof(SerialNumStr);
		return Instruct_SerialNum;
	}
}
