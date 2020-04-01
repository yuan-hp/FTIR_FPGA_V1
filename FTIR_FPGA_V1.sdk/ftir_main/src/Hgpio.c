///*
// * Hgpio.c
// *
// *  Created on: 2019年6月18日
// *      Author: hpy
// */
//
//
//#include "Hgpio.h"
//#include "xparameters_ps.h"
//#include "Hak5394.h"
//#include "system.h"
//
//XGpio HGpio;//用于连接PL 2 PS中断的AXI GPIO的对象
///*************************************************************
// * PL到PS的AXI GPIO数据中断处理函数
// * 检测到中断后刷新数据，告知CPU进行数据处理
// *************************************************************/
///*
//static void AxiGpioHandler(void *CallbackRef)
//{
//	printf("中断函数\n");
//	XGpio *GpioPtr = (XGpio *)CallbackRef;
//	u32 state = Hak5394_GetStatus();
//	u32 ddr_slect = (state>>30) & 0x00000001;//读取片选信号
//	u32 data_num = Hak5394_GetData(2);//读取硬件采集的实际数据量
//	static u32 mcnt = 0;
//	printf("int cnt:%d\n",++mcnt);
//	if(ddr_slect == 0)
//	{
//		//printf("-------------0------------\n");
//		Xil_DCacheInvalidateRange(AK_5394_DATA1_BASE_ADDR,4*data_num);//刷新缓存，更新数据
//		Xil_DCacheInvalidateRange(AK_5394_TIME1_BASE_ADDR,4*data_num);//刷新缓存，更新数据
//
//	}
//	else
//	{
//		Xil_DCacheInvalidateRange(AK_5394_DATA2_BASE_ADDR,4*data_num);//刷新缓存，更新数据
//		Xil_DCacheInvalidateRange(AK_5394_TIME2_BASE_ADDR,4*data_num);//刷新缓存，更新数据
//	}
//	//Clear the Interrupt
//	XGpio_InterruptClear(GpioPtr, 0x00000001);
//}
//*/
//static void AxiGpioHandler(void *CallbackRef)
//{
//	printf("中断函数\n");
//	XGpio *GpioPtr = (XGpio *)CallbackRef;
//	u32 state = Hak5394_GetStatus();
//	u32 ddr_slect = (state>>30) & 0x00000001;//读取片选信号
//	u32 dLCount = Hak5394_GetData(2);//获取激光脉冲时间的数据量
//
//	//////////////////////////////////////////////////////////////////////
//
//	int errornum = 0;
//	//////////////////////////////////////////////////////////////////////
//
//	MirrorDirection = ddr_slect;
//	if(MirrorDirection==0)
//	{
//		ForwordSpecNum = Hak5394_GetData(2);//读取硬件采集的实际数据量
//		Xil_DCacheInvalidateRange(AK_5394_DATA1_BASE_ADDR,4*ForwordSpecNum);//刷新缓存，更新数据
//		printf("有效数据:%u   片选:%u\n",ForwordSpecNum,ddr_slect);
//		////////////////////////////////////////////////////////////////////////////////////
//	}
//	if(MirrorDirection==1)
//	{
//		BackwordSpecNum = Hak5394_GetData(2);//读取硬件采集的实际数据量
//		Xil_DCacheInvalidateRange(AK_5394_DATA2_BASE_ADDR,4*BackwordSpecNum);//刷新缓存，更新数据
//		printf("有效数据:%u   片选:%u\n",BackwordSpecNum,ddr_slect);
//		////////////////////////////////////////////////////////////////////////////////////
//	}
//
//
//	if(PCNeedDataFlag==1)
//	{
//		MakeTcpHead();
//		int datanum;
//		datanum = MakeSpecTcpData();
//		LinTcpSend(datanum);
//		LwipBusyFlag = 1;
//		//TaskManager();
//		//ADDataAcquire();//定时读取ad数据
//	}
//
//
//	/* Clear the Interrupt */
//	XGpio_InterruptClear(GpioPtr, 0x00000001);
//}
///*********************************************************************
// * 初始化AXI GPIO,配置AXI中断
// * 函数将会配置中断，用于检测数据时候可以处理，并将中断函数AxiGpioHandler
// * 与axi gpio进行关联
// *********************************************************************/
//int Hgpio_axi_int_init() {
//	//printf("开始配置AXI GPIO\n");
//	u32 Status;
//	Status = XGpio_Initialize(&HGpio, XPAR_GPIO_0_DEVICE_ID);
//	if (Status != XST_SUCCESS) {
//		return XST_FAILURE;
//	}
//	//printf("开始配置AXI GPIO:1\n");
//	XScuGic_Config *IntcConfig;
//	/*
//	 * Initialize the interrupt controller driver so that it is ready to
//	 * use.
//	 */
//	/*
//	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
//	if (NULL == IntcConfig) {
//		return XST_FAILURE;
//	}
//	Status = XScuGic_CfgInitialize(&LinIntcInstance, IntcConfig,
//			IntcConfig->CpuBaseAddress);
//	if (Status != XST_SUCCESS) {
//		return XST_FAILURE;
//	}
//	*/
//	XScuGic_SetPriorityTriggerType(&LinIntcInstance,
//			XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR, 0xA0, 0x1);
//	/*
//	 * Connect the interrupt handler that will be called when an
//	 * interrupt occurs for the device.
//	 */
//	Status = XScuGic_Connect(&LinIntcInstance,
//			XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR,
//			(Xil_ExceptionHandler) AxiGpioHandler, &HGpio);
//	if (Status != XST_SUCCESS) {
//		return Status;
//	}
//	//printf("开始配置AXI GPIO:2\n");
//	/* Enable the interrupt for the GPIO device.*/
//	XScuGic_Enable(&LinIntcInstance, XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR);
//	//printf("开始配置AXI GPIO:3\n");
//	/*
//	 * Enable the GPIO channel interrupts so that push button can be
//	 * detected and enable interrupts for the GPIO device
//	 */
//	XGpio_InterruptEnable(&HGpio, 0x00000001);//开启第一位的中断
//	printf("开始配置AXI GPIO:4\n");
//	XGpio_InterruptGlobalEnable(&HGpio);
//	printf("开始配置AXI GPIO:5\n");
//	/*
//	 * Initialize the exception table and register the interrupt
//	 * controller handler with the exception table
//	 */
//	Xil_ExceptionInit();
//	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
//			(Xil_ExceptionHandler) XScuGic_InterruptHandler, &LinIntcInstance);
//	/* Enable non-critical exceptions */
//	Xil_ExceptionEnable();
//	printf("配置AXI GPIO 结束\n");
//	return XST_SUCCESS;
//}
///**
// * 初始化ps的gpio
// * 参数1：ps gpio指针对象
// * 参数2：ps gpio的引脚
// * 参数3：设置ps gpio的方向 0 input   1：output
// * 参数4：ps gpio的设备ID号
// * 操作成功返回 XST_SUCCESS
// */
//int HgpioPs_init(XGpioPs *Gpio,u32 pin,u8 direction,u16 DeviceId)
//{
//	int Status;
//	XGpioPs_Config *ConfigPtr;
//
//	ConfigPtr = XGpioPs_LookupConfig(DeviceId);
//	Status = XGpioPs_CfgInitialize(Gpio, ConfigPtr,ConfigPtr->BaseAddr);
//	if (Status != XST_SUCCESS) {
//		return XST_FAILURE;
//	}
//	HgpioPs_SetDir(Gpio,pin,direction);//设置方向 0 input  1  output
//	return XST_SUCCESS;
//}
///****************************************************************************/
///**
//*
//* Set the Direction of the specified pin.
//*
//* @param	InstancePtr is a pointer to the XGpioPs instance.
//* @param	Pin is the pin number to which the Data is to be written.
//*		Valid values are 0-117 in Zynq and 0-173 in Zynq Ultrascale+ MP.
//* @param	Direction is the direction to be set for the specified pin.
//*		Valid values are 0 for Input Direction, 1 for Output Direction.
//*
//* @return	None.
//*
//*****************************************************************************/
//int HgpioPs_SetDir(XGpioPs *Gpio,u32 pin,u32 direction)
//{
//	XGpioPs_SetDirectionPin(Gpio, pin,direction);
//	return 0;
//}
///**
//*读取引脚输入电平
//*/
//u32 HgpioPs_ReadPin(XGpioPs *Gpio,u32 pin)
//{
//	return XGpioPs_ReadPin(Gpio, pin);
//}
///**
// * 使能指定引脚的输出
// */
//void HgpioPs_EnableOutput(XGpioPs *Gpio,u32 pin)
//{
//	XGpioPs_SetOutputEnablePin(Gpio, pin, 1);
//}
///**
// * 关闭指定引脚的输出
// */
//void HgpioPs_DisableOutput(XGpioPs *Gpio,u32 pin)
//{
//	XGpioPs_SetOutputEnablePin(Gpio, pin, 0);
//}
///**
// * 设置指定引脚的输出电平值
// */
//void HgpioPs_WritePin(XGpioPs *Gpio,u32 pin,u32 data)
//{
//	XGpioPs_WritePin(Gpio,pin,data);
//}
////PL axi gpio初始化
///*
//int Hgpio_init(XGpio *Gpio,u16 DeviceId)
//{
//	int Status;
//		u32 delay;
//		Status = XGpio_Initialize(InstancePtr, DeviceId);
//		if (Status != XST_SUCCESS) {
//			return XST_FAILURE;
//		}
//}
//*/
//
/*
 * Hgpio.c
 *
 *  Created on: 2019年6月18日
 *      Author: hpy
 */


