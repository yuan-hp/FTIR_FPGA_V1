/*
 * Hdac7631.c
 *
 *  Created on: 2019年9月26日
 *      Author: hpy
 */

#include "Hdac7631.h"
#include "DAC7631.h"
#include "xparameters.h"
#include "xil_io.h"
#include "sleep.h"


void Hdac7631_init()
{
	DAC7631_mWriteReg(XPAR_DAC7631_0_S00_AXI_BASEADDR,DAC7631_S00_AXI_SLV_REG2_OFFSET,150U);//设置时钟信号
	Hdac7631_SetCoil(HDAC_5394_COIL_A , 37768);//设置默认输出为0
	Hdac7631_SetCoil(HDAC_5394_COIL_B , 37768);
}

/*********************************************************************
 * 设置定镜线圈电流大小
 * 参数1：线圈选择
 * 		HDAC_5394_COIL_A :线圈a
 * 		HDAC_5394_COIL_B :线圈b
 *参数2：设置的值
 *		value的范围为   0 - 65535
 *		关系式子:Vo = -2.5 + (valve*5/65536)
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
