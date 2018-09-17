// DRAEEPROMPoll.c - Main Application File
#include "DRAEEPROMPoll.h"

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
	
	// wait for WORKING bit clear...
	do { uiStatus = HWREG(EEPROM_EEDONE);} while(uiStatus & EEPROM_EEDONE_WORKING);
	// configure & setup EEBLOCK & EEOFFSET
	HWREG(EEPROM_EEBLOCK) = EEPROMBlockFromAddr(uiAddr);
	HWREG(EEPROM_EEOFFSET) = OFFSET_FROM_ADDR(uiAddr);
	
	while(uiCount)
	{
		HWREG(EEPROM_EERDWR) = *puiData;						// write the data into EEPROM
		do																					// wait for data-writing to be done 
		{ uiStatus = HWREG(EEPROM_EEDONE); } while(uiStatus & EEPROM_EEDONE_WORKING);
		puiData++;																	// update data proints to next data
		uiCount--;																	// update the data count
		HWREG(EEPROM_EEOFFSET)++;										// increment EEOFFSET to point to the next word
		if (HWREG(EEPROM_EEOFFSET)==0)
		{
			HWREG(EEPROM_EEBLOCK) += 1;								// increment EEOFFSET to point to next block
		}
	}
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
