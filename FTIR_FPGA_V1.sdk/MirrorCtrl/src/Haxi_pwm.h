/*
 * Haxi_pwm.h
 *
 *  Created on: 2019年9月17日
 *      Author: hpy
 */
#include "FPGA_PWM.h"
#ifndef SRC_HAXI_PWM_H_
#define SRC_HAXI_PWM_H_

//HAXI PWM模块设置参数定义
#define HAXI_PWM_EXT_CLK (0x80000000)  //使能外部时钟
#define HAXI_PWM_EN_PWM2 (0X40000000)   //使能通道2 PWM输出
#define HAXI_PWM_EN_PWM1 (0X20000000)   //使能通道1 PWM输出
#define HAXI_PWM_EN_PWM0 (0X10000000)   //使能通道0 PWM输出
#define HAXI_PWM_DEFAULT_PWM2 (0X08000000)   //设置通道2默认电平为1
#define HAXI_PWM_DEFAULT_PWM1 (0X04000000)   //设置通道1默认电平为1
#define HAXI_PWM_DEFAULT_PWM0 (0X02000000)   //设置通道2默认电平为1

//func

void Haxi_pwm_SetOption(u32 op,int mod);
u32 Haxi_pwm_GetOption(void);
void Haxi_pwm_init(void);
void Haxi_pwm_StartC0(void);
void Haxi_pwm_StartC1(void);
void Haxi_pwm_StartC2(void);
void Haxi_pwm_CloseC0(void);
void Haxi_pwm_CloseC1(void);
void Haxi_pwm_CloseC2(void);
void Haxi_pwm_SetFeq0(u32 Tcnt);
void Haxi_pwm_SetFeq1(u32 Tcnt);
void Haxi_pwm_SetFeq2(u32 Tcnt);
void Haxi_pwm_SetHTime0(u32 hcnt);
void Haxi_pwm_SetHTime1(u32 hcnt);
void Haxi_pwm_SetHTime2(u32 hcnt);
void Haxi_pwm_SetDir0(int dir);
void Haxi_pwm_SetDir1(int dir);
void Haxi_pwm_SetDir2(int dir);
void Haxi_pwm_clean(void);

#endif /* SRC_HAXI_PWM_H_ */
