/*
 * Haxi_gpio.c
 *
 *  Created on: 2019��9��12��
 *      Author: hpy
 *  ���ļ��������ǿ���AXI��GPIO������
 *  ����AXI�������������PL��PS���жϲ�����������
 */

#include "xparameters.h"
#include "Haxi_gpio.h"
XGpio Haxi_gpio ;//axi gpio����

static u32 gpio_o_reg ;//�洢���IO�ĵ�ƽ״̬

//Gpio 0�������ж������
//��ͨ�������axi gpio��idΪgpio 1
/****************************************************
 * ��ʼ��axi gpio����������GPIO�����GPIO
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
	XGpio_SetDataDirection(&Haxi_gpio, 1, 0x00000000);//ͨ��1��GPIOȫΪ���IO
	XGpio_SetDataDirection(&Haxi_gpio, 2, 0xFFFFFFFF);//ͨ��2��GPIOȫΪ����IO
	gpio_o_reg = XGpio_DiscreteRead(&Haxi_gpio,1);
	return XST_SUCCESS;
}

/******************************************
 * �����GPIO����д����
 ******************************************/
void Haxi_gpio_write(u32 Data)
{
	XGpio_DiscreteWrite(&Haxi_gpio, 1, Data);
	gpio_o_reg = Data;
}
/******************************************
 * �����GPIO����д����
 * �������1:�����������������ĵڼ�λ0-31
 * ����2:  1->�ߵ�ƽ  0->�͵�ƽ
 ******************************************/
void Haxi_gpio_write_bit(u32 pos,u32 data)
{
	if(data != 0 || data!= 1)return;//ֻ����0,1
	if(data==1)gpio_o_reg |= data<<pos;
	else gpio_o_reg &= ~(data<<pos);
	XGpio_DiscreteSet(&Haxi_gpio,1,pos);
	XGpio_DiscreteWrite(&Haxi_gpio, 1, gpio_o_reg);
}
