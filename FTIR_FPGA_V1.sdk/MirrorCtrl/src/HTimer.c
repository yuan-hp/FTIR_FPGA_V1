/*
 * Httc.c
 * ��ģ�����ڲ���PWM������ΪTick��ʱ��
 *  Created on: 2019��6��30��
 *      Author: hpy
 */

#include "system.h"
#include "xparameters.h"
/**
 * TTC��ʱ��ʵ������
 */
XTtcPs TTC0_Timer0;
XTtcPs TTC0_Timer1;
XTtcPs TTC0_Timer2;

XTtcPs TTC1_Timer0;
XTtcPs TTC1_Timer1;
XTtcPs TTC1_Timer2;

//XScuGic InterruptController;//�жϿ�����



static void TTC1_C0_Handler(void *CallBackRef);//�жϺ���
/****************************************************************************/
/**
*
* This function setups the interrupt system such that interrupts can occur.
* This function is application specific since the actual system may or may not
* have an interrupt controller.  The TTC could be directly connected to a
* processor without an interrupt controller.  The user should modify this
* function to fit the application.
*
* @param	IntcDeviceID is the unique ID of the interrupt controller
* @param	IntcInstacePtr is a pointer to the interrupt controller
*		instance.
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None.
*
*****************************************************************************/

static int SetupInterruptSystem(u16 IntcDeviceID,
				    XScuGic *IntcInstancePtr)
{
	int Status;
	XScuGic_Config *IntcConfig; /* The configuration parameters of the
					interrupt controller */

	/*
	 * Initialize the interrupt controller driver
	 */
	IntcConfig = XScuGic_LookupConfig(IntcDeviceID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the ARM processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler) XScuGic_InterruptHandler,
			IntcInstancePtr);

	/*
	 * Enable interrupts in the ARM
	 */
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}
/******************************************************************************************/
/**
 * ��ʼ��TTC0��ͨ��0�Ķ�ʱ��������ΪPWM���
 * ����1��MatchVale Ϊ���ڲ���PWM�ıȽ�ֵ�ĵ�ַ����ʼ��ʱ���봫��ò������Ա��޸�PWM��ռ�ձ�
 ******************************************************************************************/
