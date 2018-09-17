/************************************************************************
 *	DRAUART.c - Main Application File for UART Module 1 Self-Loop Test
 ***********************************************************************/
 #include <stdint.h>
 #include <stdbool.h>
 #include "TM4C123GH6PM.h"
 
 // Function prototypes
 void UART_Init(void);
 void UART_Transmit(char s_data);
 char UART_Receive(void);
 
 int main(void)
 {
	 uint8_t num = 0;
	 char s_data, chr[26];
	 
	 UART_Init();
	 for(s_data='A'; s_data<='Z'; s_data = s_data+1)
	 {
		 UART_Transmit(s_data);						// transmit a byte of data
		 chr[num] = UART_Receive();				// receive a byte of data
		 num++;
	 }
	 
	 while(1);	 
 }
 
 void UART_Init()
 {
	 SYSCTL->RCGCGPIO |= 0x02|0x20;			// enable clock to GPIOB & GPIOF
	 SYSCTL->RCGCUART = 0x02;						// enable clock to UART1
	 
	 GPIOB->AFSEL = 0x03;								// PB1 & PB0 for U1TX & U1RX
	 GPIOB->PCTL = 0x00000011;
	 GPIOB->DEN = 0x03;									// PB1 & PB0 as digital pins
	 GPIOF->DEN = 0xF;									// PF3 ~ PF0 as digital and output pins
	 GPIOF->DIR = 0xF;
	 
	 UART1->CTL &= 0xFFFFFFFE;					// diable UART1
	 UART1->IBRD = 0x8;									// set the integer baud rate period
	 UART1->FBRD = 0x2C;								// set fractional baud rate period
	 UART1->LCRH = 0x60;								// set baud rate = 115200, 8-bit data, 1 stop, no parity
	 UART1->CC = 0x0;										// set UARTCC as 00 to select System Clock as source
	 UART1->CTL |= 0x1;									// enable UART1
 }
 
 void UART_Transmit(char s_data)
 {
	 while ((UART1->FR & 0x20) != 0);
	 UART1->DR = s_data;
 }
 
 char UART_Receive(void)
 {
	 uint32_t ret;
	 char rdata;
	 
	 while ((UART1->FR & 0x10) != 0);
	 ret = UART1->DR;
	 if (ret & 0xF00) { GPIOF->DATA = 0xF; }		// error occurred..
	 else { rdata = (char)(UART1->DR & 0xFF); }
	 return rdata;
 }
