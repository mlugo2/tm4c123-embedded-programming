#include <stdint.h>
#include <stdbool.h>

#define SYSCTL_RCGC2_R 			(*((volatile uint32_t*)0x400FE108))
#define GPIO_PORTB_DATA_R		(*((volatile uint32_t*)0x400053FC))
#define GPIO_PORTD_DATA_R		(*((volatile uint32_t*)0x400073FC))

#define SYSCTL_RCGC2_GPIOB	0x00000002	// Port B Clock Gating Control
#define SYSCTL_RCGC2_GPIOD	0x00000008	// Port D Clock Gating Control