//int Httc_init_ttc0_timer0(u32 *MatchValue)
//{
//	int Status;
//	XTtcPs_Config *Config;
//	u32 Options = 0;
//	/*
//	 * Look up the configuration based on the device identifier
//	 */
//	Config = XTtcPs_LookupConfig(XPAR_XTTCPS_0_DEVICE_ID);
//	if (NULL == Config) {
//		return XST_FAILURE;
//	}
//	/*
//	 * Initialize the device
//	 */
//	Status = XTtcPs_CfgInitialize(&TTC0_Timer0, Config, Config->BaseAddress);
//	if (Status != XST_SUCCESS) {
//		return XST_FAILURE;
//	}
//
//	//Options = XTtcPs_GetOptions(&TTC0_Timer0);//��ȡ��ʱ�����õ�״̬
//	//�޸�Ĭ������
//	/*
//	 * Set up appropriate options for PWM: interval mode  and
//	 * match mode for waveform output.
//	 */
//	//Options |= (XTTCPS_OPTION_INTERVAL_MODE | XTTCPS_OPTION_MATCH_MODE | XTTCPS_OPTION_WAVE_DISABLE);
//	Options |= (XTTCPS_OPTION_INTERVAL_MODE | XTTCPS_OPTION_MATCH_MODE);
//	/*
//	 * Set the options
//	 */
//	XTtcPs_SetOptions(&TTC0_Timer0, Options);
//	/**
//	*
//	*/
//	XTtcPs_SetInterval(&TTC0_Timer0, (u16)65535);  //����ֵ��ȷ��PWM������
//	/**
//	*���÷�Ƶ
//	*/
//	XTtcPs_SetPrescaler(&TTC0_Timer0, 4);// 2^x ��Ƶ     XTTCPS_CLK_CNTRL_PS_DISABLE������Ƶ
//
//
//	XTtcPs_SetMatchValue(&TTC0_Timer0, 0, 32768);
//
//	/*�ж�����
//	 * Connect the Intc to the interrupt subsystem such that interrupts can
//	 * occur. This function is application specific.
//	 */
////	Status = SetupInterruptSystem(XPAR_SCUGIC_SINGLE_DEVICE_ID,&LinIntcInstance);//�����жϿ�����
////	if (Status != XST_SUCCESS) {
////		return XST_FAILURE;
////	}
//
//	/*
//	 * Connect to the interrupt controller
//	 */
//	Status = XScuGic_Connect(&LinIntcInstance, XPAR_XTTCPS_0_INTR,
//		(Xil_ExceptionHandler)TTC0_C0_Handler, (void *)MatchValue);
//	if (Status != XST_SUCCESS) {
//		return XST_FAILURE;
//	}
//	/*
//	 * Enable the interrupt for the Timer counter
//	 */
//	XScuGic_Enable(&LinIntcInstance, XPAR_XTTCPS_0_INTR);
//	XTtcPs_EnableInterrupts(&TTC0_Timer0, XTTCPS_IXR_INTERVAL_MASK);
//	XTtcPs_Start(&TTC0_Timer0);
//	return XST_SUCCESS;
//}
//int Httc_init_ttc0_timer0(u32 *MatchValue)
//{
//	int Status;
//	XTtcPs_Config *Config;
//	u32 Options = 0;
//	/*
//	 * Look up the configuration based on the device identifier
//	 */
//	Config = XTtcPs_LookupConfig(XPAR_XTTCPS_0_DEVICE_ID);
//	if (NULL == Config) {
//		return XST_FAILURE;
//	}
//	/*
//	 * Initialize the device
//	 */
//	Status = XTtcPs_CfgInitialize(&TTC0_Timer0, Config, Config->BaseAddress);
//	if (Status != XST_SUCCESS) {
//		return XST_FAILURE;
//	}
//
//	//Options = XTtcPs_GetOptions(&TTC0_Timer0);//��ȡ��ʱ�����õ�״̬
//	//�޸�Ĭ������
//	/*
//	 * Set up appropriate options for PWM: interval mode  and
//	 * match mode for waveform output.
//	 */
//	//Options |= (XTTCPS_OPTION_INTERVAL_MODE | XTTCPS_OPTION_MATCH_MODE | XTTCPS_OPTION_WAVE_DISABLE);
//	Options |= (XTTCPS_OPTION_INTERVAL_MODE | XTTCPS_OPTION_MATCH_MODE);
//	/*
//	 * Set the options
//	 */
//	XTtcPs_SetOptions(&TTC0_Timer0, Options);
//	/**
//	*
//	*/
//	XTtcPs_SetInterval(&TTC0_Timer0, (u16)65535);  //����ֵ��ȷ��PWM������
//	/**
//	*���÷�Ƶ
//	*/
//	XTtcPs_SetPrescaler(&TTC0_Timer0, 4);// 2^x ��Ƶ     XTTCPS_CLK_CNTRL_PS_DISABLE������Ƶ
//
//
//	XTtcPs_SetMatchValue(&TTC0_Timer0, 0, 32768);
//
//	/*�ж�����
//	 * Connect the Intc to the interrupt subsystem such that interrupts can
//	 * occur. This function is application specific.
//	 */
////	Status = SetupInterruptSystem(XPAR_SCUGIC_SINGLE_DEVICE_ID,&LinIntcInstance);//�����жϿ�����
////	if (Status != XST_SUCCESS) {
////		return XST_FAILURE;
////	}
//
//	/*
//	 * Connect to the interrupt controller
//	 */
//	Status = XScuGic_Connect(&LinIntcInstance, XPAR_XTTCPS_3_INTR,
//		(Xil_ExceptionHandler)TTC0_C0_Handler, (void *)MatchValue);
//	if (Status != XST_SUCCESS) {
//		return XST_FAILURE;
//	}
//	/*
//	 * Enable the interrupt for the Timer counter
//	 */
//	XScuGic_Enable(&LinIntcInstance, XPAR_XTTCPS_3_INTR);
//	XTtcPs_EnableInterrupts(&TTC0_Timer0, XTTCPS_IXR_INTERVAL_MASK);
//	XTtcPs_Start(&TTC0_Timer0);
//	return XST_SUCCESS;
//}
int Httc_init_ttc1_timer0(u32 *MatchValue)
{
	int Status;
	XTtcPs_Config *Config;
	u32 Options = 0;
	/*
	 * Look up the configuration based on the device identifier
	 */
	Config = XTtcPs_LookupConfig(XPAR_XTTCPS_1_DEVICE_ID);
	if (NULL == Config) {
		return XST_FAILURE;
	}
	/*
	 * Initialize the device
	 */
	Status = XTtcPs_CfgInitialize(&TTC1_Timer0, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	//Options = XTtcPs_GetOptions(&TTC0_Timer0);//��ȡ��ʱ�����õ�״̬
	//�޸�Ĭ������
	/*
	 * Set up appropriate options for PWM: interval mode  and
	 * match mode for waveform output.
	 */
	Options |= (XTTCPS_OPTION_INTERVAL_MODE | XTTCPS_OPTION_MATCH_MODE | XTTCPS_OPTION_WAVE_DISABLE);
	//Options |= (XTTCPS_OPTION_INTERVAL_MODE | XTTCPS_OPTION_MATCH_MODE);
	/*
	 * Set the options
	 */
	XTtcPs_SetOptions(&TTC1_Timer0, Options);
	/**
	*
	*/
	XTtcPs_SetInterval(&TTC1_Timer0, (u16)65535);  //����ֵ��ȷ��PWM������
	/**
	*���÷�Ƶ
	*/
	XTtcPs_SetPrescaler(&TTC1_Timer0, 4);// 2^x ��Ƶ     XTTCPS_CLK_CNTRL_PS_DISABLE������Ƶ


	XTtcPs_SetMatchValue(&TTC1_Timer0, 0, 32768);

	/*�ж�����
	 * Connect the Intc to the interrupt subsystem such that interrupts can
	 * occur. This function is application specific.
	 */
	Status = SetupInterruptSystem(XPAR_SCUGIC_SINGLE_DEVICE_ID,&LinIntcInstance);//�����жϿ�����
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect to the interrupt controller
	 */
	Status = XScuGic_Connect(&LinIntcInstance, XPAR_XTTCPS_3_INTR,
		(Xil_ExceptionHandler)TTC1_C0_Handler, (void *)MatchValue);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	/*
	 * Enable the interrupt for the Timer counter
	 */
	XScuGic_Enable(&LinIntcInstance, XPAR_XTTCPS_3_INTR);
	XTtcPs_EnableInterrupts(&TTC1_Timer0, XTTCPS_IXR_INTERVAL_MASK);
	XTtcPs_Start(&TTC1_Timer0);
	return XST_SUCCESS;
}

/***************************************************************************/
/**
*
* This function is the handler which handles the PWM interrupt.
*
* It updates the match register to reflect the change on duty cycle. It also
* disable interrupt at the end. The interrupt will be enabled by the Ticker
* timer counter.
*
* @param	CallBackRef contains a callback reference from the driver, in
*		this case it is a pointer to the MatchValue variable.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
//static void TTC0_C0_Handler(void *CallBackRef)
//{
//	u32 *MatchReg;
//	u32 StatusEvent;
//	XTtcPs *Timer;
//
//	MatchReg = (u32 *) CallBackRef;
//	Timer = &TTC0_Timer0;
//
//	/*
//	 * Read the interrupt status, then write it back to clear the interrupt.
//	 */
//	StatusEvent = XTtcPs_GetInterruptStatus(Timer);
//	XTtcPs_ClearInterruptStatus(Timer, StatusEvent);
//
//	if (0 != (XTTCPS_IXR_INTERVAL_MASK & StatusEvent)) {
//		XTtcPs_SetMatchValue(Timer, 0, *MatchReg);
//	}
//	else {
//		/*
//		 * If it is not Interval event, it is an error.
//		 */
//		//ErrorCount++;
//		xil_printf("�жϴ���\r\n");
//	}
//
////	if(PCNeedDataFlag==1)
////	{
////		TaskManager();
////		ADDataAcquire();//��ʱ��ȡad����
////	}
//
//	xil_printf("hhhhhhhhh\r\n");
//
//	//XTtcPs_DisableInterrupts(Timer, XTTCPS_IXR_ALL_MASK);
//}
static void TTC1_C0_Handler(void *CallBackRef)
{
	u32 *MatchReg;
	u32 StatusEvent;
	XTtcPs *Timer;

	MatchReg = (u32 *) CallBackRef;
	Timer = &TTC1_Timer0;

	/*
	 * Read the interrupt status, then write it back to clear the interrupt.
	 */
	StatusEvent = XTtcPs_GetInterruptStatus(Timer);
	XTtcPs_ClearInterruptStatus(Timer, StatusEvent);

	if (0 != (XTTCPS_IXR_INTERVAL_MASK & StatusEvent)) {
		XTtcPs_SetMatchValue(Timer, 0, *MatchReg);
	}
	else {
		/*
		 * If it is not Interval event, it is an error.
		 */
		//ErrorCount++;
		xil_printf("�жϴ���\r\n");
	}

//	if(PCNeedDataFlag==1)
//	{
//		TaskManager();
//		ADDataAcquire();//��ʱ��ȡad����
//	}

	xil_printf("hhhhhhhhh\r\n");

	//XTtcPs_DisableInterrupts(Timer, XTTCPS_IXR_ALL_MASK);
}
/****************************************************************************/
/**
 * ��ȡָ��TTC��ʱ����Ӧͨ����ʵ�����Ա��������������޸Ķ�ʱ�Ĵ�����һЩֵ
 * ����1��TTCn��ָ����ʱ����TTC0����TTC1��TTC0:0   TTC1��1
 * ����2����ָ��TTC���ĸ�ͨ��  0-2
 *
 * ����ֵ�������ɹ����ض�Ӧʵ���ĵ�ַ�����򷵻ؿյ�ַ
 ***************************************************************************/
XTtcPs *Httc_GetTimerInst(int TTCn,int channel)
{
	if(TTCn == 0)//��ȡTTC0��ʱ����ʵ��
	{
		switch(channel)
		{
			case(0):return &TTC0_Timer0;break;
			case(1):return &TTC0_Timer1;break;
			case(2):return &TTC0_Timer2;break;
			default:return NULL;
		}
	}
	else if(TTCn == 1)
	{
		switch(channel)
		{
			case(0):return &TTC1_Timer0;break;
			case(1):return &TTC1_Timer1;break;
			case(2):return &TTC1_Timer2;break;
			default:return NULL;
		}
	}
	return NULL;
}
/****************************************************************************/
/**
 * ����ָ��TTC��ʱ����Ӧͨ����Interval��
 * ����1��TTCn��ָ����ʱ����TTC0����TTC1��TTC0:0   TTC1��1
 * ����2����ָ��TTC���ĸ�ͨ��  0-2
 * ����3�����õ�ֵ
 * ����ֵ����
 ***************************************************************************/
void Httc_SetInterval(int TTCn , int channel , u16 value)
{
	if(TTCn == 0)//��ȡTTC0��ʱ����ʵ��
	{
		switch(channel)
		{
			case(0):XTtcPs_SetInterval(&TTC0_Timer0, value);break;
			case(1):XTtcPs_SetInterval(&TTC0_Timer1, value);break;
			case(2):XTtcPs_SetInterval(&TTC0_Timer2, value);break;
			default: ;
		}
	}
	else if(TTCn == 1)
	{
		switch(channel)
		{
			case(0):XTtcPs_SetInterval(&TTC1_Timer0, value);break;
			case(1):XTtcPs_SetInterval(&TTC1_Timer1, value);break;
			case(2):XTtcPs_SetInterval(&TTC1_Timer2, value);break;
			default: ;
		}
	}
}
