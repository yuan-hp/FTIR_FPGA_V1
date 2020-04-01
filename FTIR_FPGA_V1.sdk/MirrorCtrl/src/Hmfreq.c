/*
 * Hmfreq.c
 *
 *  Created on: 2019��9��30��
 *      Author: hpy
 */

#include "Hmfreq.h"
#include "xparameters.h"
#include "xil_io.h"
#include "sleep.h"

static u32 ctl = 0U; //���ƼĴ�����ֵ

/**********************************************************
 * д�Ĵ�������
 * regid:�Ĵ������  0 1 2 3
 * data:д�������
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
 * ���Ĵ�������
 * regid:�Ĵ������  0 1 2 3
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
 * ��ʼ��Ƶ��/���ڲ���ģ�飬
 *
 *********************************************************/
void Hmfreq_init() {
	//Hmfreq_RisingMea(); //���������ز�������
	Hmfreq_FallingMea();
	Hmfreq_SetPsCheckMoveDir(); //����PS��⶯������
	Hmfreq_SetClkDiv(1); //���÷�Ƶϵ������ò����Ļ���ʱ�ӣ�����ֵ����Ϊ0������رղ�������
	//Hmfreq_StartTestSelf(); //�����Բ�ģʽ
}
/**********************************************************
 *��ʼ��Ƶ��/���ڲ���ģ�飬
 *ʹ��Ӳ��PL���ּ�⶯���ƶ�����
 *********************************************************/
void Hmfreq_pl_init() {
	Hmfreq_RisingMea(); //���������ز�������
	Hmfreq_SetPlCheckMoveDir(); //����PL��⶯������
	Hmfreq_SetClkDiv(1U); //���÷�Ƶϵ������ò����Ļ���ʱ�ӣ�����ֵ����Ϊ0������رղ�������
	Hmfreq_SetPLMoveChangeTh(4U); //����Ӳ������Ӽ��ٵ���ֵ
	Hmfreq_SetPLMoveChangeBufSize(2U); //���ò�����������Ļ���ֵ
	Hmfreq_SetPLCheckDirPolar(HMFREQ_LOW);
	Hmfreq_StartPLGetMoveDir(); //����Ӳ����⶯������
}
/*********************************************************
 * ���ü�������ź�������
 * ʹ���������ش������ڲ���
 *********************************************************/
void Hmfreq_RisingMea() {
	ctl &= (~HMFREQ_CTL_FALLING_CHECK);
	Hmfreq_Write(0, ctl);
}
/*********************************************************
 * ���ü�������ź��½�����
 * ʹ���������ش������ڲ���
 *********************************************************/
void Hmfreq_FallingMea() {
	ctl |= (HMFREQ_CTL_FALLING_CHECK);
	Hmfreq_Write(0, ctl);
}
/*********************************************************
 * ���ö���������ʹ��PL��������
 *********************************************************/
void Hmfreq_SetPlCheckMoveDir() {
	ctl |= HMFREQ_CTL_PL_SET_MOV_DIR;
	Hmfreq_Write(0, ctl);
}
/*********************************************************
 * ���ö���������ʹ��PS��������
 *********************************************************/
void Hmfreq_SetPsCheckMoveDir() {
	ctl &= (~HMFREQ_CTL_PL_SET_MOV_DIR);
	Hmfreq_Write(0, ctl);
}
/*********************************************************
 * ps(���)��⵽��������ı�󣬽�������ĵ�ƽ����PL����ʹ��
 * �ú���ֻ����ʹ��PS���ж������ʱ����Ч��ʹ��PL��⶯������
 * ������Ը�����
 * ttl  HMFREQ_HIGH:����Ϊ�ߵ�ƽ
 * ttl  HMFREQ_LOW:����Ϊ�͵�ƽ
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
 * ps����pl��⶯���ƶ�����ĵ�ƽ���ԣ��ú���ֻ����ʹ��PL���
 * ������������Ӳ��pl���ڹرն��������������������pl���
 * �Ż���Ч��
 * ttl  HMFREQ_HIGH:��ʼֵΪ1
 * ttl  HMFREQ_LOW:��ʼֵΪ0
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
 * ����PL��ȡ�����ƶ�����
 *********************************************************/
void Hmfreq_StartPLGetMoveDir() {
	ctl |= HMFREQ_CTL_ENABLE_PL_GET_SPEED;
	Hmfreq_Write(0, ctl);
}
/*********************************************************
 * �ر�PL��ȡ�����ƶ�����
 *********************************************************/
void Hmfreq_ClosePLGetMoveDir() {
	ctl &= (~HMFREQ_CTL_ENABLE_PL_GET_SPEED);
	Hmfreq_Write(0, ctl);
}
/*********************************************************
 * ��������/Ƶ�ʲ����Ļ���ʱ�ӣ��Ƕ�AXIʱ�ӵķ�Ƶ��
 *
 *********************************************************/
void Hmfreq_SetClkDiv(u32 div) {
	Hmfreq_Write(1, div);
}
/*********************************************************
 * ����PL�б𶯾��ƶ�����ı���б���ֵ
 *�������б�ǰ��ʱ��֮�����thʱ����Ϊ�Ǽ���
 *********************************************************/
void Hmfreq_SetPLMoveChangeTh(u32 th) {
	Hmfreq_Write(2, th);
}
/*********************************************************
 * ����PL�б𶯾��ƶ�����ı�Ļ��泤��
 * ����th��ȡֵ��ΧΪ1-16
 *********************************************************/
void Hmfreq_SetPLMoveChangeBufSize(u32 th) {
	Hmfreq_Write(2, th);
}
/*********************************************************
 * PS��ȡ���ƼĴ�����ֵ�����ظ�ֵ���ø�ֵ����ps�˵�ctl�Ĵ���
 *********************************************************/
u32 Hmfreq_GetCtlFromPL() {
	u32 tp = Hmfreq_Read(0);
	ctl = tp;
	return tp;
}
/*********************************************************
 * PS��ȡ��Ƶֵ
 *********************************************************/
u32 Hmfreq_GetClkDiv() {
	return Hmfreq_Read(1);
}

/*********************************************************
 * PS��ȡ��������ֵT
 *********************************************************/
u32 Hmfreq_GetCycleT() {
	return Hmfreq_Read(2);
}
/*********************************************************
 * PS��ȡPL����״̬����
 *********************************************************/
u32 Hmfreq_GetPLState() {
	return Hmfreq_Read(3);
}
/*********************************************************
 * ����ģ���Բ�
 *********************************************************/
void Hmfreq_StartTestSelf() {
	ctl |= HMFREQ_CTL_TEST_SELF;
	Hmfreq_Write(0, ctl);
}
/*********************************************************
 * �ر�ģ���Բ�
 *********************************************************/
void Hmfreq_CloseTestSelf() {
	ctl &= (~HMFREQ_CTL_TEST_SELF);
	Hmfreq_Write(0, ctl);
}

