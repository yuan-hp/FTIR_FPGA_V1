/*
 * Hmfreq.c
 *
 *  Created on: 2019年9月30日
 *      Author: hpy
 */

#include "Hmfreq.h"
#include "xparameters.h"
#include "xil_io.h"
#include "sleep.h"

static u32 ctl = 0U; //控制寄存器的值

/**********************************************************
 * 写寄存器函数
 * regid:寄存器编号  0 1 2 3
 * data:写入的数据
 **********************************************************/
void Hmfreq_Write(int regid, u32 data) {
	switch (regid) {
	case (0):
		FREQMEASURE_mWriteReg(XPAR_FREQMEASURE_0_S00_AXI_BASEADDR,
				FREQMEASURE_S00_AXI_SLV_REG0_OFFSET, data);
		break;
	case (1):
		FREQMEASURE_mWriteReg(XPAR_FREQMEASURE_0_S00_AXI_BASEADDR,
				FREQMEASURE_S00_AXI_SLV_REG1_OFFSET, data);
		break;
	case (2):
		FREQMEASURE_mWriteReg(XPAR_FREQMEASURE_0_S00_AXI_BASEADDR,
				FREQMEASURE_S00_AXI_SLV_REG2_OFFSET, data);
		break;
	case (3):
		FREQMEASURE_mWriteReg(XPAR_FREQMEASURE_0_S00_AXI_BASEADDR,
				FREQMEASURE_S00_AXI_SLV_REG3_OFFSET, data);
		break;
	default:
		break;
	}
}
/**********************************************************
 * 读寄存器函数
 * regid:寄存器编号  0 1 2 3
 **********************************************************/
u32 Hmfreq_Read(int regid) {
	switch (regid) {
	case (0):
		return FREQMEASURE_mReadReg(XPAR_FREQMEASURE_0_S00_AXI_BASEADDR,
				FREQMEASURE_S00_AXI_SLV_REG0_OFFSET);
		break;
	case (1):
		return FREQMEASURE_mReadReg(XPAR_FREQMEASURE_0_S00_AXI_BASEADDR,
				FREQMEASURE_S00_AXI_SLV_REG1_OFFSET);
		break;
	case (2):
		return FREQMEASURE_mReadReg(XPAR_FREQMEASURE_0_S00_AXI_BASEADDR,
				FREQMEASURE_S00_AXI_SLV_REG2_OFFSET);
		break;
	case (3):
		return FREQMEASURE_mReadReg(XPAR_FREQMEASURE_0_S00_AXI_BASEADDR,
				FREQMEASURE_S00_AXI_SLV_REG3_OFFSET);
		break;
	default:
		return 0;
	}
}
/**********************************************************
 * 初始化频率/周期测量模块，
 *
 *********************************************************/
void Hmfreq_init() {
	//Hmfreq_RisingMea(); //利用上升沿测量周期
	Hmfreq_FallingMea();
	Hmfreq_SetPsCheckMoveDir(); //设置PS检测动镜方向
	Hmfreq_SetClkDiv(1); //设置分频系数，获得测量的基础时钟，若该值设置为0，将会关闭测量功能
	//Hmfreq_StartTestSelf(); //开启自测模式
}
/**********************************************************
 *初始化频率/周期测量模块，
 *使用硬件PL部分检测动镜移动方向
 *********************************************************/
void Hmfreq_pl_init() {
	Hmfreq_RisingMea(); //利用上升沿测量周期
	Hmfreq_SetPlCheckMoveDir(); //设置PL检测动镜方向
	Hmfreq_SetClkDiv(1U); //设置分频系数，获得测量的基础时钟，若该值设置为0，将会关闭测量功能
	Hmfreq_SetPLMoveChangeTh(4U); //设置硬件处理加减速的阈值
	Hmfreq_SetPLMoveChangeBufSize(2U); //设置产生触发脉冲的缓存值
	Hmfreq_SetPLCheckDirPolar(HMFREQ_LOW);
	Hmfreq_StartPLGetMoveDir(); //启动硬件检测动镜方向
}
/*********************************************************
 * 设置检测脉冲信号上升沿
 * 使用上升沿沿触发周期测量
 *********************************************************/
void Hmfreq_RisingMea() {
	ctl &= (~HMFREQ_CTL_FALLING_CHECK);
	Hmfreq_Write(0, ctl);
}
/*********************************************************
 * 设置检测脉冲信号下降沿沿
 * 使用上升沿沿触发周期测量
 *********************************************************/
void Hmfreq_FallingMea() {
	ctl |= (HMFREQ_CTL_FALLING_CHECK);
	Hmfreq_Write(0, ctl);
}
/*********************************************************
 * 设置动镜方向检测使用PL部分生成
 *********************************************************/
