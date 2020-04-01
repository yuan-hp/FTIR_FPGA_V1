/*
 * Huart.h
 *PS UART 操作
 *  Created on: 2019年6月14日
 *      Author: heer
 */

#ifndef SRC_HUART_H_
#define SRC_HUART_H_
#include "xuartps.h"

int Huart_init_s(XUartPs *uart,u16 DeviceId,u32 BaudRate);
int Huart_init(XUartPs *uart,u16 DeviceId,u32 BaudRate);
int Huart_Send(XUartPs *uart,u8 *SendBuf,u16 ByteCount);
int Huart_Recv(XUartPs *uart,u8 *RecvBuf,u16 ByteCount);

#endif /* SRC_HUART_H_ */
