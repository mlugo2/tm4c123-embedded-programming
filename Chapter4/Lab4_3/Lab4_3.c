#include <stdint.h>
#include <stdbool.h>
#include "Lab4_3.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

int main(void) {
	
	uint32_t ui32Input;
	bool res;
	
	// Set up the clock
	SysCtlClockSet(SYSCTL_SYSDIV_10|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	
	// Use DRA to setup RCGC2
	SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOB|SYSCTL_RCGC2_GPIOC|SYSCTL_RCGC2_GPIOD;
	
	// Set PB0 as an output pin
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0);
	
	// Set PC4 as an output pin
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
	
	// Set PD3 as an input pin
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3);
	
	// Check if ports are ready to be accessed
	res = SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)|SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)|SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB);
	
	if(res) {
		while(1) {
			ui32Input = GPIO_PORTD_DATA_R;
			
			if((ui32Input & 0x8)== 0x8) {
				GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x1);
				GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x10);
				SysCtlDelay(500000);
				GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x0);
				GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x0);
				SysCtlDelay(500000);
			}
		}
	}
}
