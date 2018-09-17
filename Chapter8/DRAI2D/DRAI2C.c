//*********************************************************
// DRAI2C.c - Main App File for I2C Module 1 to BQ32000
//*********************************************************
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

void delay_ms(int n);
void I2C1_Init(void);
uint8_t I2C1_Write(int sAddr, uint8_t rAddr, uint8_t data);
void I2C1_Read(int sAddr, uint8_t rAddr);
static int I2C1_Wait_Done(void);

#define SLAVE_ADDR 0x68						// 1101 000.

int main(void)
{
	uint8_t error;
	
	SYSCTL_RCGCGPIO_R |= 0x2|0x20;		// enable clock to GPIOB & GPIOF
	error = SYSCTL_RCGCGPIO_R;
	
	GPIO_PORTB_DIR_R |= 0x0f;					// set PORTB 3-0 as output pins
	GPIO_PORTB_DEN_R |= 0x0f;					// set PORTB 3-0 as digital pins
	GPIO_PORTF_DIR_R |= 0x0f;					// set PORTF 3-0 as output pins
	GPIO_PORTF_DEN_R |= 0x0f;					// set PORTF 3-0 as digital pins
	
	I2C1_Init();
	error = I2C1_Write(SLAVE_ADDR, 0, 0);
	if (error) { GPIO_PORTB_DATA_R = 0x0f;}
	//I2C1_Read(SLAVE_ADDR, 0);
}

void I2C1_Init(void)
{
	SYSCTL_RCGCI2C_R |= 0x02;					// enable clock to I2C1
	SYSCTL_RCGCGPIO_R |= 0x01;				// enable clock to GPIOA
	GPIO_PORTA_AFSEL_R |= 0xC0;				// PA7 for SDA1, PA6 for SCL1
	GPIO_PORTA_PCTL_R |= 0x33000000;
	GPIO_PORTA_DEN_R |= 0xC0;					// PA7 & PA6 as digital pins
	GPIO_PORTA_ODR_R |= 0x80;					// PA7 as open drain
	I2C1_MCR_R = 0x10;								// I2CI works as master mode
	I2C1_MTPR_R = 0x7;								// SCL1 = 100 kHz at system clock = 16 MHz
}

static int I2C1_Wait_Done(void)
{
	while(I2C1_MCS_R & 1);						// wait until I2C1 master is not busy
	return I2C1_MCS_R & 0xe;					// return I2C1 error code
}

uint8_t I2C1_Write(int sAddr, uint8_t rAddr, uint8_t data)
{
	uint8_t error;
	
	I2C1_MSA_R = sAddr << 1;					// write: S:(sAddr+W)+ACK+rAddr+ACK+data+ACK+P
	I2C1_MDR_R = rAddr;
	I2C1_MCS_R = 0x3;									// S:(sAddr+W)+ACK+rAddr+ACK
	
	error = I2C1_Wait_Done();					// wait until wrtie is complete
	if (error) { return error; }
	I2C1_MDR_R = data;
	I2C1_MCS_R = 0x5;									// +data+ACK+P
	error = I2C1_Wait_Done();					// wait until write is complete
	while(I2C1_MCS_R & 0x40);					// wait until bus is not busy
	error = I2C1_MCS_R & 0xe;					// check if any error occured
	return error;
}

void I2C1_Read(int sAddr, uint8_t rAddr)
{
	uint8_t error, data;
	
	while(1)
	{
		I2C1_MSA_R = sAddr << 1;				// read: S:(sAddr+W)+ACK+rAddr+ACK+R+(sAddr+R)+ACK+data+NACK+P
		I2C1_MDR_R = rAddr;
		I2C1_MCS_R = 0x3;
		error = I2C1_Wait_Done();
		if (error) { GPIO_PORTF_DATA_R |= 0x0f; }
		data = I2C1_MDR_R;
		while(I2C1_MCS_R & 0x40);				// wait until bus is not busy
		error = I2C1_MCS_R & 0xe;
		if (error) { GPIO_PORTF_DATA_R |= 0x0f; }
		GPIO_PORTB_DATA_R = data;				// Display receieved data on LED
		delay_ms(500);
	}
}

void delay_ms(int time)
{
	int i, j;
	for (i = 0; i < time; i++)
		for (j = 0; j < 3180; j++) {}
}















