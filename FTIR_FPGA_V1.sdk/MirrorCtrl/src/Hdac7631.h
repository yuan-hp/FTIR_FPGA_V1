/*
 * Hdac7631.h
 *����DAC7631
 *  Created on: 2019��9��26��
 *      Author: hpy
 */
#include "DAC7631.h"
#ifndef SRC_HDAC7631_H_
#define SRC_HDAC7631_H_

#define HDAC_5394_COIL_A 0  //DAC���Ƶ���ȦID
#define HDAC_5394_COIL_B 1

void Hdac7631_init();//��ʼ��
void Hdac7631_SetCoil(int coil , u32 value);//�������ֵ
#endif /* SRC_HDAC7631_H_ */
