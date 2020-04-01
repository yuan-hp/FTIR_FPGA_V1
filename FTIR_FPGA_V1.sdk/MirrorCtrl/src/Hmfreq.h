/*
 * Hmfreq.h
 * 测量激光脉冲的频率/周期
 *  Created on: 2019年9月30日
 *      Author: hpy
 */

#ifndef SRC_HMFREQ_H_
#define SRC_HMFREQ_H_

#include "FreqMeasure.h"//频率测量头文件

#define HMFREQ_CTL_FALLING_CHECK 			(0x00000001U) //設置使用上升沿還是下降沿触发周期测量、
#define HMFREQ_CTL_PL_SET_MOV_DIR			(0x00000002U)//设置动镜方向检测使用PL部分获得
#define HMFREQ_CTL_DIR_HIGH					(0x00000004U) //用于控制PS传入PL动镜方向信号的bit位
#define HMFREQ_CTL_PL_CHECK_DIR_POLAR		(0x00000008U)	//用于设置PL检测动镜方向的初始极性，0/1
#define HMFREQ_CTL_ENABLE_PL_GET_SPEED      (0x00000010U)  //ps启动PL获取动镜速度

#define HMFREQ_CTL_TEST_SELF				(0x80000000U)  //开启模块自测

#define  HMFREQ_HIGH						(1U)
#define  HMFREQ_LOW							(0U)
#define  HMFREQ_AXI_CLK_FREQ          		(148148163U)  //AXI的时钟频率，大约为148MHz
//////////////////////////////////////////////////////////////
void Hmfreq_Write(int regid, u32 data);
u32 Hmfreq_Read(int regid) ;
void Hmfreq_init();
void Hmfreq_pl_init();
void Hmfreq_RisingMea();
void Hmfreq_FallingMea() ;
void Hmfreq_SetPlCheckMoveDir() ;
void Hmfreq_SetPsCheckMoveDir() ;
void Hmfreq_PsPushMoveDir2PL(int ttl);
void Hmfreq_SetPLCheckDirPolar(int ttl) ;
void Hmfreq_StartPLGetMoveDir();
void Hmfreq_ClosePLGetMoveDir();
void Hmfreq_SetClkDiv(u32 div);
void Hmfreq_SetPLMoveChangeTh(u32 th) ;
void Hmfreq_SetPLMoveChangeBufSize(u32 th) ;
u32 Hmfreq_GetCtlFromPL();
u32 Hmfreq_GetClkDiv() ;
u32 Hmfreq_GetCycleT();
u32 Hmfreq_GetPLState();
void Hmfreq_StartTestSelf();
void Hmfreq_CloseTestSelf();
///////////////////////////////////////////////////////////////
#endif /* SRC_HMFREQ_H_ */
