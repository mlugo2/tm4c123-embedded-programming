#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

#define SYSCTL_RCC_XTAL_16MHZ				0x00000540	// use 16 MHz crystal
#define SYSCTL_RCC_USESYSDIV				0x00400000	// enable System Clock Divider
#define SYSCTL_SYSDIV_10						0x04C00000	// system clock osc-pll/10
#define SYSCTL_RCGC2_GPIOD					0x00000008	// Port D Clock Gating Control
#define SYSCTL_RCGC2_GPIOF					0x00000020	// Port F Clock Gating Control

int main(void)
{
	// Enable and setup clock source for GPIO ports D and F
	SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOD|SYSCTL_RCGC2_GPIOF;
	
	// Setup pins PF3~PF1 on the GPIO Port F as output
	GPIO_PORTF_DIR_R = 0xE;
	
	// Enable PF3~PF1 as digital function pins
	GPIO_PORTF_DEN_R = 0xF;
	
	// Setup PD1 as input pin
	GPIO_PORTD_DIR_R = 0x0;
	
	// Enable PD1 as digital function pin
	GPIO_PORTD_DEN_R = 0xF;
	
	// HIGH-Level triggered inturrupt
	GPIO_PORTD_IS_R = 0x2;
	GPIO_PORTD_IBE_R = 0x0;
	GPIO_PORTD_IEV_R = 0x2;
	GPIO_PORTD_ICR_R = 0x2;
	GPIO_PORTD_IM_R = 0x2;
	
	// Set GPIO Port D priority level as 3
	NVIC_PRI0_R = 0x60000000;
	
	// Enable GPIO Port D to be triggered by interrupt
	NVIC_EN0_R = 0x8;
	
	// Globally enabale all interrupts
	__enable_irq();
	
	while(1)		// wait for interrupt
	{
		GPIO_PORTF_DATA_R = 0x0;
	}
}

void GPIOD_Handler(void)
{
	GPIO_PORTF_DATA_R = 0xE;
}
