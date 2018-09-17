/*******************************************
 *	DRAFlash.c - Main Application File		 *
 *******************************************/
 #include <stdint.h>
 #include <stdbool.h>
 #include "tm4c123gh6pm.h"
 
 int main(void)
 {
	 volatile uint32_t ui32Loop;
	 unsigned long ulCount, ulAddress, *proData;
	 unsigned long Data[3] = {0x78563412, 0x8B674523, 0xA3456789};
	 proData = Data;
	 ulAddress = 0x1000; ulCount = 3;										// Starting adress and number of words to be written
	 
	 SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;								// Enable PORTF in RCGC2
	 GPIO_PORTF_DIR_R = 0x0E;														// Set PF1~PF3 pins as output pins
	 GPIO_PORTF_DEN_R = 0x0E;														// Enable PF1~PF3 pins for digital function
	 
	 // Erase flash memory 1 KB block starting 0x1000
	 FLASH_FCMISC_R = FLASH_FCMISC_AMISC;								// Clear the flash access interrupt
	 FLASH_FMA_R = ulAddress;														// Erase
	 FLASH_FMC_R = FLASH_FMC_WRKEY|FLASH_FMC_ERASE;
	 
	 // Wait for the erase to complete...
	 while (FLASH_FMC_R & FLASH_FMC_ERASE) 
	 {
	 }
		 
	 GPIO_PORTF_DATA_R = 0x08;														// Set FP3 to HI to indicate erasing done
	 
	 for (ui32Loop = 0; ui32Loop < 2000000; ui32Loop++)
   {
	 }																										// Delay a period to keep PF3 on
	 
	 // Loop to perform words programming
	 while (ulCount)
	 {
		 FLASH_FMA_R = ulAddress;
		 FLASH_FMD_R = *proData;
		 FLASH_FMC_R = FLASH_FMC_WRKEY|FLASH_FMC_WRITE;
		 // Wait until the word has been programmed
		 while(FLASH_FMC_R&FLASH_FMC_WRITE)
		 {
		 }
		 // Increment to the next word.
		 proData++;
		 ulAddress += 4;
		 ulCount--;
	 }
	 GPIO_PORTF_DATA_R = 0x04;														// Set PF2 to HI to indicate writing done
 }
