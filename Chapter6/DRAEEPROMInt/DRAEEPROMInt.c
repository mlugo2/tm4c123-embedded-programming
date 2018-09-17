// DRAEEPROMPoll.c - Main Application File
#include "DRAEEPROMInt.h"

int main(void)
{
	uint32_t uiStatus, prData[18];
	uint32_t uiAddress = 0x100, uiCount = 18;
	
	HWREG(SYSCTL_RCGCEEPROM) = SYSCTL_RCGCEEPROM_R0;			// enable clock to EEPROM
	uiStatus = EEPROMInit();															// initialize EEPROM
	if (uiStatus) {return 1;}
	uiStatus = EEPROMWrite(pwData, uiAddress, uiCount);		// writing data into EEPROM
	if (uiStatus) {return 1;}
	EEPROMRead(prData, uiAddress, uiCount);								// reading data from EEPROM
	while(1) {}
}

uint32_t EEPROMInit(void)
{
	uint32_t uiStatus;
	volatile uint_fast8_t ui8Delay;
	
	// wait for WORKING bit is cleared to 0...
	while(EEPROM_EEDONE & EEPROM_EEDONE_WORKING) {}	// wait for WORKING bit clear...
	uiStatus = HWREG(EEPROM_EESUPP);								// check the PRETRY & ERETRY bit values
	if (uiStatus & (EEPROM_EESUPP_PRETRY|EEPROM_EESUPP_ERETRY))
	{	// reset the EEPROM by setting and clearing SREEPROM register
		HWREG(SREEPROM) = 0x1;
		for (ui8Delay = 0; ui8Delay < 16; ui8Delay++){}
		HWREG(SREEPROM) = 0x0;
			
		while(EEPROM_EEDONE & EEPROM_EEDONE_WORKING) {} // wait for WORKING bit is cleared to 0...
		uiStatus = HWREG(EEPROM_EESUPP);
		if (uiStatus & (EEPROM_EESUPP_PRETRY|EEPROM_EESUPP_ERETRY))
		{ return(EEPROM_INIT_ERROR); }
		else { return(EEPROM_INIT_RETRY); }
	}
	return( EEPROM_INIT_OK );
}

uint32_t EEPROMWrite(uint32_t *puiData, uint32_t uiAddr, uint32_t uiCount)
{	
	uint32_t uiStatus;
	
	IntRegister(INT_FLASH, FLASH_Handler);
	IntEnable(INT_FLASH);
	
	HWREG(SYSCTL_RCGCGPIO) = 0x02;								// enable clock to Port B
	HWREG(GPIO_PORTB_DIR) |= 0xF;									// set PB0-PB3 as output pins
	HWREG(GPIO_PORTB_DEN) |= 0xF;									// enable PB0~PB3 as digital function
	
	// wait for WORKING bit clear...
	do { uiStatus = HWREG(EEPROM_EEDONE);} while(uiStatus & EEPROM_EEDONE_WORKING);
	// configure & setup EEBLOCK & EEOFFSET
	HWREG(EEPROM_EEBLOCK) = EEPROMBlockFromAddr(uiAddr);
	HWREG(EEPROM_EEOFFSET) = OFFSET_FROM_ADDR(uiAddr);
	
	HWREG(EEPROM_EEINT)=EEPROM_EEINT_INT;							// enable the EEPROM interrupt
	HWREG(FLASH_FCMISC)=FLASH_FCMISC_EMISC;						// clear any previous ERIS interrupt
	HWREG(FLASH_FCIM)=FLASH_FCIM_EMASK;								// unmask bit[2](EMASK) in the FCIM register
	
	while(uiCount)
	{
		EEIntFlag++;
		HWREG(EEPROM_EERDWR) = *puiData;						// write the data into EEPROM
		while(EEIntFlag){}													// wait for the word-writing-complete interrupt
		puiData++;																	// update data proints to next data
		uiCount--;																	// update the data count
		HWREG(EEPROM_EEOFFSET)++;										// increment EEOFFSET to point to the next word
		if (HWREG(EEPROM_EEOFFSET)==0)
		{
			HWREG(EEPROM_EEBLOCK) += 1;								// increment EEOFFSET to point to next block
		}
	}
	HWREG(EEPROM_EEINT) = 0x0;										// disable the EEPROM interrupt
	return(HWREG(EEPROM_EEDONE));
}

void EEPROMRead(uint32_t *puiData, uint32_t uiAddr, uint32_t uiCount)
{
	// configure & setup EEBLOCK & EEOFFSET
	HWREG(EEPROM_EEBLOCK) = EEPROMBlockFromAddr(uiAddr);
	HWREG(EEPROM_EEOFFSET) = OFFSET_FROM_ADDR(uiAddr);
	
	while(uiCount)
	{
		*puiData = HWREG(EEPROM_EERDWR);				// read a word from EEPROM
		puiData++;
		uiCount--;
		HWREG(EEPROM_EEOFFSET)++;								// increment EEOFFSET to point to the next word
		if (HWREG(EEPROM_EEOFFSET)==0)
			HWREG(EEPROM_EEBLOCK)+=1;							// increment block number to point to next block
	}
}

void FLASH_Handler(void)
{
	uint32_t uiLoop;
	
	wCount++;																// increment the global variable wCount by 1
	if (FLASH_FCRIS & FLASH_FCRIS_ERIS)
	{
		switch(wCount)
		{
			case 1:															// 1st WWC interrupt occurted
				HWREG(GPIO_PORTB_DATA) = 0x1;			// set PB0 to HI to indicate this
				EEIntFlag--;											// stop while(EEIntFlag) loop in EEPROMWrite()
				break;
			case 2:															// 2nd WWC interrupt occurted
				HWREG(GPIO_PORTB_DATA) = 0x3;			// turn on LEDs PB0 & PB1
				EEIntFlag--;											// stop while(EEIntFlag) loop in EEPROMWrite()
				break;
			case 3:															// 3rd WWC interrupt occurted
				HWREG(GPIO_PORTB_DATA) = 0x7;			// turn on LEDs PB0 & PB1 & PB2
				EEIntFlag--;											// stop while(EEIntFlag) loop in EEPROMWrite()
				break;
			case 4:															// 4th WWC interrupt occurted
				HWREG(GPIO_PORTB_DATA) = 0xF;			// turn on LEDs PB0~PB3
				EEIntFlag--;											// stop while(EEIntFlag) loop in EEPROMWrite()
				break;
		}
		for(uiLoop=0;uiLoop<2000000;uiLoop++) {};
		HWREG(FLASH_FCMISC)=FLASH_FCMISC_EMISC;			// clear EMISC and ERIS bits to reset interrupt
	}
}
