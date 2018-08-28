/*
 * LibUtil.cpp
 *
 *  Created on: 2018年8月28日
 *      Author: mqvista
 */

#include <UserLib/LibUtil.h>

LibUtil::LibUtil() {
	// TODO Auto-generated constructor stub

}

void LibUtil::GetMcuUUID(uint32_t* ID) {
	ID[0] = *(uint32_t *)0x1FFFF7E8;

	ID[1] = *(uint32_t *)(0x1FFFF7E8 + 0x04);

	ID[2] = *(uint32_t *)(0x1FFFF7E8 + 0x08);
}
