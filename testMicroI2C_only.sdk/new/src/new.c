

/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xiic.h"
#include "xintc.h"
#include "xil_exception.h"
#include "sleep.h"
#include "xil_printf.h"

#include "i2cCustom.h"

/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define IIC_DEVICE_ID			XPAR_IIC_0_DEVICE_ID
#define INTC_DEVICE_ID			XPAR_INTC_0_DEVICE_ID
#define IIC_INTR_ID			XPAR_INTC_0_IIC_0_VEC_ID


#define EEPROM_ADDRESS		0x69	/* 0xA0 as an 8 bit number. */


typedef u8 AddressType;
typedef u16 AddressType1;


int config_mpu(void);


/************************** Variable Definitions *****************************/

XIic IicInstance;		/* The instance of the IIC device. */
XIntc InterruptController;	/* The instance of the Interrupt Controller. */

/*
 * Write buffer for writing a page.
 */
u8 WriteBuffer[2]={MPUREG_PWR_MGMT_1,0x01};

u8 ReadBuffer[1]={0x00};	/* Read buffer for reading a page. */

volatile u8 TransmitComplete;	/* Flag to check completion of Transmission */
volatile u8 ReceiveComplete;	/* Flag to check completion of Reception */

u8 EepromIicAddr;		/* Variable for storing Eeprom IIC address */


int main(void)
{
	int Status;

	u8 Appu,Start_Number_High,Start_Number_Low =0;		// variable to read back the values
	u16 Start_Number=0;
	/*
	 * Run the Dynamic EEPROM example.
	 */
	Status = IicDynInit(0x69);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Appu=DynReadReg(0x75);
	xil_printf("%d",Appu);

	Status= config_mpu();
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Start_Number_High=DynReadReg(0x19);
	xil_printf("%d",Start_Number_High);
	Status=writeReg(MPUREG_FIFO_EN,0b11111000);
	Start_Number_High=DynReadReg(MPUREG_FIFO_EN);
		xil_printf("%d",Start_Number_High);
while(1)
{
	Start_Number_High=DynReadReg(MPUREG_TEMP_OUT_H);
	Start_Number_Low=DynReadReg(MPUREG_TEMP_OUT_L);

	u16 Start_Number = (Start_Number_High << 8) | (Start_Number_Low & 0xff);

	xil_printf("%d",Start_Number);
	xil_printf("\n\r");

	sleep(1);
}

	return XST_SUCCESS;
}

