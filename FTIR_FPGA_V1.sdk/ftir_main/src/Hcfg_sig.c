/*
 * Hcfg_sig.c
 *
 *  Created on: 2019年10月22日
 *      Author: hpy
 */

#include "Hcfg_sig.h"
#include "xparameters.h"
#include "xil_io.h"
#include "sleep.h"
#include "system.h"
#include "H_IRQ.h"


#define ICDICPR0 (0xf8f01280)
#define ICDICPR1 (0xf8f01284)
#define ICDICPR2 (0xf8f01288)





static void IRQ62_LASER(void *CallbackRef)
{
	u32 it_flag = Xil_In32(ICDICPR1);
	it_flag |= (1u<<30);
	Xil_Out32(ICDICPR1,it_flag);

	//printf("进入62号激光中断 \r\n");
	XScuGic_SoftwareIntr(&LinIntcInstance, LaserSoftIntrIdToCpu1, CPU1) ;
}

/**
 * 光电开关中断处理函数
 */
static void IRQ63_SWITCH(void *CallbackRef)
{
	u32 it_flag = Xil_In32(ICDICPR1);
	it_flag |= (1u<<31);
	Xil_Out32(ICDICPR1,it_flag);
	//TestCounter++;
	//printf("进入63号光电开关中断 \r\n");
	XScuGic_SoftwareIntr(&LinIntcInstance, ZeroCheckIntrIdToCpu1, CPU1) ;
}




void Hcfg_sig_Write(u32 reg, u32 data) {
	switch (reg) {
	case (0):
		AXI_SIG_CFG_mWriteReg(XPAR_AXI_SIG_CFG_0_S00_AXI_BASEADDR,
				AXI_SIG_CFG_S00_AXI_SLV_REG0_OFFSET, data);
		break;
	case (1):
		AXI_SIG_CFG_mWriteReg(XPAR_AXI_SIG_CFG_0_S00_AXI_BASEADDR,
				AXI_SIG_CFG_S00_AXI_SLV_REG1_OFFSET, data);
		break;
	case (2):
		AXI_SIG_CFG_mWriteReg(XPAR_AXI_SIG_CFG_0_S00_AXI_BASEADDR,
				AXI_SIG_CFG_S00_AXI_SLV_REG2_OFFSET, data);
		break;
	case (3):
		AXI_SIG_CFG_mWriteReg(XPAR_AXI_SIG_CFG_0_S00_AXI_BASEADDR,
				AXI_SIG_CFG_S00_AXI_SLV_REG3_OFFSET, data);
		break;
	default: break;
	}
}

void Hcfg_sig_init() {
	Hcfg_sig_Write(0,0x40000000);
	Hcfg_sig_Write(1,10u);//设置触发电平宽度
	XScuGic_Connect(&LinIntcInstance, 62,(Xil_InterruptHandler)IRQ62_LASER, NULL);
	XScuGic_SetPriorityTriggerType(&LinIntcInstance,62,LaserPeriority, 0b11);//激光脉冲上升沿触发
	//XScuGic_Enable(&LinIntcInstance,62);


	XScuGic_Connect(&LinIntcInstance, 63,(Xil_InterruptHandler)IRQ63_SWITCH, NULL);
	XScuGic_SetPriorityTriggerType(&LinIntcInstance,63,ZeroCheckPeriority, 0b11);//光电开关脉冲上升沿触发
	//XScuGic_Enable(&LinIntcInstance,63);
}
void Hcfg_EnableLaserInt()
{
	XScuGic_Enable(&LinIntcInstance,62);
}
void Hcfg_DisableLaserInt(){
	XScuGic_Disable(&LinIntcInstance,62);
}

void Hcfg_EnableSwitchInt()
{
	XScuGic_Enable(&LinIntcInstance,63);
}
void Hcfg_DisableSwitchInt()
{
	XScuGic_Disable(&LinIntcInstance,63);
}
void dis_info()
{
	u32 info = 	AXI_SIG_CFG_mReadReg(XPAR_AXI_SIG_CFG_0_S00_AXI_BASEADDR,
			AXI_SIG_CFG_S00_AXI_SLV_REG3_OFFSET);
	u32 s = info>>31;
	u32 o = (info >> 30)&1u;
	//u32 sfg =
	printf("in = %u    o=%u \r\n",s,o);
}

