#define SYSCTL_RCGC2_R			(*((volatile uint32_t*)0x400FE108))
#define GPIO_PORTD_DATA_R		(*((volatile uint32_t*)0x400073FC))
#define GPIO_PORTC_DATA_R		(*((volatile uint32_t*)0x400063FC))
#define GPIO_PORTB_DATA_R		(*((volatile uint32_t*)0x500053FC))

#define SYSCTL_RCGC2_GPIOB	0x00000002	// Port B Clock Gating Control
#define SYSCTL_RCGC2_GPIOC	0x00000004	// Port B Clock Gating Control
#define SYSCTL_RCGC2_GPIOD	0x00000008	// Port B Clock Gating Control
