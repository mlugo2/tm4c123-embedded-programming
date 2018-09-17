#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_i2c.h"
#include "inc/hw_types.h"
#include "driverlib/i2c.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#define SLAVE_ADDRESS						0x3C
#define GPIO_PB2_I2C0SCL				0x00010803
#define GPIO_PB3_I2C0SDA				0x00010C03

int main(void)
{
	char pui32DataTx[3] = {'I', '2', 'C' };
	char pui32DataRx[3];
	uint32_t ui32Index;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);		// enable I2C0 clock 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);	// enable GPIO Port B clock
	
	GPIOPinTypeI2C(SYSCTL_PERIPH_GPIOB, GPIO_PIN_3|GPIO_PIN_2);
	GPIOPinConfigure(GPIO_PB3_I2C0SDA);
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	
	HWREG(I2C0_BASE + I2C_O_MCR) |= 0x01;
	I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
	
	I2CSlaveEnable(I2C0_BASE);
	I2CSlaveInit(I2C0_BASE, SLAVE_ADDRESS);
	
	I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, true);
	
	for ( ui32Index = 0; ui32Index < 3; ui32Index++ )
	{
		I2CMasterDataPut(I2C0_BASE, pui32DataTx[ui32Index]);
	}
	
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	
	ui32Index = 0;
	while(I2CSlaveStatus(I2C0_BASE) && I2C_SLAVE_ACT_RREQ)
  {
		pui32DataRx[ui32Index++] = (char)I2CSlaveDataGet(I2C0_BASE);
	}		
	
	
}
