/*
 * Hspi.h
 * PS SPI�Ĳ���
 *  Created on: 2019��6��13��
 *      Author: heer
 */

#ifndef SRC_HSPI_H_
#define SRC_HSPI_H_


#include "xspips.h"		/* SPI device driver */

s32 XSpiPs_SelfTest(XSpiPs *InstancePtr);//�Բ�
int Hspi_init(XSpiPs *SpiInstancePtr,u16 SpiDeviceId);//��ʼ��spi
int Hspi_Transfer8(XSpiPs *Spi,u8 ChipSlect,u8 *SendBuf,u8 *RecvBuf,u32 ByteCount);//����д����
s32 Hspi_Transfer16(XSpiPs *InstancePtr,u8 ChipSlect, u16 *SendBufPtr,u16 *RecvBufPtr, u32 ByteCount);
#endif /* SRC_HSPI_H_ */
