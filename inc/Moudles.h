/*
 * Moudles.h
 *
 *  Created on: 2018年8月6日
 *      Author: mqvista
 */

#ifndef MOUDLES_H_
#define MOUDLES_H_

#include "UserLib/ILI9341.h"
#include "UserLib/LibADC.h"
#include "UserLib/LibPower.h"
#include "UserLib/LibSenser.h"
#include "UserLib/LibFlash.h"
#include "UserLib/LibUtil.h"
#include "LibUSB.h"

extern ILI9341 ili9341;
extern LibADC libADC;
extern LibPower libPower;
extern LibSenser libSenser;
extern LibFlash libFlash;
extern LibUtil libUtil;
extern LibUSB libUSB;


#endif /* MOUDLES_H_ */
