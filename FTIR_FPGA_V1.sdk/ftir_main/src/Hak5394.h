/*
 * Hak5394.h
 *
 *  Created on: 2019年6月14日
 *      Author: hpy
 */

#ifndef SRC_HAK5394_H_
#define SRC_HAK5394_H_

#include "AK5394_LF398.h"

//定义AD数据基地址
#define AK_5394_DATA_BUF_LEN 1000000  //设置数据最大长度
#define AK_5394_DATA1_BASE_ADDR 			(0x04000000U)  //AD数据1基地址
#define AK_5394_DATA2_BASE_ADDR 			(AK_5394_DATA1_BASE_ADDR + AK_5394_DATA_BUF_LEN*4)//AD数据2基地址
#define AK_5394_TIME1_BASE_ADDR 			(AK_5394_DATA2_BASE_ADDR + AK_5394_DATA_BUF_LEN*4)//AD数据获取时间1基地址
#define AK_5394_TIME2_BASE_ADDR				(AK_5394_TIME1_BASE_ADDR + AK_5394_DATA_BUF_LEN*4)//AD数据获取时间2基地址
#define AK_5394_LASER_PULSE_TIME1_BASE_ADDR (AK_5394_TIME2_BASE_ADDR + AK_5394_DATA_BUF_LEN*4)//激光脉冲时间1数组基地址
#define AK_5394_LASER_PULSE_TIME2_BASE_ADDR (AK_5394_LASER_PULSE_TIME1_BASE_ADDR + AK_5394_DATA_BUF_LEN*4)//激光脉冲时间1数组基地址

#define DDR_HIGH_AK2594_ADDR (AK_5394_DATA1_BASE_ADDR + AK_5394_DATA_BUF_LEN*6)

#define LWIP_DATA_BUF_LEN 10000000  //设置数据最大长度
#define LWIP_SEND_BUFFER_FORWARD (AK_5394_LASER_PULSE_TIME2_BASE_ADDR + AK_5394_DATA_BUF_LEN*4)//激光脉冲时间1数组基地址
#define LWIP_SEND_BUFFER_BACKWARD (LWIP_SEND_BUFFER_FORWARD + LWIP_DATA_BUF_LEN)//激光脉冲时间1数组基地址


extern  int *ak5394_m1; //指向存储AD数据1的实际物理地址的数组
extern  int *ak5394_m2; //指向存储AD数据2的实际物理地址的数组
extern  u32 *ak5394_t1; //指向存储AD数据时间1的实际物理地址的数组
extern  u32 *ak5394_t2; //指向存储AD数据时间2的实际物理地址的数组
extern	u32 *ak5394_laser_t1;//激光脉冲时间标刻1指针数组
extern	u32 *ak5394_laser_t2;//激光脉冲时间标刻2指针数组
/*
#define AK_5394_DATA1_BASE_ADDR 0x04000000U
#define AK_5394_DATA2_BASE_ADDR 0x040C8000U
#define AK_5394_TIME1_BASE_ADDR 0x04190000U
#define AK_5394_TIME2_BASE_ADDR 0x04258000U
//分配DDR中对应AD数据的内存地址给数组
extern int ak5394_m1[AK_5394_DATA_BUF_LEN] __attribute__((section(".ak5394M1Section")));//
extern int ak5394_m2[AK_5394_DATA_BUF_LEN] __attribute__((section(".ak5394M2Section")));//
//分配DDR中对应AD数据时间的内存地址给数组
extern u32 ak5394_t1[AK_5394_DATA_BUF_LEN] __attribute__((section(".ak5394T1Section")));//
extern u32 ak5394_t2[AK_5394_DATA_BUF_LEN] __attribute__((section(".ak5394T2Section")));//
*/
//定义控制参数
#define AK_TIME_BIT_WIDE         		0x08000000U   //设置定时数据位宽，0:32位 1:16位
#define AK_ENABLE_PL_FILTER 			0x04000000U  //开启硬件滤波
#define AK_REUSE_REG1_LASER_TIME_NUM 	0x02000000U  //开启数据通道1复用为激光脉冲数据量
#define AK_ENABLE_SET_LF398             0X01000000U  //使能LF398 MCU控制权限
#define AK_SET_LF398_HIGH               0x00800000U  //设置LF398逻辑电压为高电平
#define AK_INT_SYS_AD_DATA				0x00400000U  //开启硬件采集终端信号同步AD数据 (AD数据完成就进行读取，无论时间信息是否完成写入DDR)
#define AK_MISS_DATA_AD_OR_LASER        0x00200000U // 设置丢失数据返回对象为激光脉冲时间顺序