#include "Hgpio.h"
#include "xparameters_ps.h"
#include "Hak5394.h"
#include "system.h"

XGpio HGpio;//用于连接PL 2 PS中断的AXI GPIO的对象
/*************************************************************
 * PL到PS的AXI GPIO数据中断处理函数
 * 检测到中断后刷新数据，告知CPU进行数据处理
 *************************************************************/
/*
static void AxiGpioHandler(void *CallbackRef)
{
	printf("中断函数\n");
	XGpio *GpioPtr = (XGpio *)CallbackRef;
	u32 state = Hak5394_GetStatus();
	u32 ddr_slect = (state>>30) & 0x00000001;//读取片选信号
	u32 data_num = Hak5394_GetData(2);//读取硬件采集的实际数据量
	static u32 mcnt = 0;
	printf("int cnt:%d\n",++mcnt);
	if(ddr_slect == 0)
	{
		//printf("-------------0------------\n");
		Xil_DCacheInvalidateRange(AK_5394_DATA1_BASE_ADDR,4*data_num);//刷新缓存，更新数据
		Xil_DCacheInvalidateRange(AK_5394_TIME1_BASE_ADDR,4*data_num);//刷新缓存，更新数据

	}
	else
	{
		Xil_DCacheInvalidateRange(AK_5394_DATA2_BASE_ADDR,4*data_num);//刷新缓存，更新数据
		Xil_DCacheInvalidateRange(AK_5394_TIME2_BASE_ADDR,4*data_num);//刷新缓存，更新数据
	}
	//Clear the Interrupt
	XGpio_InterruptClear(GpioPtr, 0x00000001);
}
*/
//static void AxiGpioHandler(void *CallbackRef)
//{
//	printf("中断函数\n");
//	XGpio *GpioPtr = (XGpio *)CallbackRef;
//	u32 state = Hak5394_GetStatus();
//	u32 ddr_slect = (state>>30) & 0x00000001;//读取片选信号
//
//
//	//////////////////////////////////////////////////////////////////////
//
//	int errornum = 0;
//	//////////////////////////////////////////////////////////////////////
//
//	MirrorDirection = ddr_slect;
//	if(MirrorDirection==0)//正向
//	{
//		ForwordSpecNum = Hak5394_GetData(2);//读取硬件采集的实际数据量
//		Xil_DCacheInvalidateRange(AK_5394_DATA1_BASE_ADDR,4*ForwordSpecNum);//刷新光谱缓存，更新数据
//		Xil_DCacheInvalidateRange(AK_5394_TIME1_BASE_ADDR,4*ForwordSpecNum);//刷新光谱时刻缓存，更新数据
//
//		ForwordSpeedNum = Hak5394_GetData(1);//获取激光脉冲时间的数据量
//		Xil_DCacheInvalidateRange(AK_5394_LASER_PULSE_TIME1_BASE_ADDR,4*ForwordSpeedNum);//刷新缓存，更新数据
//		for(int i = 0;i<3000;i++)
//		{
//			printf("%d\r\n",ak5394_m1[i]);
//		}
//
//		//printf("有效数据:%u   片选:%u\n",ForwordSpecNum,ddr_slect);
//		////////////////////////////////////////////////////////////////////////////////////
//	}
//	if(MirrorDirection==1)//反向
//	{
//		BackwordSpecNum = Hak5394_GetData(2);//读取硬件采集的实际数据量
//		Xil_DCacheInvalidateRange(AK_5394_DATA2_BASE_ADDR,4*BackwordSpecNum);//刷新光谱缓存，更新数据
//		Xil_DCacheInvalidateRange(AK_5394_TIME2_BASE_ADDR,4*BackwordSpecNum);//刷新缓存，更新数据
//
//		BackwordSpeedNum = Hak5394_GetData(1);//获取激光脉冲时间的数据量
//		Xil_DCacheInvalidateRange(AK_5394_LASER_PULSE_TIME2_BASE_ADDR,4*BackwordSpeedNum);//刷新缓存，更新数据
//
//		//printf("有效数据:%u   片选:%u\n",BackwordSpecNum,ddr_slect);
//		////////////////////////////////////////////////////////////////////////////////////
//	}
//
//	/* Clear the Interrupt */
//	XGpio_InterruptClear(GpioPtr, 0x00000001);
//}


