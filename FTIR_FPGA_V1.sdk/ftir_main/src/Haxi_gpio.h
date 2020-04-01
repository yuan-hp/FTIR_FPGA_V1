/*
 * Haxi_gpio.h
 *
 *  Created on: 2019Äê9ÔÂ12ÈÕ
 *      Author: hpy
 */

#ifndef SRC_HAXI_GPIO_H_
#define SRC_HAXI_GPIO_H_
#include "xgpio.h"
int Haxi_gpio_init();
void Haxi_gpio_write(u32 Data);
void Haxi_gpio_write_bit(u32 pos,u32 data);


#endif /* SRC_HAXI_GPIO_H_ */
