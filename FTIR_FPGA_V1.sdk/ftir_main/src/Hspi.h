/*
 * Hspi.h
 * PS SPI的操作
 *  Created on: 2019年6月13日
 *      Author: heer
 */

#ifndef SRC_HSPI_H_
#define SRC_HSPI_H_


#include "xspips.h"		/* SPI device driver */

s32 XSpiPs_SelfTest(XSpiPs *InstancePtr);//自测
int Hspi_init(XSpiPs *SpiInstancePtr,u16 SpiDeviceId);//初始化spi
int Hspi_Transfer8(XSpiPs *Spi,u8 ChipSlect,u8 *SendBuf,u8 *RecvBuf,u32 ByteCount);//读，写数据
s32 Hspi_Transfer16(XSpiPs *InstancePtr,u8 ChipSlect, u16 *SendBufPtr,u16 *RecvBufPtr, u32 ByteCount);
#endif /* SRC_HSPI_H_ */
