/*
 * Hdac7631.c
 *
 *  Created on: 2019��11��18��
 *      Author: hpy
 */

#include "Hdac7631.h"
#include "DAC7631.h"
#include "xparameters.h"
#include "xil_io.h"
#include "sleep.h"

/*********************************************************************
 * DAC7631��ʼ��
 ********************************************************************/
void Hdac7631_init() {
	Hdac_7631_SetClk(150U); //����DACʱ��
	Hdac7631_Close_Force_update();
	Hdac7631_Enable();
	usleep(1);
	Hdac7631_SetDa(65536 / 2, 65536 / 2);
}

/*********************************************************************
 * ���ö�����Ȧ������С
 * ����1����Ȧѡ��
 *		value����������16λDA����ֵ��
 *		��ϵʽ��:Vo = -2.5 + (valve*5/65536)
 ********************************************************************/
void Hdac7631_SetCoil(u32 value) {
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG1_OFFSET, value);
}
/*********************************************************************
 * ����DA����ֵ
 * ���������ֱ�������AD����ֵ
 ********************************************************************/
void Hdac7631_SetDa(u16 da1, u16 da2) {
	u32 value = da1 << 16 | da2;
	Hdac7631_SetCoil(value);
}
/*********************************************************************
 * ����DAC����ʱ�ӣ�����DAC7631
 * ����1:�Ƕ�axiʱ�ӵķ�Ƶϵ��
 * Ҫ���Ƶ��õ���ʱ�Ӳ��ó���30MHz
 ********************************************************************/
void Hdac_7631_SetClk(u32 value) {
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG2_OFFSET, value);
}
/*********************************************************************
 * ��ȡDAC����ʱ�ӵķ�Ƶ����
 ********************************************************************/
u32 Hdac_7631_GetClk(void) {
	return DAC7631_mReadReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG2_OFFSET);
}
/*********************************************************************
 * ��ȡDAC��ǰ���üĴ�����ֵ
 ********************************************************************/
u32 Hdac7631_GetCfg(void) {
	return DAC7631_mReadReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG0_OFFSET);
}
/*********************************************************************
 * ʹ��DAC7631
 ********************************************************************/
void Hdac7631_Enable() {
	u32 tr = Hdac7631_GetCfg();
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG0_OFFSET, tr | HDAC7631_ENABLE);
}
/*********************************************************************
 * �ر�DAC7631
 ********************************************************************/
void Hdac7631_Disenable() {
	u32 tr = Hdac7631_GetCfg();
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG0_OFFSET, (tr & (~HDAC7631_ENABLE)));
}
/*********************************************************************
 * �������һ��DAC����д��
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
 * �������ǿ�Ƹ���DAC����
 ********************************************************************/
void Hdac7631_Open_Force_update() {
	u32 tr = Hdac7631_GetCfg();
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG0_OFFSET, tr | HDAC7631_ENABLE_FORCE_UPDATE);
}
/*********************************************************************
 * �ر����ǿ�Ƹ���DAC����
 ********************************************************************/
void Hdac7631_Close_Force_update() {
	u32 tr = Hdac7631_GetCfg();
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,
			DAC7631_S00_AXI_SLV_REG0_OFFSET,
			tr & (~HDAC7631_ENABLE_FORCE_UPDATE));
}
