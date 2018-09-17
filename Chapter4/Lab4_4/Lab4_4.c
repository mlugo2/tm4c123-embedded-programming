#include "Lab4_4.h"
#include "tm4c123gh6pm.h"

// Function prototypes
void Delay(int);
void SetSound(int);

int main(void) {
	uint32_t ui32Input, RCC;
	
	// Setup the clock
	RCC = (SYSCTL_RCC_XTAL_16MHZ|SYSCTL_RCC_USESYSDIV|SYSCTL_SYSDIV_10);
	
	// Assign the clock setup to the RCC register
	SYSCTL_RCC_R = RCC;
	
	// Enable ports B, C, and D
	SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOB|SYSCTL_RCGC2_GPIOC|SYSCTL_RCGC2_GPIOD;
	
	// Set PB0-PB3 as output pins
	GPIO_PORTB_DIR_R |= 0x0F;
	
	// Set PB0-PB3 as digital function pins
	GPIO_PORTB_DEN_R |= 0x0F;
	
	// Set PD0-PD3 as input pins
	GPIO_PORTC_DIR_R |= 0xF0;
	
	// Set PD0-PD3 as digital function pins
	GPIO_PORTD_DEN_R |= 0x0F;
	
	// Set PC4 as an output pin
	GPIO_PORTC_DIR_R |= 0x10;
	
	// Enable PC4 as a digital function
	GPIO_PORTC_DEN_R |= 0x10;
	
	while(1) {
		
		// Get input
		ui32Input = GPIO_PORTD_DATA_R;
		
		// Assign values of input to LEDs
		GPIO_PORTB_DATA_R = ui32Input;
		
		// DIP-SW1 pressed 400Hz sent to speaker
		if (ui32Input == 0x8)
			SetSound(25000);
		// DIP-SW1 & SW2 pressed 600Hz sent to speaker
		else if (ui32Input == 0x0C) 
			SetSound(17000);
		// DIP-SW1, SW2, and SW3 pressed 800Hz
		else if (ui32Input == 0x0E)
			SetSound(12500);
	}
}

void SetSound(int period) {
	GPIO_PORTC_DATA_R = 0x10;
	Delay(period);
	GPIO_PORTC_DATA_R = 0x0;
	Delay(period);
}

void Delay(int time) {
	int i;
	for (i = 0; i < time; i++) {
	}
}
