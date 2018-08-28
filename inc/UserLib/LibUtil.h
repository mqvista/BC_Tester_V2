/*
 * LibUtil.h
 *
 *  Created on: 2018年8月28日
 *      Author: mqvista
 */

#ifndef USERLIB_LIBUTIL_H_
#define USERLIB_LIBUTIL_H_
#include <stm32f10x.h>

class LibUtil {
public:
	LibUtil();
	void GetMcuUUID(uint32_t* ID);
};

#endif /* USERLIB_LIBUTIL_H_ */
