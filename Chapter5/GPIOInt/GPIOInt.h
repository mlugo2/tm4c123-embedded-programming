// GPIOInt.h	- Header file for the sample project GPIOInt.c
#include <stdint.h>

#define SYSCTL_RCC_XTAL_16MHZ				0x00000540	// use 16 MHz crystal
#define SYSCTL_RCC_USESYSDIV				0x00400000	// enable System Clock Divider
#define SYSCTL_SYSDIV_10						0x04C00000	// system clock osc-pll/10
#define SYSCTL_RCGC2_GPIOB					0x00000002	// Port B Clock Gating Control
#define SYSCTL_RCGC2_GPIOD					0x00000008	// Port D Clock Gating Control
