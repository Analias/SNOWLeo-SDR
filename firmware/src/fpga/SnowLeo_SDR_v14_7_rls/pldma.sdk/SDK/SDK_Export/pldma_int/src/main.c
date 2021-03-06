/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * Test PLDMA
 * include pldma_mwr, pldma_mrd
 * include Poll mode and Interrupt mode
 *  * **************************************************************************************************
 * *******************************************only active when lm6002 configured by iic*****************
 * *****************************************************************************************************
 *
 */

#include <stdio.h>
#include "xparameters.h"
#include "xil_cache.h"
#include "xil_exception.h"
#include "xscugic.h"




#define PLDMA_MWR_IRQ_ID XPAR_FABRIC_PLDMA_MWR_0_DMA_DONE_IRQ_INTR
#define PLDMA_MRD_IRQ_ID XPAR_FABRIC_PLDMA_MRD_0_DMA_DONE_IRQ_INTR

#define PLDMA_mWriteReg(BaseAddress, RegOffset, Data) \
 	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define PLDMA_mReadReg(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (RegOffset))

int counter1,counter2;
XScuGic InterruptController;
unsigned int mwr_addr = XPAR_PLDMA_MWR_0_BASEADDR;
unsigned int mrd_addr = XPAR_PLDMA_MRD_0_BASEADDR;

/*****************************************************************************/
/**
*
* This function is used by the TestAppGen generated application to setup
* the interrupt controller.
*
* @param	IntcInstancePtr is the reference to the Interrupt Controller
*		instance.
* @param	DeviceId is device ID of the Interrupt Controller Device,
*		typically XPAR_<INTC_instance>_DEVICE_ID value from
*		xparameters.h.
*
* @return	XST_SUCCESS to indicate success, otherwise XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int ScuGicInterruptSetup(XScuGic *IntcInstancePtr, u16 DeviceId)
{

	int Status;
	static XScuGic_Config *GicConfig;

	GicConfig = XScuGic_LookupConfig(DeviceId);
	if (NULL == GicConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(IntcInstancePtr, GicConfig,
					GicConfig->CpuBaseAddress);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
				(Xil_ExceptionHandler)XScuGic_InterruptHandler,
				IntcInstancePtr);
	Xil_ExceptionEnable();
	return XST_SUCCESS;

}
void PldmaMwr_IRQHandler()
{

	//clear interrupt
	printf("mwr times = %d\n",counter1++);
	PLDMA_mWriteReg(mwr_addr, 0x00, 0x10);		// clear irq
	PLDMA_mWriteReg(mwr_addr, 0x04, 0x01000000);// DMA write destination address
	PLDMA_mWriteReg(mwr_addr, 0x08, 1024);		// DMA br len
	PLDMA_mWriteReg(mwr_addr, 0x0c, 5);			// DMA times
	PLDMA_mWriteReg(mwr_addr, 0x00, 0x11);		// DMA start

}

void PldmaMrd_IRQHandler()
{

	//clear interrupt
	printf("mrd times = %d\n",counter2++);
	PLDMA_mWriteReg(mrd_addr, 0x00, 0x10);		// clear irq
	PLDMA_mWriteReg(mrd_addr, 0x04, 0x10000000);// DMA write destination address
	PLDMA_mWriteReg(mrd_addr, 0x08, 1024);		// DMA br len
	PLDMA_mWriteReg(mrd_addr, 0x0c, 5);			// DMA times
	PLDMA_mWriteReg(mrd_addr, 0x00, 0x11);		// DMA start

}

int SetUpInterruptSystem(XScuGic *XScuGicInstancePtr)
{
  /*
   * Connect the interrupt controller interrupt handler to the hardware
   * interrupt handling logic in the ARM processor.
   */
  Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
      (Xil_ExceptionHandler) XScuGic_InterruptHandler,
      XScuGicInstancePtr);
  /*
   * Enable interrupts in the ARM
   */
  Xil_ExceptionEnable();
  return XST_SUCCESS;
}



int main()
{
	u32 Status;
   //disable cache for pldma
	Xil_DCacheDisable();
    Xil_ICacheDisable();

    printf("Hello World\n\r");

    // pl ctrl regs test
    PLDMA_mWriteReg(mwr_addr, 0x10, 0x11000011);
    PLDMA_mWriteReg(mwr_addr, 0x14, 0x11000014);
    PLDMA_mWriteReg(mwr_addr, 0x18, 0x11000018);
    PLDMA_mWriteReg(mwr_addr, 0x1c, 0x1100001c);
    PLDMA_mWriteReg(mwr_addr, 0x20, 0x11000020);
    PLDMA_mWriteReg(mwr_addr, 0x24, 0x11000024);	// the readonly reg
    Status = PLDMA_mReadReg(mwr_addr, 0x10);
    Status = PLDMA_mReadReg(mwr_addr, 0x14);
    Status = PLDMA_mReadReg(mwr_addr, 0x18);
    Status = PLDMA_mReadReg(mwr_addr, 0x1c);
    Status = PLDMA_mReadReg(mwr_addr, 0x20);
    Status = PLDMA_mReadReg(mwr_addr, 0x24);

    //initial ps interrupt controller
    Status = ScuGicInterruptSetup(&InterruptController, XPAR_PS7_SCUGIC_0_DEVICE_ID);
    if (Status == 0) {
       printf("ScuGic Interrupt Setup PASSED\r\n");
    }
    Status = XScuGic_Connect(&InterruptController,PLDMA_MWR_IRQ_ID,(Xil_ExceptionHandler)PldmaMwr_IRQHandler,0);
    if (Status != XST_SUCCESS) {
      return XST_FAILURE;
    }
    Status = XScuGic_Connect(&InterruptController,PLDMA_MRD_IRQ_ID,(Xil_ExceptionHandler)PldmaMrd_IRQHandler,0);
    if (Status != XST_SUCCESS) {
      return XST_FAILURE;
    }

    XScuGic_Enable(&InterruptController, PLDMA_MWR_IRQ_ID);
    XScuGic_Enable(&InterruptController, PLDMA_MRD_IRQ_ID);
    //pldma_mwr control
    //while(1)				//while for Poll mode test
    //{
	PLDMA_mWriteReg(mwr_addr, 0x04, 0x01000000);	// DMA write destination address
	PLDMA_mWriteReg(mwr_addr, 0x08, 1024);			// DMA br len
	PLDMA_mWriteReg(mwr_addr, 0x0c, 5);				// DMA times
	PLDMA_mWriteReg(mwr_addr, 0x00, 0x11);			// DMA start
	//while(!PLDMA_mReadReg(mwr_addr, 0x00));
	//PLDMA_mWriteReg(mwr_addr, 0x00, 0x10);
	//}
	//pldma_mrd control
	//while(1)				//while for Poll mode test
	//{
	PLDMA_mWriteReg(mrd_addr, 0x04, 0x10000000);	// DMA base address		0x20000000
	PLDMA_mWriteReg(mrd_addr, 0x08, 2048);			// DMA br len
	PLDMA_mWriteReg(mrd_addr, 0x0c, 0);			// DMA times
	PLDMA_mWriteReg(mrd_addr, 0x00, 0x11);			// DMA start
	//while(!PLDMA_mReadReg(mrd_addr, 0x00));
	//PLDMA_mWriteReg(mrd_addr, 0x00, 0x10);
	//}

    while(1);

    return 0;
}