static void AxiGpioHandler(void *CallbackRef)
{
	printf("中断函数\n");
	XGpio *GpioPtr = (XGpio *)CallbackRef;
	u32 state = Hak5394_GetStatus();
	u32 ddr_slect = (state>>30) & 0x00000001;//读取片选信号


	//////////////////////////////////////////////////////////////////////

	int errornum = 0;
	//////////////////////////////////////////////////////////////////////

	MirrorDirection = ddr_slect;
	if(MirrorDirection==0)//正向
	{
		ForwordSpecNum = Hak5394_GetData(2);//读取硬件采集的实际数据量
		Xil_DCacheInvalidateRange(AK_5394_DATA1_BASE_ADDR,4*ForwordSpecNum);//刷新光谱缓存，更新数据
		Xil_DCacheInvalidateRange(AK_5394_TIME1_BASE_ADDR,4*ForwordSpecNum);//刷新光谱时刻缓存，更新数据

		ForwordSpeedNum = Hak5394_GetData(1);//获取激光脉冲时间的数据量
		Xil_DCacheInvalidateRange(AK_5394_LASER_PULSE_TIME1_BASE_ADDR,4*ForwordSpeedNum);//刷新缓存，更新数据

		printf("有效数据:%u   片选:%u\n",ForwordSpecNum,ddr_slect);
		////////////////////////////////////////////////////////////////////////////////////
	}
	if(MirrorDirection==1)//反向
	{
		BackwordSpecNum = Hak5394_GetData(2);//读取硬件采集的实际数据量
		Xil_DCacheInvalidateRange(AK_5394_DATA2_BASE_ADDR,4*BackwordSpecNum);//刷新光谱缓存，更新数据
		Xil_DCacheInvalidateRange(AK_5394_TIME2_BASE_ADDR,4*BackwordSpecNum);//刷新缓存，更新数据

		BackwordSpeedNum = Hak5394_GetData(1);//获取激光脉冲时间的数据量
		Xil_DCacheInvalidateRange(AK_5394_LASER_PULSE_TIME2_BASE_ADDR,4*BackwordSpeedNum);//刷新缓存，更新数据

		printf("有效数据:%u   片选:%u\n",BackwordSpecNum,ddr_slect);
		////////////////////////////////////////////////////////////////////////////////////
	}

	if(Mypcb->state==CLOSED)
	{
		PCNeedDataFlag=0;
		//printf("未连接\r\n");

	}


	if(PCNeedDataFlag==1)
	{
		MakeTcpHead();
		//LinTcpSend(datanum);

		if(MirrorDirection==0)//正向
		{
			////////////////////////////////////////////////////////////////////////////////////
			ForwordNeedSendFlag=1;
			LwipForwordNum=MakeSpecTcpData();

		}
		if(MirrorDirection==1)//反向
		{
			////////////////////////////////////////////////////////////////////////////////////
			BackwordNeedSendFlag=1;
			LwipBackwordNum=MakeSpecTcpData();

		}

	}


	/* Clear the Interrupt */
	XGpio_InterruptClear(GpioPtr, 0x00000001);
}
/*********************************************************************
 * 初始化AXI GPIO,配置AXI中断
 * 函数将会配置中断，用于检测数据时候可以处理，并将中断函数AxiGpioHandler
 * 与axi gpio进行关联
 *********************************************************************/
