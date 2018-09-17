//*****************************************************
// DRAPWM.c - Main Application file for PWM - DC Motor
//*****************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc\tm4c123gh6pm.h"
#include "driverlib\sysctl.h"

void Delay(uint32_t time);

int main(void)
{
	uint32_t pw, RCC;
	
	RCC = (SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	SYSCTL_RCC_R = RCC;																				// 40 MHz system clock source
	
	SYSCTL_RCGCPWM_R |= 0x2;																	// enable PWM1 with clock
	while((SYSCTL_PRPWM_R & 0x2)==0) {};											// wait for PWM1 ready
	SYSCTL_RCGCGPIO_R = 0x20 | 0x2;														// enable Ports F & B ready
	while((SYSCTL_PRGPIO_R & 0x22)==0){};											// wait GPIO Ports F & B ready
	SYSCTL_RCC_R = SYSCTL_RCC_USEPWMDIV|SYSCTL_RCC_PWMDIV_2;	// 20 MHz clock
	SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM0;											// enable PWM1 again
	
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;												// unlock GPTOF commit register
	GPIO_PORTF_CR_R |= 0x02;																	// enable PF1 to be committed
	GPIO_PORTF_LOCK_R = 0x0;																	// lock GPIOF commit register
		
	// PWM1_2B - PF1 - M1PWM5 pin - Module 1 Generator 2 - pwm2B
	PWM1_2_CTL_R = 0x0;																				// disable PWM1_2B or M1PWM5
	PWM1_2_GENB_R = 0x0000080C;																// high on LOAD, low on CMPB down
	PWM1_2_LOAD_R = 3999;																			// load = 4000 - 1
	PWM1_2_CTL_R = 0x1;																				// enable PWM1_2B or M1PWM5
	PWM1_ENABLE_R = 0x20;																			// enable PWM1
	
	GPIO_PORTF_DIR_R |= 0x02;																	// enable PF1 output pin
	GPIO_PORTF_DEN_R |= 0x02;																	// enable PF1 digital pin
	GPIO_PORTF_AFSEL_R |= 0x2;																// PF1 - Alternate Function: PWM1-2B
	GPIO_PORTF_PCTL_R |= GPIO_PCTL_PF1_M1PWM5;								// 0x00000050;
	GPIO_PORTF_AMSEL_R &= ~0x02;															// disable analog function PF1
	GPIO_PORTB_DIR_R |= 0x03;																	// enable PB1, PB0 output pins
	GPIO_PORTB_DEN_R |= 0x03;																	// enable PB1, PB0 digital functions
	GPIO_PORTB_DATA_R |= 0x01;																// enable PB0 (AIN1=1)CW-rotation
	
	while(1)
	{
		for (pw = 100; pw < 3999; pw+=20)												// increase duty cycle
		{
			PWM1_2_CMPB_R = pw; Delay(50000);											// delay 15 ms
		}
		for (pw = 3999; pw > 100; pw -=20)											// decrease duty cycle
		{
			PWM1_2_CMPB_R = pw;	Delay(50000);											// Delay 15 ms
		}
	}
}

void Delay(uint32_t time)
{
	uint32_t Loop;
	for (Loop = 0; Loop < time; Loop++) {}
}
