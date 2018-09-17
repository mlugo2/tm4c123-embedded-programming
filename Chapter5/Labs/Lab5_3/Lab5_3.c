#include <stdint.h>
#include <stdbool.h>
#include "TM4C123GH6PM.h"

// Function prototypes
void Ports_Init(void);
void Delay(int time);
void SetSound(int period);

int main(void)
{
	// Setup clock and enable GPIO ports B, C, D, and F
	SYSCTL->RCGCGPIO = 0x20|0x08|0x04|0x02;
	
	// Initialize ports
	Ports_Init();
	
	// Configure PD0, hi-level-triggered interrupt pin
	int pd = 0x1;
	GPIOD->IS =  pd;
	GPIOD->IBE = pd;
	GPIOD->IEV = pd;
	GPIOD->ICR = pd;
	GPIOD->IM =  pd;
	
	// Configure PF4, lo-level-triggered interrupt pin
	int pf = 0x10;
	GPIOF->IS =  pf;
	GPIOF->IBE = pf;
	GPIOF->IEV = 0x0;
	GPIOF->ICR = pf;
	GPIOF->IM =  pf;
	
	// Set interrupt priority level of PD0 pin
	NVIC->IP[3] = 0x60;							// set interrupt priority to 3
	NVIC->ISER[0] |= 0x00000008;		// enable IRQ3
	
	// Set interrupt priority level of PF4 pin
	NVIC->IP[30] = 0xA0;						// set interrupt priority to 5
	NVIC->ISER[0] |= 0x40000000;		// enable IRQ30
	
	__enable_irq();
	
	while(1)
	{
		GPIOB->DATA = 0x0;
	}
}

void GPIOD_Handler(void)
{
	GPIOB->DATA = 0x6;			// turn on LED PB1 & PB2 in Trainer
	SetSound(25000);
}

void GPIOF_Handler(void)
{
	GPIOB->DATA = 0x9;			// turn on LED PB0 & PB3 in Trainer
	SetSound(12500);
}

void Ports_Init(void)
{
	// Set GPIO Port B as output pins & digital function
	GPIOB->DIR = 0x0F;
	GPIOB->DEN = 0xF;
	
	// PC4 as output pin, enable digital function
	GPIOC->DIR = 0x10;
	GPIOC->DEN = 0x10;
	
	// PD0 as input pin, enable digital function
	GPIOD->DIR = 0x0;
	GPIOD->DEN = 0x1;
	
	// PF4 as input pin, enable digital function
	GPIOF->DIR = 0x00;
	GPIOF->DEN = 0x10;
	
	// PF4 pull-up mode
	GPIOF->PUR |= 0x10;
	
}

void Delay(int time)
{
	int i;
	for(i=0; i < time; i++) {}
}

void SetSound(int period)
{
	GPIOC->DATA = 0x10;			// send high half-cycle
	Delay(period);
	GPIOC->DATA = 0x0;			// send low half-cycle
	Delay(period);
}
