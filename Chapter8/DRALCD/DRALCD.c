//**************************************************************************
// DRALCD.c - Main Application file for LCD Project - The First Part Codes
//**************************************************************************
#include "DRALCD.H"

int main(void)
{
	LCD_Init();					// initialize LCD controller
	LCD_Comd(1);				// clear screen, move cursor to home
	
	// Write "WELCOME TO JCSU!" on LCD
	LCD_Data('H');LCD_Data('E');LCD_Data('L');LCD_Data('L');LCD_Data('O');
}

void LCD_Init(void)											// Initialize SSI2 then initialize LCD controller
{
	SYSCTL->RCGCSSI |= 0x04;							// enable clock to SSI2
	SYSCTL->RCGCGPIO |= 0x02|0x04;				// enable clock to Port B and Port C
	
	// PB7 & PB4 for SSI2Tx and SSI2Clk
	GPIOB->AMSEL &= ~0x90;								// turn off analog of PB7 & PB4
	GPIOB->AFSEL |= 0x90;									// set PB7 & PB4 for alternative functions
	GPIOB->PCTL &= ~0xf00f0000;						// clear functions for PB7 & PB4
	GPIOB->PCTL |= 0x20020000;						// PB7 & PB4 for SSI2 function
	GPIOB->DEN |= 0x90;										// PB7 & PB4 as digitial function pins
	
	// PC6 for SSI2 slave select and CS_LCD_(STCP) clock
	GPIOC->AMSEL &= ~0x40;								// disable PC6 analog function
	GPIOC->DIR |= 0x40;										// set PC6 as output for CS_LCD signal
	GPIOC->DEN |= 0x40;										// set PC6 as digital pins
	
	SSI2->CR1 = 0;												// make SSI2 as master and disable SSI2
	SSI2->CC = 0;													// use system clock (16MHz)
	SSI2->CPSR = 16;											// clock prescalar divide by 16 gets 1 MHz SSI2Clk clock
	SSI2->CR0 = 0x0007;										// clock rate div by 1, phase/polarity 0/0, freecale, data size 8
	SSI2->CR1 = 2;												// enable SSI2
	
	delay_ms(20);													// LCD controller reset sequence
	LCD_cd_Write(0x30,0);									// send reset code 1 two times to SPLC780
	delay_ms(5);
	LCD_cd_Write(0x30,0);
	delay_ms(1);
	LCD_cd_Write(0x30,0);									// send reset code 2 to SPLC780
	delay_ms(1);
	
	LCD_cd_Write(0x20,0);									// use 4-bit data mode
	delay_ms(1);
	LCD_Comd(0x28);												// set 4-bit data, 2-line, 5x7 font
	LCD_Comd(0x06);												// move cursor right
	LCD_Comd(0x0C);												// turn on display, cursor off - no blinking
	LCD_Comd(0x01);												// clear screen, move cursor to home
}

void SSI2_Write(unsigned char data)
{
	GPIOC->DATA &= ~0x40;									// clear STCP (CS_LCD) in 74VHCT595 to Low (PC6)
	SSI2->DR = data;											// write serial data into 74VHCT595
	while(SSI2->SR & 0x10);								// wait for 74VHCT595 serial data shift done
	GPIOC->DATA |= 0x40;									// set CS_LCD(STCP) to High to simulate a positive-going-edge
}

void LCD_cd_Write(char data, unsigned char control)
{
	data &= 0xf0;													// clear lower nibble for data
	control &= 0x0f;											// clear uppder nibble for control
	SSI2_Write(data|control|BL);					// RS = 0, R/W = 0
	SSI2_Write(data|control|EN|BL);				// pulse E
	delay_ms(0);
	SSI2_Write(data|BL);
	SSI2_Write(BL);
}

void LCD_Comd(unsigned char cmd)
{
	LCD_cd_Write(cmd & 0xF0, 0);					// upper nibble first
	LCD_cd_Write(cmd << 4, 0);						// then lower nibble
	if (cmd < 4)
		delay_ms(2);												// command 1 and 2 needs up to 1.64ms
	else
		delay_ms(1);												// all others 40 us
}

void LCD_Data(char data)
{
	LCD_cd_Write(data & 0xf0, RS);				// upper nibble first
	LCD_cd_Write(data << 4, RS);					// then lower nibble
	delay_us(40);
}

void delay_ms(int time)
{
	int i, j;
	for(i = 0; i < time; i++)
		for (j = 0; j < 3180; j++) {}				// do nothing 1ms
}

void delay_us(int time)
{
	int i, j;
	for(i=0; i<time; i++)
		for(j=0;j<3;j++) {}									// do nothing 1 us
}