int Hgpio_axi_int_init() {
	//printf("开始配置AXI GPIO\n");
	u32 Status;
	Status = XGpio_Initialize(&HGpio, XPAR_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	//printf("开始配置AXI GPIO:1\n");
	XScuGic_Config *IntcConfig;
	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	/*
	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}
	Status = XScuGic_CfgInitialize(&LinIntcInstance, IntcConfig,
			IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	*/
	XScuGic_SetPriorityTriggerType(&LinIntcInstance,
			XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR, 0xA0, 0x1);
	/*
	 * Connect the interrupt handler that will be called when an
	 * interrupt occurs for the device.
	 */
	Status = XScuGic_Connect(&LinIntcInstance,
			XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR,
			(Xil_ExceptionHandler) AxiGpioHandler, &HGpio);
	if (Status != XST_SUCCESS) {
		return Status;
	}
	//printf("开始配置AXI GPIO:2\n");
	/* Enable the interrupt for the GPIO device.*/
	XScuGic_Enable(&LinIntcInstance, XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR);
	//printf("开始配置AXI GPIO:3\n");
	/*
	 * Enable the GPIO channel interrupts so that push button can be
	 * detected and enable interrupts for the GPIO device
	 */
	XGpio_InterruptEnable(&HGpio, 0x00000001);//开启第一位的中断
	printf("开始配置AXI GPIO:4\n");
	XGpio_InterruptGlobalEnable(&HGpio);
	printf("开始配置AXI GPIO:5\n");
	/*
	 * Initialize the exception table and register the interrupt
	 * controller handler with the exception table
	 */
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler) XScuGic_InterruptHandler, &LinIntcInstance);
	/* Enable non-critical exceptions */
	Xil_ExceptionEnable();
	printf("配置AXI GPIO 结束\n");
	return XST_SUCCESS;
}
/**
 * 初始化ps的gpio
 * 参数1：ps gpio指针对象
 * 参数2：ps gpio的引脚
 * 参数3：设置ps gpio的方向 0 input   1：output
 * 参数4：ps gpio的设备ID号
 * 操作成功返回 XST_SUCCESS
 */
