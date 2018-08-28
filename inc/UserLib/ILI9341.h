/*
 * ILI9341.h
 *
 *  Created on: Feb 23, 2018
 *      Author: mqvista
 */

#ifndef ILI9341_H_
#define ILI9341_H_
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "UserLib/LibSPI.h"
#include "stdio_ext.h"
#include "Delay.h"
#include "UserLib/FONT.H"
#include <stdlib.h>
#include <stdio.h>
#include "UConvert.h"

#define DIS_A0 GPIO_Pin_9
#define DIS_RES GPIO_Pin_8

///////////////////////////
//  Define rotate display
//  Only choose one
///////////////////////////
//#define DISPLAY_ROTATE_NORMAL
#define DISPLAY_ROTATE_LEFT
//#define DISPLAY_ROTATE_RIGHT

class ILI9341 {
public:
	ILI9341();
	void Init();

	void LCD_Clean(uint16_t Color);
	void LCD_WriteRAM_Prepare();
	void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
	void LCD_Fill(uint16_t xsta, uint16_t xend, uint16_t ysta, uint16_t yend,
			uint16_t color);
	void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
	void LCD_ShowChar_4824(uint16_t x, uint16_t y, int8_t num, uint16_t color);
	void LCD_ShowChar_1608(uint16_t x, uint16_t y, int8_t word, uint16_t fontColor, uint16_t bkColor = 0x00);
	void LCD_ShowString_1608(uint16_t x, uint16_t y, const char *str, uint16_t fontColor, uint16_t bkColor = 0x00);
	void LCD_ShowString(uint16_t x, uint16_t y, uint16_t color, uint8_t *p);
	void LCD_ShowNum(uint16_t x, uint16_t y, uint16_t color,
			uint8_t length, const int32_t num);
	void LCD_ShowFloat(uint16_t x, uint16_t y, uint16_t color,
			uint8_t length, const float num);
	void LCD_ShowBattery(uint16_t x, uint16_t y, uint16_t color, uint16_t batLev);
	void LCD_ShowLightning(uint16_t x, uint16_t y, uint16_t color, bool flag);
	void LCD_ShowSenserLable(uint16_t x, uint16_t y, uint16_t length, uint16_t fgColor, uint16_t bkColor = 0x00);

private:
	LibSPI m_libSPI;
	Delay delay;
	UConvert uConvert;
	void LCD_GPIO_Init();
	void WriteCommand(uint8_t cmd);
	void WriteData(uint8_t data);
};

#endif /* ILI9341_H_ */
