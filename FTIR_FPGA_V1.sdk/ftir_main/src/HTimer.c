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



static void TTC0_C0_Handler(void *CallBackRef);//�жϺ���
static void TTC0_C1_Handler(void *CallBackRef);//�жϺ���

/******************************************************************************************/
/**
 * ��ʼ��TTC0��ͨ��0�Ķ�ʱ��������ΪPWM���
 * ����1��MatchVale Ϊ���ڲ���PWM�ıȽ�ֵ�ĵ�ַ����ʼ��ʱ���봫��ò������Ա��޸�PWM��ռ�ձ�
 ******************************************************************************************/
int Httc_init_ttc0_timer0(u32 *MatchValue)
{
	int Status;
	XTtcPs_Config *Config;
	u32 Options = 0;
	/*
	 * Look up the configuration based on the device identifier
	 */
	Config = XTtcPs_LookupConfig(XPAR_XTTCPS_0_DEVICE_ID);
	if (NULL == Config) {
		return XST_FAILURE;
	}
	/*
	 * Initialize the device
	 */
	Status = XTtcPs_CfgInitialize(&TTC0_Timer0, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	//Options = XTtcPs_GetOptions(&TTC0_Timer0);//��ȡ��ʱ�����õ�״̬
	//�޸�Ĭ������
	/*
	 * Set up appropriate options for PWM: interval mode  and
	 * match mode for waveform output.
	 */
	//Options |= (XTTCPS_OPTION_INTERVAL_MODE | XTTCPS_OPTION_MATCH_MODE | XTTCPS_OPTION_WAVE_DISABLE);
	Options |= (XTTCPS_OPTION_INTERVAL_MODE | XTTCPS_OPTION_MATCH_MODE);
	/*
	 * Set the options
	 */
	XTtcPs_SetOptions(&TTC0_Timer0, Options);
	/**
	*
	*/
	XTtcPs_SetInterval(&TTC0_Timer0, (u16)76);  //����ֵ��ȷ��PWM������56
	/**
	*���÷�Ƶ
	*/
	XTtcPs_SetPrescaler(&TTC0_Timer0, 2);// 2^x ��Ƶ     XTTCPS_CLK_CNTRL_PS_DISABLE������Ƶ

	XTtcPs_SetMatchValue(&TTC0_Timer0, 0, 25);
	/*
	 * Connect to the interrupt controller
	 */
	Status = XScuGic_Connect(&LinIntcInstance, XPAR_XTTCPS_0_INTR,
		(Xil_ExceptionHandler)TTC0_C0_Handler, (void *)MatchValue);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	//�ж����ȼ�
	//XScuGic_SetPriorityTriggerType(&LinIntcInstance,XPAR_XTTCPS_0_INTR,TTC0Periority, 0b11);
	/*
	 * Enable the interrupt for the Timer counter
	 */
	XScuGic_Enable(&LinIntcInstance, XPAR_XTTCPS_0_INTR);
	XTtcPs_EnableInterrupts(&TTC0_Timer0, XTTCPS_IXR_INTERVAL_MASK);
	XTtcPs_Start(&TTC0_Timer0);
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
static void TTC0_C0_Handler(void *CallBackRef)
{
//	u32 *MatchReg;
	u32 StatusEvent;
	XTtcPs *Timer;

//	MatchReg = (u32 *) CallBackRef;
	Timer = &TTC0_Timer0;

	/*
	 * Read the interrupt status, then write it back to clear the interrupt.
	 */
	StatusEvent = XTtcPs_GetInterruptStatus(Timer);
	XTtcPs_ClearInterruptStatus(Timer, StatusEvent);

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


//	TestCounter++;
//
//	if(TestCounter>1000000)
//	{
//		xil_printf("0\r\n");
//		TestCounter=0;
//	}

	//xil_printf("0\r\n");
	XScuGic_SoftwareIntr(&LinIntcInstance, TTC0SoftIntrIdToCpu1, CPU1);

	//XTtcPs_DisableInterrupts(Timer, XTTCPS_IXR_ALL_MASK);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Httc_init_ttc0_timer1(u32 *MatchValue)
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
	Status = XTtcPs_CfgInitialize(&TTC0_Timer1, Config, Config->BaseAddress);
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
	XTtcPs_SetOptions(&TTC0_Timer1, Options);
	/**
	*
	*/
	XTtcPs_SetInterval(&TTC0_Timer1, (u16)56);  //����ֵ��ȷ��PWM������56
	/**
	*���÷�Ƶ
	*/
	XTtcPs_SetPrescaler(&TTC0_Timer1, 4);// 2^x ��Ƶ     XTTCPS_CLK_CNTRL_PS_DISABLE������Ƶ

	XTtcPs_SetMatchValue(&TTC0_Timer1, 0, 25);
	/*
	 * Connect to the interrupt controller
	 */
	Status = XScuGic_Connect(&LinIntcInstance, XPAR_XTTCPS_1_INTR,
		(Xil_ExceptionHandler)TTC0_C1_Handler, (void *)MatchValue);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	//�ж����ȼ�
	//XScuGic_SetPriorityTriggerType(&LinIntcInstance,XPAR_XTTCPS_1_INTR,TTC1Periority, 0b11);

	/*
	 * Enable the interrupt for the Timer counter
	 */
	XScuGic_Enable(&LinIntcInstance, XPAR_XTTCPS_1_INTR);
	XTtcPs_EnableInterrupts(&TTC0_Timer1, XTTCPS_IXR_INTERVAL_MASK);
	XTtcPs_Start(&TTC0_Timer1);
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
static void TTC0_C1_Handler(void *CallBackRef)
{
//	u32 *MatchReg;
	u32 StatusEvent;
	XTtcPs *Timer;

	//MatchReg = (u32 *) CallBackRef;
	Timer = &TTC0_Timer1;

	/*
	 * Read the interrupt status, then write it back to clear the interrupt.
	 */
	StatusEvent = XTtcPs_GetInterruptStatus(Timer);
	XTtcPs_ClearInterruptStatus(Timer, StatusEvent);

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
	//xil_printf("1\r\n");
	XScuGic_SoftwareIntr(&LinIntcInstance, TTC1SoftIntrIdToCpu1, CPU1);

	//XTtcPs_DisableInterrupts(Timer, XTTCPS_IXR_ALL_MASK);
}




