/*
 * Httc.c
 * 该模块用于产生PWM或者作为Tick定时器
 *  Created on: 2019年6月30日
 *      Author: hpy
 */

#include "system.h"
#include "xparameters.h"
/**
 * TTC定时器实例声明
 */
XTtcPs TTC0_Timer0;
XTtcPs TTC0_Timer1;
XTtcPs TTC0_Timer2;

XTtcPs TTC1_Timer0;
XTtcPs TTC1_Timer1;
XTtcPs TTC1_Timer2;

//XScuGic InterruptController;//中断控制器



static void TTC0_C0_Handler(void *CallBackRef);//中断函数
static void TTC0_C1_Handler(void *CallBackRef);//中断函数

/******************************************************************************************/
/**
 * 初始化TTC0的通道0的定时器，设置为PWM输出
 * 参数1：MatchVale 为用于产生PWM的比较值的地址，初始化时必须传入该参数，以便修改PWM的占空比
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

	//Options = XTtcPs_GetOptions(&TTC0_Timer0);//获取定时器设置的状态
	//修改默认设置
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
	XTtcPs_SetInterval(&TTC0_Timer0, (u16)76);  //计数值，确定PWM的周期56
	/**
	*设置分频
	*/
	XTtcPs_SetPrescaler(&TTC0_Timer0, 2);// 2^x 分频     XTTCPS_CLK_CNTRL_PS_DISABLE；不分频

	XTtcPs_SetMatchValue(&TTC0_Timer0, 0, 25);
	/*
	 * Connect to the interrupt controller
	 */
	Status = XScuGic_Connect(&LinIntcInstance, XPAR_XTTCPS_0_INTR,
		(Xil_ExceptionHandler)TTC0_C0_Handler, (void *)MatchValue);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	//中断优先级
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
//		xil_printf("中断错误\r\n");
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

	//Options = XTtcPs_GetOptions(&TTC0_Timer0);//获取定时器设置的状态
	//修改默认设置
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
	XTtcPs_SetInterval(&TTC0_Timer1, (u16)56);  //计数值，确定PWM的周期56
	/**
	*设置分频
	*/
	XTtcPs_SetPrescaler(&TTC0_Timer1, 4);// 2^x 分频     XTTCPS_CLK_CNTRL_PS_DISABLE；不分频

	XTtcPs_SetMatchValue(&TTC0_Timer1, 0, 25);
	/*
	 * Connect to the interrupt controller
	 */
	Status = XScuGic_Connect(&LinIntcInstance, XPAR_XTTCPS_1_INTR,
		(Xil_ExceptionHandler)TTC0_C1_Handler, (void *)MatchValue);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	//中断优先级
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
//		xil_printf("中断错误\r\n");
//	}
	//xil_printf("1\r\n");
	XScuGic_SoftwareIntr(&LinIntcInstance, TTC1SoftIntrIdToCpu1, CPU1);

	//XTtcPs_DisableInterrupts(Timer, XTTCPS_IXR_ALL_MASK);
}




