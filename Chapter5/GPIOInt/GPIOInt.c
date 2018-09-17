#include "GPIOInt.h"
#include "tm4c123gh6pm.h"

void Enable_IRQ(void)
{
	int R2;
	__asm { 
					MOV R2, 0x0
					MSR BASEPRI, R2
					CPSIE I
				}
}

int main(void)
{
	volatile uint32_t RCC;
	
	RCC = (SYSCTL_RCC_XTAL_16MHZ|SYSCTL_RCC_USESYSDIV|SYSCTL_SYSDIV_10);
	SYSCTL_RCC_R = RCC;
	SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOB|SYSCTL_RCGC2_GPIOD;
	GPIO_PORTB_DIR_R = 0x01;
	GPIO_PORTB_DEN_R = 0x01;
	GPIO_PORTD_DIR_R = ~0x8;																// set PD3 as input pin
	GPIO_PORTD_DEN_R = 0x08;																// enable PD3 as digital function
	GPIO_PORTD_IS_R = 0x8;																	// enable PD3 is level-triggered interrupt
	GPIO_PORTD_IBE_R = 0x0;																	// disable PD3 IBE function
	GPIO_PORTD_IEV_R = 0x8;																	// enable PD3 high level/Rising edge
	GPIO_PORTD_ICR_R = 0x8;																	// clear PD3 for any previous interrupt
	GPIO_PORTD_IM_R = 0x8;																	// enable (unmask) PD3 interrupt
	
	NVIC_PRI0_R = 0x60000000;					// NVIC->IP[3] = 0x60 priority 3
	NVIC_EN0_R = 0x8;									// NVIC-ISER[0] = 0x00000008; enable IRQ3
	Enable_IRQ();											// __enable_irq(); global enable IRQs
	
	while(1) { GPIO_PORTB_DATA_R = 0x0; } // wait for PD3 interrupt/set PB0 to LOW to turn off LED
}

void GPIOD_Handler(void)					// ISR for PD3 interrupt
{
	GPIO_PORTB_DATA_R = 0x1;				// set PB0 to high to turn on LED PB0
}