int HgpioPs_init(XGpioPs *Gpio,u32 pin,u8 direction,u16 DeviceId)
{
	int Status;
	XGpioPs_Config *ConfigPtr;

	ConfigPtr = XGpioPs_LookupConfig(DeviceId);
	Status = XGpioPs_CfgInitialize(Gpio, ConfigPtr,ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	HgpioPs_SetDir(Gpio,pin,direction);//设置方向 0 input  1  output
	return XST_SUCCESS;
}
/****************************************************************************/
/**
*
* Set the Direction of the specified pin.
*
* @param	InstancePtr is a pointer to the XGpioPs instance.
* @param	Pin is the pin number to which the Data is to be written.
*		Valid values are 0-117 in Zynq and 0-173 in Zynq Ultrascale+ MP.
* @param	Direction is the direction to be set for the specified pin.
*		Valid values are 0 for Input Direction, 1 for Output Direction.
*
* @return	None.
*
*****************************************************************************/
int HgpioPs_SetDir(XGpioPs *Gpio,u32 pin,u32 direction)
{
	XGpioPs_SetDirectionPin(Gpio, pin,direction);
	return 0;
}
/**
*读取引脚输入电平
*/
u32 HgpioPs_ReadPin(XGpioPs *Gpio,u32 pin)
{
	return XGpioPs_ReadPin(Gpio, pin);
}
/**
 * 使能指定引脚的输出
 */
void HgpioPs_EnableOutput(XGpioPs *Gpio,u32 pin)
{
	XGpioPs_SetOutputEnablePin(Gpio, pin, 1);
}
/**
 * 关闭指定引脚的输出
 */
void HgpioPs_DisableOutput(XGpioPs *Gpio,u32 pin)
{
	XGpioPs_SetOutputEnablePin(Gpio, pin, 0);
}
/**
 * 设置指定引脚的输出电平值
 */
void HgpioPs_WritePin(XGpioPs *Gpio,u32 pin,u32 data)
{
	XGpioPs_WritePin(Gpio,pin,data);
}
//PL axi gpio初始化
/*
int Hgpio_init(XGpio *Gpio,u16 DeviceId)
{
	int Status;
		u32 delay;
		Status = XGpio_Initialize(InstancePtr, DeviceId);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}
}
*/

