/***************************************************
 *	MyProject.h
 *	Header file for the project MyProject.c
 ***************************************************/
 
 /***************************************************
 *	System Control registers (SYSCTL)
 ***************************************************/
#define SYSCTL_RCGC2_R				(*((volatile uint32_t*)0x400FE108))
#define SYSCTL_RCGC2_GPIOF		0x00000020	// Port F Clock Gating Control
 
 /***************************************************
 *	GPIO registers (PORTF)
 ***************************************************/
#define GPIO_PORTF_DIR_R			(*((volatile uint32_t*)0x40025400))
#define GPIO_PORTF_DEN_R			(*((volatile uint32_t*)0x4002551C))
#define GPIO_PORTF_DATA_R		(*((volatile uint32_t*)0x400253FC))
