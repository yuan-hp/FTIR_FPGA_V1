/*
 * Hdac7631.c
 *
 *  Created on: 2019年11月18日
 *      Author: hpy
 */

#include "Hdac7631.h"
#include "DAC7631.h"
#include "xparameters.h"
#include "xil_io.h"
#include "sleep.h"

/*********************************************************************
 * DAC7631初始化
 ********************************************************************/
void Hdac7631_init() {
	Hdac_7631_SetClk(150U); //设置DAC时钟
	Hdac7631_Close_Force_update();
	Hdac7631_Enable();
	usleep(1);
	Hdac7631_SetDa(65536 / 2, 65536 / 2);
}

/*********************************************************************
 * 设置定镜线圈电流大小
 * 参数1：线圈选择
 *		value包含了两个16位DA的数值，
 *		关系式子:Vo = -2.5 + (valve*5/65536)
 ********************************************************************/
void Hdac7631_SetCoil(u32 value) {
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG1_OFFSET, value);
}
/*********************************************************************
 * 设置DA的数值
 * 两个参数分别数两个AD的数值
 ********************************************************************/
void Hdac7631_SetDa(u16 da1, u16 da2) {
	u32 value = da1 << 16 | da2;
	Hdac7631_SetCoil(value);
}
/*********************************************************************
 * 设置DAC基本时钟，驱动DAC7631
 * 参数1:是对axi时钟的分频系数
 * 要求分频后得到的时钟不得超过30MHz
 ********************************************************************/
void Hdac_7631_SetClk(u32 value) {
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG2_OFFSET, value);
}
/*********************************************************************
 * 读取DAC基本时钟的分频因子
 ********************************************************************/
u32 Hdac_7631_GetClk(void) {
	return DAC7631_mReadReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG2_OFFSET);
}
/*********************************************************************
 * 读取DAC当前配置寄存器的值
 ********************************************************************/
u32 Hdac7631_GetCfg(void) {
	return DAC7631_mReadReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG0_OFFSET);
}
/*********************************************************************
 * 使能DAC7631
 ********************************************************************/
void Hdac7631_Enable() {
	u32 tr = Hdac7631_GetCfg();
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG0_OFFSET, tr | HDAC7631_ENABLE);
}
/*********************************************************************
 * 关闭DAC7631
 ********************************************************************/
void Hdac7631_Disenable() {
	u32 tr = Hdac7631_GetCfg();
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG0_OFFSET, (tr & (~HDAC7631_ENABLE)));
}
/*********************************************************************
 * 软件触发一次DAC数据写入
 ********************************************************************/
void Hdac7631_Trig() {
	u32 tr = Hdac7631_GetCfg();
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG0_OFFSET, tr | HDAC7631_FORCE_TRIG);
	usleep(2);
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG0_OFFSET, tr & (~HDAC7631_FORCE_TRIG));
}
/*********************************************************************
 * 开启软件强制更新DAC数据
 ********************************************************************/
void Hdac7631_Open_Force_update() {
	u32 tr = Hdac7631_GetCfg();
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG0_OFFSET, tr | HDAC7631_ENABLE_FORCE_UPDATE);
}
/*********************************************************************
 * 关闭软件强制更新DAC数据
 ********************************************************************/
void Hdac7631_Close_Force_update() {
	u32 tr = Hdac7631_GetCfg();
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG0_OFFSET,
			tr & (~HDAC7631_ENABLE_FORCE_UPDATE));
}
