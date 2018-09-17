//*********************************************************************************************
// DRAUSBHostDevice.c - Main Application File for USB as a Host and a Device - Both IN Mode
//*********************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "TM4C123GH6PM.h"

void USBDevice_Init(void);
void USBHost_Init(void);
uint32_t USBDevice_SendData(void);
uint32_t USBDevice_PutData(uint8_t *pui8Data, uint32_t ui32Size);
void USBHost_RecvData(uint8_t *pui8Data, uint32_t ui32Size);

int main(void)
{
	uint8_t ret, SndData[64], RevData[64];
	
	for (ret = 0; ret < 64; ret++)					// initialize sending data array
		SndData[ret] = ret;
	
	SYSCTL->RCGCUSB = 0x1;									// enable the USB module
	SYSCTL->RCGCGPIO = 0x20;								// enable GPIO Port F
	GPIOF->DIR = 0xf;												// set PF3~PF0 as output pins
	GPIOF->DEN = 0xf;												// set PF3~PF0 as digital functions pins
	
	USBDevice_Init();
	USBHost_Init();
	ret = USBDevice_PutData(SndData, 64);
	
	if(ret != 0) {return 1;}
	ret = USBDevice_SendData();
	if (ret != 0) {return 1;}
	USB0->DEVCTL = 0x1;
	USBHost_RecvData(RevData, 64);
	
	while(1);
}