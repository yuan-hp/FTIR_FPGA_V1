/*
 * Hspi.c
 *
 *  Created on: 2019年6月13日
 *      Author: hpy
 */

#include "Hspi.h"
#include "xparameters.h"

/*****************************************************************************/
/**
*
* Runs a self-test on the driver/device. The self-test is destructive in that
* a reset of the device is performed in order to check the reset values of
* the registers and to get the device into a known state.
*
* Upon successful return from the self-test, the device is reset.
*
* @param	InstancePtr is a pointer to the XSpiPs instance.
*
* @return
* 		- XST_SUCCESS if successful
*		- XST_REGISTER_ERROR indicates a register did not read or write
*		correctly.
*
* @note		None.
*
******************************************************************************/
s32 XSpiPs_SelfTest(XSpiPs *InstancePtr)
{
	s32 Status;
	u32 Register;
	u8 DelayTestNss;
	u8 DelayTestBtwn;
	u8 DelayTestAfter;
	u8 DelayTestInit;

	Xil_AssertNonvoid(InstancePtr != NULL);
	Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	/*
	 * Reset the SPI device to leave it in a known good state
	 */
	XSpiPs_Reset(InstancePtr);

	/*
	 * All the SPI registers should be in their default state right now.
	 */
	Register = XSpiPs_ReadReg(InstancePtr->Config.BaseAddress,
				 XSPIPS_CR_OFFSET);
	if (Register != XSPIPS_CR_RESET_STATE) {
		return (s32)XST_REGISTER_ERROR;
	}

	Register = XSpiPs_ReadReg(InstancePtr->Config.BaseAddress,
				 XSPIPS_SR_OFFSET);
	if (Register != XSPIPS_ISR_RESET_STATE) {
		return (s32)XST_REGISTER_ERROR;
	}

	DelayTestNss = 0x5AU;
	DelayTestBtwn = 0xA5U;
	DelayTestAfter = 0xAAU;
	DelayTestInit = 0x55U;

	/*
	 * Write and read the delay register, just to be sure there is some
	 * hardware out there.
	 */
	Status = XSpiPs_SetDelays(InstancePtr, DelayTestNss, DelayTestBtwn,
				   DelayTestAfter, DelayTestInit);
	if (Status != (s32)XST_SUCCESS) {
		return Status;
	}

	XSpiPs_GetDelays(InstancePtr, &DelayTestNss, &DelayTestBtwn,
			&DelayTestAfter, &DelayTestInit);
	if ((0x5AU != DelayTestNss) || (0xA5U != DelayTestBtwn) ||
		(0xAAU != DelayTestAfter) || (0x55U != DelayTestInit)) {
		return (s32)XST_REGISTER_ERROR;
	}

	Status = XSpiPs_SetDelays(InstancePtr, 0U, 0U, 0U, 0U);
	if (Status != (s32)XST_SUCCESS) {
		return Status;
	}

	/*
	 * Reset the SPI device to leave it in a known good state
	 */
	XSpiPs_Reset(InstancePtr);

	return (s32)XST_SUCCESS;
}
/**
 * 功能：初始化spi
 * 函数名：Hspi_init
 * 参数1：Spi  XSpiPs指针
 * 参数2：spi ID
 * 操作成功返回0
 */
int Hspi_init(XSpiPs *Spi,u16 SpiDeviceId)
{

	XSpiPs_Config *SpiConfig;
	int Status;
	//查询spi
	SpiConfig = XSpiPs_LookupConfig(SpiDeviceId);
	if (NULL == SpiConfig) {
		return XST_FAILURE;
	}
	Status = XSpiPs_CfgInitialize(Spi, SpiConfig,SpiConfig->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	//自测
	Status = XSpiPs_SelfTest(Spi);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	//设置为spi主机
	XSpiPs_SetOptions(Spi, XSPIPS_MASTER_OPTION | XSPIPS_CLK_PHASE_1_OPTION);
	XSpiPs_SetClkPrescaler(Spi, XSPIPS_CLK_PRESCALE_256);//设置spi时钟
	XSpiPs_Enable(Spi);
	return XST_SUCCESS;
}
/*
 * 函数名：Hspi_Transfer
 * 功能：spi写/读数据
 * 参数1：Spi  XSpiPs指针
 * 参数2:ChipSlect 片选信号0-2
 * 参数2：SendBuf 发送字符串
 * 参数3：RecvBuf 接收字符串
 * 参数4：ByteCount 发送字节数
 * 操作成功返回0，否则返回1
 *  Created on: 2019年6月13日
 *      Author: hpy
 */
int Hspi_Transfer8(XSpiPs *Spi,u8 ChipSlect,u8 *SendBuf,u8 *RecvBuf,u32 ByteCount)
{
	XSpiPs_Abort(Spi);//清除之前的状态
	XSpiPs_SetSlaveSelect(Spi, ChipSlect);//片选
	if((s32)XST_SUCCESS == XSpiPs_Transfer(Spi,SendBuf,RecvBuf,ByteCount)){
		return 0;
	}
	return 1;
}
