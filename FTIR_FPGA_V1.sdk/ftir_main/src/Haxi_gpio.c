/*
 * Haxi_gpio.c
 *
 *  Created on: 2019年9月12日
 *      Author: hpy
 *  该文件的作用是控制AXI的GPIO函数。
 *  包括AXI输入输出，但是PL到PS的中断不包含在里面
 */

#include "xparameters.h"
#include "Haxi_gpio.h"
XGpio Haxi_gpio ;//axi gpio对象

static u32 gpio_o_reg ;//存储输出IO的电平状态

//Gpio 0是用于中断引入的
//普通输入输出axi gpio的id为gpio 1
/****************************************************
 * 初始化axi gpio，包括输入GPIO和输出GPIO
 ***************************************************/
int Haxi_gpio_init()
{
	int Status;
	/* Initialize the GPIO driver */
	Status = XGpio_Initialize(&Haxi_gpio, XPAR_GPIO_1_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("AXI Gpio1 Initialization Failed\r\n");
		return XST_FAILURE;
	}
	/* Set the direction for all signals as inputs except the LED output */
	XGpio_SetDataDirection(&Haxi_gpio, 1, 0x00000000);//通道1的GPIO全为输出IO
	XGpio_SetDataDirection(&Haxi_gpio, 2, 0xFFFFFFFF);//通道2的GPIO全为输入IO
	gpio_o_reg = XGpio_DiscreteRead(&Haxi_gpio,1);
	return XST_SUCCESS;
}

/******************************************
 * 对输出GPIO进行写操作
 ******************************************/
void Haxi_gpio_write(u32 Data)
{
	XGpio_DiscreteWrite(&Haxi_gpio, 1, Data);
	gpio_o_reg = Data;
}
/******************************************
 * 对输出GPIO进行写操作
 * 输入参数1:这个参数是设置输出的第几位0-31
 * 参数2:  1->高电平  0->低电平
 ******************************************/
void Haxi_gpio_write_bit(u32 pos,u32 data)
{
	if(data != 0 || data!= 1)return;//只接受0,1
	if(data==1)gpio_o_reg |= data<<pos;
	else gpio_o_reg &= ~(data<<pos);
	XGpio_DiscreteSet(&Haxi_gpio,1,pos);
	XGpio_DiscreteWrite(&Haxi_gpio, 1, gpio_o_reg);
}
