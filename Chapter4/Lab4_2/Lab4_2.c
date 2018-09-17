#include "Lab4_2.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

int main(void) {
	uint32_t ui32Input;
	bool res;
	
	// Set up the clock
	SysCtlClockSet(SYSCTL_SYSDIV_10|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	
	// Enable the GPIO PROTB & PORTD
	SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOB|SYSCTL_RCGC2_GPIOD;
	
	// Set PB0-PB3 as an output pins
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	
	// Set PD0-PD3 as an input pin
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3);
	
	// Check whether both ports are ready to be accessed
	res = SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB) | SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD);
	
	if (res) {
		while(1) {
			ui32Input = GPIO_PORTD_DATA_R;
			
			// If DIP-SW1 is pressed turn on LED
			if ((ui32Input & 0x8) == 0x8) {
				GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x3);
				SysCtlDelay(500000);
				GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x0);
				SysCtlDelay(500000);
			}
		}
	}
}
