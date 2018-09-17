#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"

void GPIOD_Handler(void);

int main(void) 
{
	SysCtlClockSet(SYSCTL_SYSDIV_10|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_0);
	
	GPIOIntRegister(GPIO_PORTD_BASE, GPIOD_Handler);
	IntPrioritySet(INT_GPIOD, 0x60);
	GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_0,GPIO_FALLING_EDGE);
	GPIOIntEnable(GPIO_PORTD_BASE,GPIO_INT_PIN_0);
	IntMasterEnable();
	
	while(1)
	{
		__wfi();
	}
}

void GPIOD_Handler(void)
{
	GPIOIntClear(GPIO_PORTD_BASE, GPIO_INT_PIN_0);
	if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1))
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, 0);
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0);
	}
	else
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1, 0xE);
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, 0xF);
	}
}
