/*
 * Hgpio.h
 *  PS GPIO操作
 *  Created on: 2019年6月18日
 *      Author: hpy
 */

#ifndef SRC_HGPIO_H_
#define SRC_HGPIO_H_

#include "xgpiops.h"
#include "xgpio.h"

#define GPIO_INPUT 0
#define GPIO_OUTPUT 1

/*
int Hgpio_init(XGpioPs *Gpio,u32 pin,u8 direction,u16 DeviceId);
int Hgpio_SetDir(XGpioPs *Gpio,u32 pin,u32 direction);
u32 Hgpio_ReadPin(XGpioPs *Gpio,u32 pin);
void Hgpio_EnableOutput(XGpioPs *Gpio,u32 pin);
void Hgpio_DisableOutput(XGpioPs *Gpio,u32 pin);
void Hgpio_WritePin(XGpioPs *Gpio,u32 pin,u32 data);
*/
int HgpioPs_init(XGpioPs *Gpio,u32 pin,u8 direction,u16 DeviceId);
int HgpioPs_SetDir(XGpioPs *Gpio,u32 pin,u32 direction);
u32 HgpioPs_ReadPin(XGpioPs *Gpio,u32 pin);
void HgpioPs_EnableOutput(XGpioPs *Gpio,u32 pin);
void HgpioPs_DisableOutput(XGpioPs *Gpio,u32 pin);
void HgpioPs_WritePin(XGpioPs *Gpio,u32 pin,u32 data);
int Hgpio_axi_int_init();
extern XGpio HGpio;//中断AXI GPIO对象

#endif /* SRC_HGPIO_H_ */
