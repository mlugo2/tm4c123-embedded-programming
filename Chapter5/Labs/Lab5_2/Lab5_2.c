#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

// Function prototypes
void Delay(int time);
void SetSound(int period);

int main(void)
{
	// Enable GPIO Ports B, C, and D
	SYSCTL->RCGCGPIO = 0x08|0x04|0x02;
	
	// Set PB0~PB1 as output
	GPIOB->DIR = 0x3;
	
	// Enable PB0~PB1 as digital function
	GPIOB->DEN = 0x3;
	
	// Set PC4 as output
	GPIOC->DIR = 0x10;
	
	// Enable PC4 as digital function
	GPIOC->DEN = 0x10;
	
	// Set PD2 as input
	GPIOD->DIR = 0x0;
	
	// Enable PD2 and PD0 as digital function
	GPIOD->DEN = 0x5;
	
	GPIOD->IS = 0x5;
	GPIOD->IBE = 0x5;
	GPIOD->IEV = 0x5;
	GPIOD->ICR = 0x5;
	GPIOD->IM = 0x5;
	
	// Set priority level 3
	NVIC->IP[3] = 0x6;
	NVIC->ISER[0] = 0x8;	
	
	// globally enable all interrupts
	__enable_irq();
	
	while(1)
	{
		GPIOB->DATA = 0x0;
	}
}

void GPIOD_Handler(void)
{
	
	if (GPIOD->DATA == 0x4)
	{
		GPIOB->DATA = 0x1;
		SetSound(25000);
	}
	else if (GPIOD->DATA == 0x1)
	{
		GPIOB->DATA = 0x2;
		SetSound(50000);
	}
	
}

void Delay(int time)
{
	int i;
	for (i = 0; i <= time; i++)
	{
	}
}

void SetSound(int period)
{
	GPIOC->DATA = 0x10;
	Delay(period);
	GPIOC->DATA = 0x0;
	Delay(period);
}