void Hak5394_init(void);//软件初始化
void Hak5394_WriteReg(u32 cmd,u32 data);
int Hak5394_GetData(u8 ch);
u32 Hak5394_GetStatus(void);

void Hak5394_SetDDRSize(u32 size);
void Hak5394_StartGetData(void);
void Hak5394_CloseGetData(void);
void Hak5394_PL_init(void);  //使用硬件采集时使用该函数初始化AK5394模块

void Hak5394_SetOption(u32 option,int sw);//设置硬件参数 bit设置
/********************************************************************
 * 设置ak5394计时器的计时器频率
 * 输入的参数为分频数
 * 0:关闭时钟
 * 1:不分频
 * N：N分频
 *******************************************************************/
void Hak5394_SetTimerFeq(u32 f);
/**********************************************************************
 * 设置定是采集的定时时间，在则测模式下，该值用于模拟激光脉冲信号
 *********************************************************************/
void Hak5394_SetGetDataInterval(u32 in);
/******************************************************************************************/
/**
 * 开启硬件AD数据采集
 * 针对采集数据为8192个点，使用A9处理器的L2，刷新一次cache用时为30us，使用10khz进行触发，采集满一
 * 次数据用时为 8192*100 us = 819ms，接近于1s，因此有足够的时间进行数据传输，因此只需要在硬件采集
 * 完成的中断信号中进行数据缓存更新，尽量不要再其中处理其他数据
 *******************************************************************************************/
void Hak5394_StartGetData(void);
/*******************************************************************************************
 * 设置时间寄存器数据位宽
 * 输入参数：16U   32U，错误输入则默认设置为32位位宽
 ******************************************************************************************/
void Hak5394_SetTime_BitWide(u32 bitwide);
/*******************************************************************************************
 * 设置硬件滤波
 * 0:关闭硬件滤波，其他：开启硬件滤波
 ******************************************************************************************/
void Hak5394_Enable_PL_Filter(int set);
/*******************************************************************************************
 * 设置数据通道1为激光脉冲时间数量通道
 * 0:关闭复用，其他：开启复用
 ******************************************************************************************/
void Hak5394_Reuse_ch1(int set);

/*******************************************************************************************
 *开启LF398接受MCU设置逻辑电压
 * 0:关闭复用，其他：开启
 ******************************************************************************************/
void Hak5394_Enable_McuSetLf(int set);
/*******************************************************************************************
 *设置LF398逻辑电压为高电平
 ******************************************************************************************/
void Hak5394_SetLfHigh(void);
/*******************************************************************************************
 *设置LF398逻辑电压为低电平
 ******************************************************************************************/
void Hak5394_SetLfLow(void);
/*******************************************************************************************
 *开启硬件采集终端信号同步AD数据 (AD数据完成就进行读取，无论时间信息是否完成写入DDR)
 * 0:关闭，其他：设置
 ******************************************************************************************/
void Hak5394_SetSysInt(u32 set);
/*******************************************************************************************
 *设置丢失数据返回对象为激光脉冲时间顺序
 * 0:关闭，其他：设置
 ******************************************************************************************/
void Hak5394_SetMissDataCh(u32 set);
#endif /* SRC_HAK5394_H_ */
