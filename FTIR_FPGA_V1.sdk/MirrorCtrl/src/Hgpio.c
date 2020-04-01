/*
 * Hgpio.c
 *
 *  Created on: 2019��6��18��
 *      Author: hpy
 */


#include "Hgpio.h"
#include "xparameters_ps.h"
#include "Hak5394.h"
#include "system.h"

XGpio HGpio;//��������PL 2 PS�жϵ�AXI GPIO�Ķ���
/*************************************************************
 * PL��PS��AXI GPIO�����жϴ�����
 * ��⵽�жϺ�ˢ�����ݣ���֪CPU�������ݴ���
 *************************************************************/
/*
static void AxiGpioHandler(void *CallbackRef)
{
	printf("�жϺ���\n");
	XGpio *GpioPtr = (XGpio *)CallbackRef;
	u32 state = Hak5394_GetStatus();
	u32 ddr_slect = (state>>30) & 0x00000001;//��ȡƬѡ�ź�
	u32 data_num = Hak5394_GetData(2);//��ȡӲ���ɼ���ʵ��������
	static u32 mcnt = 0;
	printf("int cnt:%d\n",++mcnt);
	if(ddr_slect == 0)
	{
		//printf("-------------0------------\n");
		Xil_DCacheInvalidateRange(AK_5394_DATA1_BASE_ADDR,4*data_num);//ˢ�»��棬��������
		Xil_DCacheInvalidateRange(AK_5394_TIME1_BASE_ADDR,4*data_num);//ˢ�»��棬��������

	}
	else
	{
		Xil_DCacheInvalidateRange(AK_5394_DATA2_BASE_ADDR,4*data_num);//ˢ�»��棬��������
		Xil_DCacheInvalidateRange(AK_5394_TIME2_BASE_ADDR,4*data_num);//ˢ�»��棬��������
	}
	//Clear the Interrupt
	XGpio_InterruptClear(GpioPtr, 0x00000001);
}
*/
static void AxiGpioHandler(void *CallbackRef)
{
	printf("�жϺ���\n");
	XGpio *GpioPtr = (XGpio *)CallbackRef;
	//u32 state = Hak5394_GetStatus();
	//u32 ddr_slect = (state>>30) & 0x00000001;//��ȡƬѡ�ź�
	//u32 dLCount = Hak5394_GetData(2);//��ȡ��������ʱ���������


	/* Clear the Interrupt */
	XGpio_InterruptClear(GpioPtr, 0x00000001);
}
/*********************************************************************
 * ��ʼ��AXI GPIO,����AXI�ж�
 * �������������жϣ����ڼ������ʱ����Դ��������жϺ���AxiGpioHandler
 * ��axi gpio���й���
 *********************************************************************/
int Hgpio_axi_int_init() {
	//printf("��ʼ����AXI GPIO\n");
	u32 Status;
	Status = XGpio_Initialize(&HGpio, XPAR_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	//printf("��ʼ����AXI GPIO:1\n");
	//XScuGic_Config *IntcConfig;
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
	//printf("��ʼ����AXI GPIO:2\n");
	/* Enable the interrupt for the GPIO device.*/
	XScuGic_Enable(&LinIntcInstance, XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR);
	//printf("��ʼ����AXI GPIO:3\n");
	/*
	 * Enable the GPIO channel interrupts so that push button can be
	 * detected and enable interrupts for the GPIO device
	 */
	XGpio_InterruptEnable(&HGpio, 0x00000001);//������һλ���ж�
	printf("��ʼ����AXI GPIO:4\n");
	XGpio_InterruptGlobalEnable(&HGpio);
	printf("��ʼ����AXI GPIO:5\n");
	/*
	 * Initialize the exception table and register the interrupt
	 * controller handler with the exception table
	 */
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler) XScuGic_InterruptHandler, &LinIntcInstance);
	/* Enable non-critical exceptions */
	Xil_ExceptionEnable();
	printf("����AXI GPIO ����\n");
	return XST_SUCCESS;
}
/**
 * ��ʼ��ps��gpio
 * ����1��ps gpioָ�����
 * ����2��ps gpio������
 * ����3������ps gpio�ķ��� 0 input   1��output
 * ����4��ps gpio���豸ID��
 * �����ɹ����� XST_SUCCESS
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
	HgpioPs_SetDir(Gpio,pin,direction);//���÷��� 0 input  1  output
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
*��ȡ���������ƽ
*/
u32 HgpioPs_ReadPin(XGpioPs *Gpio,u32 pin)
{
	return XGpioPs_ReadPin(Gpio, pin);
}
/**
 * ʹ��ָ�����ŵ����
 */
void HgpioPs_EnableOutput(XGpioPs *Gpio,u32 pin)
{
	XGpioPs_SetOutputEnablePin(Gpio, pin, 1);
}
/**
 * �ر�ָ�����ŵ����
 */
void HgpioPs_DisableOutput(XGpioPs *Gpio,u32 pin)
{
	XGpioPs_SetOutputEnablePin(Gpio, pin, 0);
}
/**
 * ����ָ�����ŵ������ƽֵ
 */
void HgpioPs_WritePin(XGpioPs *Gpio,u32 pin,u32 data)
{
	XGpioPs_WritePin(Gpio,pin,data);
}
//PL axi gpio��ʼ��
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

