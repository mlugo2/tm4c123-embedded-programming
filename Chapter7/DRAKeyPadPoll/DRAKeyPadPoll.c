//*******************************************
// DRAKeyPadPoll.c - Main Application File
//*******************************************
#include <stdint.h>
#include <stdbool.h>
#include "TM4C123GH6PM.h"

void InitKeypad(void);
char ReadKeypad(void);

int main(void)
{
	char keyCode, keyNum;
		
	InitKeypad();									// initialize ports A, B, & D
	while(1)
	{
		keyCode = ReadKeypad();			// try to read a key from keypad
		if (keyCode != 0)						// if a key is pressed...
		{
			keyNum = keyCode;					// reserve the key code
			keyCode = ReadKeypad();		// try to get the key again
			if (keyCode == keyNum)		// is the same key?
			{ GPIOB->DATA = keyNum;}	// display key code on portB
			else 
			{ GPIOB->DATA = 0; }
		}
	}
}

void InitKeypad(void)
{
	SYSCTL->RCGC2 |= 0x1|0x2|0x8;		// enable GPIO Ports A, B, & D
	GPIOB->DIR |= 0x0F;							// set PORTB 3-0 as output pins
	GPIOB->DEN |= 0x0F;							// set PORTB 3-0 as digital pins
	GPIOA->DIR |= ~0x3C;						// set PORTA 5-2 as input pins(rows 0-3)
	GPIOA->DEN |= 0x3C;							// set PORTA 5-2 as digital pins
	GPIOD->DIR |= 0x0F;							// set PORTD 3-0 as output pins(cols 0-3)
	GPIOD->DEN |= 0x0F;							// set PORTD 3-0 as digital pins
}

char ReadKeypad(void)
{
	int row = 0, col, rowNum = 4;
	const char row_select[] = { 0x01, 0x02, 0x04, 0x08};
	
	while( row < rowNum )
	{
		GPIOD->DIR &= ~0x0F;									// disable all rows
		GPIOD->DIR |= row_select[row];				// enable one row
		GPIOD->DATA |= 0x0F;									// set the active row high
		col = GPIOA->DATA & 0x3C;							// read all columns
		if (col != 0) break;									// if one of the input is high, some key is pressed.
		row++;
	}
	GPIOD->DIR &= ~0x0F;										// disable all rows
	if (col == 0) {return col;}							// if now keys pressed, return 0
	// get here when one of the rows has key pressed.
	if ( col == 0x04 ) return (1);		// key in column 0
	if ( col == 0x08 ) return (1);		// key in column 1
	if ( col == 0x10 ) return (1);		// key in column 2
	if ( col == 0x20 ) return (1);		// key in column 3
	
	return 0;
}
