/*
 * Haxi_pwm.c
 *
 *  Created on: 2019��9��17��
 *      Author: hpy
 */
#include "Haxi_pwm.h"
#include "xparameters.h"
#include "xil_io.h"
#include "sleep.h"

static u32 Option_inst = 0U;
/**************************************************************************************
 *						 ����Haxi_pwm����������ģʽ
 * 	------------------------------------------------------------------------------
 |  λ��   |   [31]    |  [30:28] |   [27:25]   |  [2:0]  |
 ------------------------------------------------------------------------------
 |         |ʱ��ѡ��   |���ʹ��    |��PWM�����  |�����ź�  |
 |         |0:AXIʱ�� |1:���ʹ��|Ĭ�ϵ�ƽ[0/1]|        |
 | ����˵�� |1:�ⲿ����|30:ͨ��2  |27:ͨ��2    |2��ͨ��2 |
 |         |ʱ��           |29:ͨ��1  |26:ͨ��1    |1��ͨ��1 |
 |         |          |28:ͨ��0  |25:ͨ��0    |0��ͨ��0 |
 ------------------------------------------------------------------------------
 |  Ĭ��ֵ  |     0    |   000    |     000    |   000   |
 ------------------------------------------------------------------------------
 *����1λ����д���option����
 *����2Ϊд��ģʽ��0ʹ�õ���д��ʲô����ʲô  ����ֵ����д���֮ǰ��ֵ�Ļ�ֵ
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
 * ��ʼ��AXI PWMģ��
 **************************************************/
void Haxi_pwm_init(void) {
	Haxi_pwm_SetOption(0u, 0); //�ָ�Ĭ��ֵ
	Haxi_pwm_SetFeq0(20000U); //��������
	Haxi_pwm_SetHTime0(10000U); //���øߵ�ƽʱ��

	//Haxi_pwm_SetFeq1(100000000U); //��������
	//Haxi_pwm_SetHTime1(50000000U); //���øߵ�ƽʱ��
	Haxi_pwm_SetOption(
			HAXI_PWM_EN_PWM0 | HAXI_PWM_EN_PWM0 | HAXI_PWM_EXT_CLK
					| HAXI_PWM_DEFAULT_PWM2 | HAXI_PWM_DEFAULT_PWM1
					| HAXI_PWM_DEFAULT_PWM0, 1);	//ʹ��ͨ��0 PWM��ʹ���ⲿʱ��
}
/*********************************************************************************
 * ���Ӳ����ʵ�ʵ�option����ֵ,ͬʱ��������洢��optionֵ
 *********************************************************************************/
u32 Haxi_pwm_GetOption(void) {
	Option_inst = FPGA_PWM_mReadReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG6_OFFSET);
	return Option_inst;
}
/***************************************************************
 * ����ͨ��0 PWM���
 ***************************************************************/
void Haxi_pwm_StartC0(void) {
	Haxi_pwm_SetOption(HAXI_PWM_EN_PWM0, 1);
}
/***************************************************************
 * ����ͨ��1 PWM���
 ***************************************************************/
void Haxi_pwm_StartC1(void) {
	Haxi_pwm_SetOption(HAXI_PWM_EN_PWM1, 1);
}
/***************************************************************
 * ����ͨ��2 PWM���
 ***************************************************************/
void Haxi_pwm_StartC2(void) {
	Haxi_pwm_SetOption(HAXI_PWM_EN_PWM2, 1);
}
/***************************************************************
 * �ر�ͨ��0 PWM���
 ***************************************************************/
void Haxi_pwm_CloseC0(void) {
	u32 op = ~HAXI_PWM_EN_PWM0;
	u32 op1 = FPGA_PWM_mReadReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG6_OFFSET);
	Haxi_pwm_SetOption((op & op1), 0);
}
/***************************************************************
 * �ر�ͨ��1 PWM���
 ***************************************************************/
void Haxi_pwm_CloseC1(void) {
	u32 op = ~HAXI_PWM_EN_PWM1;
	u32 op1 = FPGA_PWM_mReadReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG6_OFFSET);
	Haxi_pwm_SetOption((op & op1), 0);
}
/***************************************************************
 * �ر�ͨ��2 PWM���
 ***************************************************************/
void Haxi_pwm_CloseC2(void) {
	u32 op = ~HAXI_PWM_EN_PWM2;
	u32 op1 = FPGA_PWM_mReadReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG6_OFFSET);
	Haxi_pwm_SetOption((op & op1), 0);
}
/****************************************************************
 * ����ͨ��0 PWM����/Ƶ��
 ****************************************************************/
void Haxi_pwm_SetFeq0(u32 Tcnt) {
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG0_OFFSET, Tcnt);
}
/****************************************************************
 * ����ͨ��1 PWM����/Ƶ��
 ****************************************************************/
void Haxi_pwm_SetFeq1(u32 Tcnt) {
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG2_OFFSET, Tcnt);
}
/****************************************************************
 * ����ͨ��2 PWM����/Ƶ��
 ****************************************************************/
void Haxi_pwm_SetFeq2(u32 Tcnt) {
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG4_OFFSET, Tcnt);
}
/****************************************************************
 * ����ͨ��0 PWM�ߵ�ƽ����ʱ��
 ****************************************************************/
void Haxi_pwm_SetHTime0(u32 hcnt) {
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG1_OFFSET, hcnt);
}
/****************************************************************
 * ����ͨ��1 PWM�ߵ�ƽ����ʱ��
 ****************************************************************/
void Haxi_pwm_SetHTime1(u32 hcnt) {
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG3_OFFSET, hcnt);
}
/****************************************************************
 * ����ͨ��2 PWM�ߵ�ƽ����ʱ��
 ****************************************************************/
void Haxi_pwm_SetHTime2(u32 hcnt) {
	FPGA_PWM_mWriteReg(XPAR_FPGA_PWM_0_S00_AXI_BASEADDR,
			FPGA_PWM_S00_AXI_SLV_REG5_OFFSET, hcnt);
}

/***************************************************************
 * ����ͨ��0�������ŵ�ƽ ���ڿ��Ʒ���
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
 * ����ͨ��1�������ŵ�ƽ ���ڿ��Ʒ���
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
 * ����ͨ��2�������ŵ�ƽ ���ڿ��Ʒ���
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
 * �ر�����PWM�����׼���˳�
 ***************************************************************/
void Haxi_pwm_clean(void) {
	Haxi_pwm_CloseC0();
	Haxi_pwm_CloseC1();
	Haxi_pwm_CloseC2();
}
