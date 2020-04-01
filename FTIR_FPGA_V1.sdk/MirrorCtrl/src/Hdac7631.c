/*
 * Hdac7631.c
 *
 *  Created on: 2019��9��26��
 *      Author: hpy
 */

#include "Hdac7631.h"
#include "DAC7631.h"
#include "xparameters.h"
#include "xil_io.h"
#include "sleep.h"


void Hdac7631_init()
{
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,DAC7631_S00_AXI_SLV_REG2_OFFSET,150U);//����ʱ���ź�
	Hdac7631_SetCoil(HDAC_5394_COIL_A , 37768);//����Ĭ�����Ϊ0
	Hdac7631_SetCoil(HDAC_5394_COIL_B , 37768);
}

/*********************************************************************
 * ���ö�����Ȧ������С
 * ����1����Ȧѡ��
 * 		HDAC_5394_COIL_A :��Ȧa
 * 		HDAC_5394_COIL_B :��Ȧb
 *����2�����õ�ֵ
 *		value�ķ�ΧΪ   0 - 65535
 *		��ϵʽ��:Vo = -2.5 + (valve*5/65536)
 ********************************************************************/
void Hdac7631_SetCoil(int coil , u32 value)
{
	if(coil == HDAC_5394_COIL_A){
		DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,DAC7631_S00_AXI_SLV_REG0_OFFSET,value);
	}
	else{
		DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,DAC7631_S00_AXI_SLV_REG1_OFFSET,value);
	}
	usleep(10);
}
