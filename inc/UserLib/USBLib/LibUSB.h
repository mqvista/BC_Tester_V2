/*
 * LibUSB.h
 *
 *  Created on: 2018年8月28日
 *      Author: mqvista
 */

#ifndef INC_USERLIB_USBLIB_LIBUSB_H_
#define INC_USERLIB_USBLIB_LIBUSB_H_
#include "stm32f10x.h"
#include <string.h>
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"


class LibUSB {
public:
	LibUSB();
	void Init();
	void ReflushReceiveData();
};

#endif /* INC_USERLIB_USBLIB_LIBUSB_H_ */
