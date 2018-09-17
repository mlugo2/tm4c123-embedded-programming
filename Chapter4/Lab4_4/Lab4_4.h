#include <stdint.h>
#include <stdbool.h>


#define SYSCTL_RCGC2_R 					(*((volatile uint32_t*)0x400FE108))
#define GPIO_PORTB_DATA_R				(*((volatile uint32_t*)0x400053FC))
#define GPIO_PORTB_DIR_R				(*((volatile uint32_t*)0x40005400))
#define GPIO_PORTB_DEN_R				(*((volatile uint32_t*)0x4000551C))
	
#define GPIO_PORTC_DATA_R				(*((volatile uint32_t*)0x400063FC))
#define GPIO_PORTC_DIR_R				(*((volatile uint32_t*)0x40006400))
#define GPIO_PORTC_DEN_R				(*((volatile uint32_t*)0x4000651C))

#define GPIO_PORTD_DATA_R				(*((volatile uint32_t*)0x400073FC))
#define GPIO_PORTD_DIR_R				(*((volatile uint32_t*)0x40007400))
#define GPIO_PORTD_DEN_R				(*((volatile uint32_t*)0x4000751C))
	
#define SYSCTL_RCC_R						(*((volatile uint32_t*)0x400FE060))
#define SYSCTL_RCC_XTAL_16MHZ		0x00000540	// use 16MHz crystal
#define SYSCTL_RCC_USESYSDIV		0x00400000	// enable System Clock Divider
#define SYSCTL_SYSDIV_10				0x04C00000	// system clock is osc-pll/10
#define SYSCTL_RCGC2_GPIOB			0x00000002	// Port B Clock Gating Control
#define SYSCTL_RCGC2_GPIOC			0x00000004	// Port C Clock Gating Control
#define SYSCTL_RCGC2_GPIOD			0x00000008	// Port D Clock Gating Control

