/*
 * Haxi_pwm.c
 *
 *  Created on: 2019年9月17日
 *      Author: hpy
 */
#include "Haxi_pwm.h"
#include "xparameters.h"
#include "xil_io.h"
#include "sleep.h"

static u32 Option_inst = 0U;
/**************************************************************************************
 *						 配置Haxi_pwm参数，工作模式
 * 	------------------------------------------------------------------------------
 |  位置   |   [31]    |  [30:28] |   [27:25]   |  [2:0]  |
 ------------------------------------------------------------------------------
 |         |时钟选择   |输出使能    |无PWM输出的  |方向信号  |
 |         |0:AXI时钟 |1:输出使能|默认电平[0/1]|        |
 | 功能说明 |1:外部输入|30:通道2  |27:通道2    |2：通道2 |
 |         |时钟           |29:通道1  |26:通道1    |1：通道1 |
 |         |          |28:通道0  |25:通道0    |0：通道0 |
 ------------------------------------------------------------------------------
 |  默认值  |     0    |   000    |     000    |   000   |
 ------------------------------------------------------------------------------
 *参数1位即将写入的option参数
 *参数2为写入模式，0使用的是写入什么就是什么  其他值则是写入和之前的值的或值
 *
 **************************************************************************************/
void Haxi_pwm_SetOption(u32 op, int mod) {
	if (mod == 0) {
		Option_inst = op;
	} else {
		Option_inst |= op;
	}
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG6_OFFSET, Option_inst);
}
/**************************************************
 * 初始化AXI PWM模块
 **************************************************/
void Haxi_pwm_init(void) {
	Haxi_pwm_SetOption(0u, 0); //恢复默认值
	Haxi_pwm_SetFeq0(20000U); //设置周期
	Haxi_pwm_SetHTime0(10000U); //设置高电平时间

	//Haxi_pwm_SetFeq1(100000000U); //设置周期
	//Haxi_pwm_SetHTime1(50000000U); //设置高电平时间
	Haxi_pwm_SetOption(
			HAXI_PWM_EN_PWM0 | HAXI_PWM_EN_PWM0 | HAXI_PWM_EXT_CLK
					| HAXI_PWM_DEFAULT_PWM2 | HAXI_PWM_DEFAULT_PWM1
					| HAXI_PWM_DEFAULT_PWM0, 1);	//使能通道0 PWM，使用外部时钟
}
/*********************************************************************************
 * 获得硬件中实际的option设置值,同时更新软件存储的option值
 *********************************************************************************/
u32 Haxi_pwm_GetOption(void) {
	Option_inst = FPGA_PWM_mReadReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG6_OFFSET);
	return Option_inst;
}
/***************************************************************
 * 开启通道0 PWM输出
 ***************************************************************/
void Haxi_pwm_StartC0(void) {
	Haxi_pwm_SetOption(HAXI_PWM_EN_PWM0, 1);
}
/***************************************************************
 * 开启通道1 PWM输出
 ***************************************************************/
void Haxi_pwm_StartC1(void) {
	Haxi_pwm_SetOption(HAXI_PWM_EN_PWM1, 1);
}
/***************************************************************
 * 开启通道2 PWM输出
 ***************************************************************/
void Haxi_pwm_StartC2(void) {
	Haxi_pwm_SetOption(HAXI_PWM_EN_PWM2, 1);
}
/***************************************************************
 * 关闭通道0 PWM输出
 ***************************************************************/
void Haxi_pwm_CloseC0(void) {
	u32 op = ~HAXI_PWM_EN_PWM0;
	u32 op1 = FPGA_PWM_mReadReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG6_OFFSET);
	Haxi_pwm_SetOption((op & op1), 0);
}
/***************************************************************
 * 关闭通道1 PWM输出
 ***************************************************************/
void Haxi_pwm_CloseC1(void) {
	u32 op = ~HAXI_PWM_EN_PWM1;
	u32 op1 = FPGA_PWM_mReadReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG6_OFFSET);
	Haxi_pwm_SetOption((op & op1), 0);
}
/***************************************************************
 * 关闭通道2 PWM输出
 ***************************************************************/
void Haxi_pwm_CloseC2(void) {
	u32 op = ~HAXI_PWM_EN_PWM2;
	u32 op1 = FPGA_PWM_mReadReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG6_OFFSET);
	Haxi_pwm_SetOption((op & op1), 0);
}
/****************************************************************
 * 设置通道0 PWM周期/频率
 ****************************************************************/
void Haxi_pwm_SetFeq0(u32 Tcnt) {
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG0_OFFSET, Tcnt);
}
/****************************************************************
 * 设置通道1 PWM周期/频率
 ****************************************************************/
void Haxi_pwm_SetFeq1(u32 Tcnt) {
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG2_OFFSET, Tcnt);
}
/****************************************************************
 * 设置通道2 PWM周期/频率
 ****************************************************************/
void Haxi_pwm_SetFeq2(u32 Tcnt) {
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG4_OFFSET, Tcnt);
}
/****************************************************************
 * 设置通道0 PWM高电平持续时间
 ****************************************************************/
void Haxi_pwm_SetHTime0(u32 hcnt) {
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG1_OFFSET, hcnt);
}
/****************************************************************
 * 设置通道1 PWM高电平持续时间
 ****************************************************************/
void Haxi_pwm_SetHTime1(u32 hcnt) {
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG3_OFFSET, hcnt);
}
/****************************************************************
 * 设置通道2 PWM高电平持续时间
 ****************************************************************/
void Haxi_pwm_SetHTime2(u32 hcnt) {
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG5_OFFSET, hcnt);
}

/***************************************************************
 * 设置通道0辅助引脚电平 用于控制方向
 ***************************************************************/
void Haxi_pwm_SetDir0(int dir) {
	if (dir == 0) {
		Option_inst &= 0xFFFFFFFE;
	} else {
		Option_inst |= 0x00000001;
	}
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG6_OFFSET, Option_inst);
}
/***************************************************************
 * 设置通道1辅助引脚电平 用于控制方向
 ***************************************************************/
void Haxi_pwm_SetDir1(int dir) {
	if (dir == 0) {
		Option_inst &= 0xFFFFFFFD;
	} else {
		Option_inst |= 0x00000002;
	}
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG6_OFFSET, Option_inst);
}
/***************************************************************
 * 设置通道2辅助引脚电平 用于控制方向
 ***************************************************************/
void Haxi_pwm_SetDir2(int dir) {
	if (dir == 0) {
		Option_inst &= 0xFFFFFFFB;
	} else {
		Option_inst |= 0x00000004;
	}
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG6_OFFSET, Option_inst);
}
/***************************************************************
 * 关闭所有PWM输出，准备退出
 ***************************************************************/
void Haxi_pwm_clean(void) {
	Haxi_pwm_CloseC0();
	Haxi_pwm_CloseC1();
	Haxi_pwm_CloseC2();
}
