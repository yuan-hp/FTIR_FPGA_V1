/*
 * Htas5111.c
 *
 *  Created on: 2019��11��18��
 *      Author: hpy
 */
#include"system.h"
/*********************************************************************
 * TAS5111��ʼ��
 ********************************************************************/
void Htas5111_init() {
	Htas5111_SeriesMode(); //����Haxi����pwm����TAS5111
	Htas5111_ResetPolar();
	//Htas5111_SetPolar();
	//Htas5111_SetSd(); //����mode��
	Htas5111_Reset(); //��λһ��оƬ
	usleep(1000);
	Htas5111_SetReset();
}
/*********************************************************************
 * TAS5111����ģʽ
 ********************************************************************/
void Htas5111_SeriesMode() {
	u32 value = TAS5111_mReadReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET);
	value |= HTAS5111_USING_SERIES_MODE;
	TAS5111_mWriteReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET, value);
}
/*********************************************************************
 * TAS5111����ģʽ�ر�
 ********************************************************************/
void Htas5111_SeriesModeClose() {
	u32 value = TAS5111_mReadReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET);
	value &= (~HTAS5111_USING_SERIES_MODE);
	TAS5111_mWriteReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET, value);
}
/*********************************************************************
 * TAS5111����RESETΪ1
 ********************************************************************/
void Htas5111_SetReset() {
	u32 value = TAS5111_mReadReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET);
	value |= (HTAS5111_SET_RESET);
	TAS5111_mWriteReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET, value);
}
/*********************************************************************
 * TAS5111����RESETΪ0
 ********************************************************************/
void Htas5111_Reset() {
	u32 value = TAS5111_mReadReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET);
	value &= (~HTAS5111_SET_RESET);
	TAS5111_mWriteReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET, value);
}
/*********************************************************************
 * TAS5111�����ڼ���ģʽ�µļ���
 ********************************************************************/
void Htas5111_SetPolar() {
	u32 value = TAS5111_mReadReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET);
	value |= (HTAS5111_SERIES_SET_POLARITY);
	TAS5111_mWriteReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET, value);
}
/*********************************************************************
 * TAS5111�����ڼ���ģʽ�µļ���Ϊ0
 ********************************************************************/
void Htas5111_ResetPolar() {
	u32 value = TAS5111_mReadReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET);
	value &= (~HTAS5111_SERIES_SET_POLARITY);
	TAS5111_mWriteReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET, value);
}
/*********************************************************************
 * TAS5111����sdΪ1
 ********************************************************************/
void Htas5111_SetSd() {
	u32 value = TAS5111_mReadReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET);
	value |= (HTAS5111_SET_SD);
	TAS5111_mWriteReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET, value);
}
/*********************************************************************
 * TAS5111����sdΪ0
 ********************************************************************/
void Htas5111_ResetSd() {
	u32 value = TAS5111_mReadReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET);
	value &= (~HTAS5111_SET_SD);
	TAS5111_mWriteReg(XPAR_TAS5111_0_S00_AXI_BASEADDR,
			TAS5111_S00_AXI_SLV_REG0_OFFSET, value);
}