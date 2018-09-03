#ifndef __USB_IT_H
#define __USB_IT_H
#include "stm32f10x.h"
#include "usb_istr.h"

#ifdef __cplusplus
 extern "C" {
#endif

void USB_LP_CAN1_RX0_IRQHandler(void);

void USBWakeUp_IRQHandler(void);

#ifdef __cplusplus
 }
#endif
#endif