void Hmfreq_SetPlCheckMoveDir() {
	ctl |= HMFREQ_CTL_PL_SET_MOV_DIR;
	Hmfreq_Write(0, ctl);
}
/*********************************************************
 * 设置动镜方向检测使用PS部分生成
 *********************************************************/
void Hmfreq_SetPsCheckMoveDir() {
	ctl &= (~HMFREQ_CTL_PL_SET_MOV_DIR);
	Hmfreq_Write(0, ctl);
}
/*********************************************************
 * ps(软件)检测到动镜方向改变后，将代表方向的电平传入PL部分使用
 * 该函数只有在使用PS进行动镜检测时才有效，使用PL检测动镜方向
 * 将会忽略该设置
 * ttl  HMFREQ_HIGH:设置为高电平
 * ttl  HMFREQ_LOW:设置为低电平
 *********************************************************/
void Hmfreq_PsPushMoveDir2PL(int ttl) {
	if (HMFREQ_HIGH == ttl) {
		ctl |= HMFREQ_CTL_DIR_HIGH;
		Hmfreq_Write(0, ctl);
	} else if (HMFREQ_LOW == ttl) {
		ctl &= (~HMFREQ_CTL_DIR_HIGH);
		Hmfreq_Write(0, ctl);
	}
}
/*********************************************************
 * ps设置pl检测动镜移动方向的电平极性，该函数只有在使用PL检测
 * 动镜方向，且在硬件pl处于关闭动镜方向检测后在重新启动pl检测
 * 才会生效。
 * ttl  HMFREQ_HIGH:初始值为1
 * ttl  HMFREQ_LOW:初始值为0
 *********************************************************/
void Hmfreq_SetPLCheckDirPolar(int ttl) {
	if (HMFREQ_HIGH == ttl) {
		ctl |= HMFREQ_CTL_PL_CHECK_DIR_POLAR;
		Hmfreq_Write(0, ctl);
	} else if (HMFREQ_LOW == ttl) {
		ctl &= (~HMFREQ_CTL_PL_CHECK_DIR_POLAR);
		Hmfreq_Write(0, ctl);
	}
}
/*********************************************************
 * 启动PL获取动镜云顶方向
 *********************************************************/
void Hmfreq_StartPLGetMoveDir() {
	ctl |= HMFREQ_CTL_ENABLE_PL_GET_SPEED;
	Hmfreq_Write(0, ctl);
}
/*********************************************************
 * 关闭PL获取动镜云顶方向
 *********************************************************/
void Hmfreq_ClosePLGetMoveDir() {
	ctl &= (~HMFREQ_CTL_ENABLE_PL_GET_SPEED);
	Hmfreq_Write(0, ctl);
}
/*********************************************************
 * 设置周期/频率测量的基础时钟，是对AXI时钟的分频数
 *
 *********************************************************/
void Hmfreq_SetClkDiv(u32 div) {
	Hmfreq_Write(1, div);
}
/*********************************************************
 * 设置PL判别动镜移动方向改变的判别阈值
 *当减速判别前后时间之差大于th时才认为是减速
 *********************************************************/
void Hmfreq_SetPLMoveChangeTh(u32 th) {
	Hmfreq_Write(2, th);
}
/*********************************************************
 * 设置PL判别动镜移动方向改变的缓存长度
 * 参数th的取值范围为1-16
 *********************************************************/
void Hmfreq_SetPLMoveChangeBufSize(u32 th) {
	Hmfreq_Write(2, th);
}
/*********************************************************
 * PS获取控制寄存器的值，返回该值并用该值更新ps端的ctl寄存器
 *********************************************************/
u32 Hmfreq_GetCtlFromPL() {
	u32 tp = Hmfreq_Read(0);
	ctl = tp;
	return tp;
}
/*********************************************************
 * PS获取分频值
 *********************************************************/
u32 Hmfreq_GetClkDiv() {
	return Hmfreq_Read(1);
}

/*********************************************************
 * PS获取脉冲周期值T
 *********************************************************/
u32 Hmfreq_GetCycleT() {
	return Hmfreq_Read(2);
}
/*********************************************************
 * PS获取PL部分状态数据
 *********************************************************/
u32 Hmfreq_GetPLState() {
	return Hmfreq_Read(3);
}
/*********************************************************
 * 开启模块自测
 *********************************************************/
void Hmfreq_StartTestSelf() {
	ctl |= HMFREQ_CTL_TEST_SELF;
	Hmfreq_Write(0, ctl);
}
/*********************************************************
 * 关闭模块自测
 *********************************************************/
void Hmfreq_CloseTestSelf() {
	ctl &= (~HMFREQ_CTL_TEST_SELF);
	Hmfreq_Write(0, ctl);
}

