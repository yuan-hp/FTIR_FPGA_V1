/*
 * Hdac7631.h
 *驱动DAC7631
 *  Created on: 2019年9月26日
 *      Author: hpy
 */
#include "DAC7631.h"
#ifndef SRC_HDAC7631_H_
#define SRC_HDAC7631_H_

#define HDAC_5394_COIL_A 0  //DAC控制的线圈ID
#define HDAC_5394_COIL_B 1

void Hdac7631_init();//初始化
void Hdac7631_SetCoil(int coil , u32 value);//设置输出值
#endif /* SRC_HDAC7631_H_ */
