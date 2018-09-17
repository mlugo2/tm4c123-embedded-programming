// DRAEEPROMPoll.h - Header File for DRAEEPROMPoll.c
#include <stdint.h>
#include <stdbool.h>
#include "inc\hw_eeprom.h"
#include "inc\hw_sysctl.h"
#include "driverlib\eeprom.h"

#include "driverlib\interrupt.h"
#include "inc\hw_flash.h"

#define SREEPROM		0x400FE558
#define HWREG(x)		(*((volatile uint32_t*)(x)))
#define OFFSET_FROM_ADDR(x)	(((x)>>2)&0x0F)

#define INT_FLASH		45
#define GPIO_PORTB_DATA		0x400053FC
#define GPIO_PORTB_DIR		0x40005400
#define GPIO_PORTB_DEN		0x4000551C
uint32_t EEIntFlag = 0, wCount = 0;
void FLASH_Handler(void);

uint32_t EEPROMInit(void);
uint32_t EEPROMWrite(uint32_t *puiData, uint32_t uiAddr, uint32_t uiCount);
void EEPROMRead(uint32_t *puiData, uint32_t uiAddr, uint32_t uiCount);

uint32_t pwData[] = {0x12345678, 0x23456789, 0x3456789A, 0x56789ABC,
										 0x6789ABCD, 0x789ABCDE, 0xDEBC9A78, 0x78563412,
										 0x89674523, 0x9A785634, 0xAB896745, 0xBC9A7856,
										 0x00112233, 0xABABABAB, 0x11223344, 0x22334455};
