/*
 * Hdac7631.h
 *驱动DAC7631
 *  Created on: 2019年9月26日
 *      Author: hpy
 */
#include "DAC7631.h"
#ifndef SRC_HDAC7631_H_
#define SRC_HDAC7631_H_

//与硬件有关的宏参数
#define HDAC7631_ENABLE 					(0x80000000)  //开启DAC7631
#define HDAC7631_ENABLE_FORCE_UPDATE		(0x40000000)  //开启强制更新DA数据(MCU发送更新指令，否则不更新数据)，关闭时只有当数据发生改变时更新

#define HDAC7631_FORCE_TRIG					(0x00000001)  //利用该位置传输出发信号



void Hdac7631_init();
void Hdac7631_SetCoil(u32 value);
void Hdac7631_SetDa(u16 da1,u16 da2);
void Hdac_7631_SetClk(u32 value);
u32 Hdac_7631_GetClk(void);
u32 Hdac7631_GetCfg(void);
void Hdac7631_Enable();
void Hdac7631_Disenable();
void Hdac7631_Trig();
void Hdac7631_Open_Force_update();
void Hdac7631_Close_Force_update();

#endif /* SRC_HDAC7631_H_ */
