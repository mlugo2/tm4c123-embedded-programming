// DRAFlashInt.h - Header File for Main Application File
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/interrupt.h"

#define INT_FLASH		45						// Flash Memory Control and EEPROM

void FLASHM_Handler(void);
void Delay(void);

uint32_t DRAFlashErase(uint32_t uiAddr);
uint32_t DRAFlashWrite(uint32_t *pData, uint32_t uiAddr, uint32_t uiCount);
uint32_t eraseFlag = 0, writeFlag = 0;
uint32_t ui32Loop, intCount = 0;