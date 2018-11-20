/*
 * LibComm.h
 *
 *  Created on: 2018年9月4日
 *      Author: mqvista
 */

#ifndef SRC_USERLIB_LIBCOMM_H_
#define SRC_USERLIB_LIBCOMM_H_
#include <stm32f10x.h>
#include <string.h>

enum Action_Typedef {
	Action_Unknown,	// 0
	Action_Set,		// 1
	Action_Get		// 2
};

enum Function_Typedef {
	Function_Unknown,	// 0
	Function_Set,		// 1
	Function_Get		// 2
};

enum Instruct_Typedef {
	Instruct_Unknown,
	Instruct_SerialNum,
	Instruct_MFDate,
	Instruct_MCUID,
	Instruct_Senser
};

class LibComm {
public:
	LibComm();
	void Communication(uint8_t *&RecData, const uint32_t RecLength, uint8_t *SendData, uint32_t &SendLength);

private:
	uint8_t Str2Cmp(const char *a, const char *b);
	Action_Typedef CheckAction(uint8_t *&RecData);
	Instruct_Typedef CheckInstruct(uint8_t *&RecData);

};

#endif /* SRC_USERLIB_LIBCOMM_H_ */
