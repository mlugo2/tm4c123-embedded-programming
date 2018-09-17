// CMSISInt.c
#include <stdint.h>
#include <stdbool.h>
#include "TM4C123GH6PM.h"

int main(void)
{
	SYSCTL->RCGCGPIO |= 0x08;							// enable clock to PORTD
	SYSCTL->RCGCGPIO |= 0x02;							// enable clock to PORTB
	SYSCTL->RCGCGPIO |= 0x20;							// enable clock to PORTF
	
	GPIOB->DIR |= 0x0F;										// set PB0~PB3 as output pins
	GPIOB->DEN |= 0x0F;										// enable PB0~PB3 as digital functions
	GPIOF->DIR |= 0xE;										// set PF3~PF1 as output pins
	GPIOF->DEN |= 0xF;										// enable PF3~PF0 as digital functions
	GPIOD->DIR |= 0x0;										// set PD0 as an input pin
	GPIOD->DEN |= 0x1;										// enable PD0 as digital function
	
	GPIOD->IS &= ~0x1;										// make bit PD0 as an edge sensitive
	GPIOD->IBE &= ~0x1;										// trigger is controlled by IEV
	GPIOD->IEV |= 0x1;										// rising edge trigger
	GPIOD->ICR |= 0x1;										// clear any prior interrupt
	GPIOD->IM	|= 0x1;											// unmask interrupt
	
	NVIC_SetPriority(GPIOD_IRQn, 0x60);		// set PORTD priority level as 3
	NVIC_EnableIRQ(GPIOD_IRQn);						// enable PORTD to generate interrupt
	
	__enable_irq();												// globally enable interrupt
	
	while(1)															// wait for interrupt
	{
	}
}

void GPIOD_Handler(void)									// PD0 interrupt handler
{
	if((NVIC_GetActive(GPIOD_IRQn)) == 1)	// check whether PD0 interrupt flag is set
	{
		GPIOD->ICR |=0x01;									// if it is, clear any interrupt flag
	}
	GPIOB->DATA ^=15;											// toggle 4 LEDs PB0~PB3 in Trainer	
	GPIOF->DATA ^=14;											// toggle 3-color LED in TM
}
