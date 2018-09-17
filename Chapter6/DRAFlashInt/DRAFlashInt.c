// DRAFlashInt.c - Main Application File
#include "DRAFlashInt.h"

int main(void)
{
	uint32_t ret, uiCount, uiAddress, *proData;
	uint32_t Data[3] = { 0x78563412, 0x8B674523, 0xA3456789 };
	uiAddress = 0x2000; uiCount = 3;
	proData = Data;
	
	SYSCTL_RCGCGPIO_R = 0x02;														// enable clock to Port B
	GPIO_PORTB_DIR_R |= 0xF;														// set PB0~PB3 as output pins
	GPIO_PORTB_DEN_R |= 0xF;														// enable PB0~PB3 as digital function
	
	FLASH_FCMISC_R = FLASH_FCMISC_PMISC;								// reset PMISC & PRIS bit in FCRIS to clear interrupt
	FLASH_FCIM_R = FLASH_FCIM_PMASK;										// unmask bit[1] (PMASK) in the FCIM register
	IntRegister(INT_FLASH, FLASHM_Handler);							// register the interrupt handler
	IntEnable(INT_FLASH);																// enable the flash interrupt
	
	ret = DRAFlashErase(uiAddress);
	if (ret != 0) {return 1;}														// if erase operation is wrong, return 1
	ret = DRAFlashWrite(proData, uiAddress, uiCount);
	if (ret != 0) {return 1;}														// if program operation is wrong, return 1
	return 0;
}

uint32_t DRAFlashErase(uint32_t uiAddr)
{
	
	eraseFlag++;
	// erase flash memory 1KB block starting at 0x2000
	FLASH_FCMISC_R = FLASH_FCMISC_AMISC;								// clear any flash access inturrupt
	FLASH_FMA_R = uiAddr;																// assign the starting address
	FLASH_FMC_R = FLASH_FMC_WRKEY|FLASH_FMC_ERASE;			// perform the erase operations..
	while (eraseFlag)			// wait for erase-complete inturrupt...
	{
	}
	return 0;
}


uint32_t DRAFlashWrite(uint32_t *pData, uint32_t uiAddr, uint32_t uiCount)
{
	FLASH_FCMISC_R = FLASH_FCMISC_AMISC;								// clear any flash access interrupt
	
	while (uiCount)
	{
		writeFlag++;
		FLASH_FMA_R = uiAddr;															// assign the starting address
		FLASH_FMD_R = *pData;
		FLASH_FMC_R = FLASH_FMC_WRKEY|FLASH_FMC_WRITE;		// perform the write operation
		
		while (writeFlag)		// Wait for the write-operation inturrupt
		{
		}
		pData++;
		uiAddr += 4;
		uiCount--;
	}
	if (FLASH_FCRIS_R & FLASH_FCRIS_ARIS)
	{
		return 1;
	}
	return 0;
}

void FLASHM_Handler(void)															// flash memory interrupt handler
{
	intCount++;
	if (FLASH_FCMISC_R & FLASH_FCMISC_PMISC)						// an erase/write complete interrupt occurred
	{
		switch(intCount)
		{
			case 1:																					// an erase-complete interrupt occurred
				GPIO_PORTB_DATA_R = 0x1;											// set PB0 to HI to indicate erasing done
				eraseFlag--;																	// stop while(eraseFlag) loop in DRAFlashErase()
				break;
			
			case 2:																					// frist-word write-complete interrupt occured
				GPIO_PORTB_DATA_R = 0x3;											// turn on LEDs PB0 & PB1
				writeFlag--;																	// stop the while(writeFlag) loop in DRAFlashWrite()
				break;
			
			case 3:																					// second-word write-complete interrupt occurred
				GPIO_PORTB_DATA_R = 0x7;											// turn on LEDs PB0, PB1, & PB2
				writeFlag--;																	// stop the while(writeFlag) loop in DRAFlashWrite()
				break;
			
			case 4:																					// third-word write-complete interrupt occured
				GPIO_PORTB_DATA_R = 0xF;											// turn on LEDs PB0 - PB3
				writeFlag--;																	// stop while(writeFlag) loop in DRAFlashWrite()
				break;
		}
		Delay();
		FLASH_FCMISC_R = FLASH_FCMISC_PMISC;							// clear PMISC and PRIS bits to clear interrupt
	}
}

void Delay(void)
{
	for (ui32Loop = 0; ui32Loop < 2000000; ui32Loop++) {}
}