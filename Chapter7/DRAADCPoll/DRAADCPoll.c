#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

void Delay(uint32_t time);

int main(void)
{
	int pSensor, pMeter, pTemp;
	
	// Initialize ADC0 related GPIO Ports B, C, E, & F and ADC0 clock
	SYSCTL_RCGC2_R = 
	SYSCTL_RCGC2_GPIOE|SYSCTL_RCGC2_GPIOB|SYSCTL_RCGC2_GPIOF|SYSCTL_RCGC2_GPIOC;
	SYSCTL_RCGCADC_R |= 0x1;											// enable clock to drive ADC module
	GPIO_PORTE_AFSEL_R |= 0x3F;										// enable PE0 - PE5 work as alternate function
	GPIO_PORTE_DIR_R |= ~0x3F;										// define PE0 - PE5 as input pins
	GPIO_PORTE_AMSEL_R |= 0x3F;										// disable analog isolation circuit for PE0-PE5
	
	GPIO_PORTB_DEN_R |= 0xF;											// enable PB3-PB0 as digital function pins
	GPIO_PORTB_DIR_R |= 0xF;											// configure PB3-PB0 as output pins
	
	GPIO_PORTC_DEN_R |= 0x10;											// set PC4 as output pin
	GPIO_PORTC_DIR_R |= 0x10;											// enable PC4 as digital pin
	
	GPIO_PORTF_DEN_R |= 0xF;											// enable PF3-PF0 as digital function pins
	GPIO_PORTF_DIR_R |= 0xF;											// configure PF3-PF0 as output pins
	
	// Initialize ADC module 0 - ADC0
	GPIO_PORTE_ADCCTL_R = 0x0;										// Use default trigger source - processor
	
	// Initialize ADC0 Sample Sequencer 1 - SS1
	ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN1;							// diable ADC0 SS1
	ADC0_EMUX_R = ADC_EMUX_EM1_PROCESSOR;					// select the default trigger source
	ADC0_SSMUX1_R |= 0x0821;											// 1st sample AIN1, 2nd sample AIN2, 3rd sample AIN8
	ADC0_SSCTL1_R |= 0x0600;											// end at 3rd sample with interrupt
	ADC0_ACTSS_R |= ADC_ACTSS_ASEN1;							// enable ADC0 SS1
	
	while(1)
	{
		ADC0_PSSI_R = ADC_PSSI_SS1;									// Initiate SS1
		while ((ADC0_RIS_R & 2)==0);								// Polling to wait for ADC convertion done
		pMeter = ADC0_SSFIFO1_R;										// read the 1st sample from AIN1
		pSensor = ADC0_SSFIFO1_R;										// read the 2nd sample from AIN2
		pTemp = ADC0_SSFIFO1_R;											// read the 3rd sample from AIN8
		ADC0_ISC_R = ADC_ISC_IN1;										// clear the interrupt in RIS
		GPIO_PORTF_DATA_R = pMeter >> 8;						// send 4 MSB in the 1st sample to LEDs PF3~PF1
		GPIO_PORTB_DATA_R = pSensor >> 8;						// send 4 MSB in the 2nd sample to LEDs PB3~PB0
		GPIO_PORTC_DATA_R = pTemp << 2;							// send 3rd sample*4 to speaker
		Delay(50000);
	}
}

void Delay(uint32_t time)
{
	uint32_t Loop;
	for (Loop=0; Loop<time; Loop++) {}
}
