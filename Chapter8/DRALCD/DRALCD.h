//*******************************************************
// DRALCD.h - Header Files for the LCD Projects - DRALCD
//*******************************************************
#include <stdint.h>
#include <stdbool.h>
#include "TM4C123GH6PM.h"

#define RS	1							// 74VHCT595 Q0 bit for RS (Reg Select)
#define EN	2							// 74VHCT595 Q1 bit for E (Enable LCD)
#define BL	4							// 74VHCT595 Q2 bit for BL (Backlight)

void delay_ms(int time);
void delay_us(int time);
void LCD_cd_Write(char data, unsigned char control);
void LCD_Comd(unsigned char cmd);
void LCD_Data(char data);
void LCD_Init(void);
void SSI2_Write(unsigned char data);

