/*
 * Hmfreq.h
 * �������������Ƶ��/����
 *  Created on: 2019��9��30��
 *      Author: hpy
 */

#ifndef SRC_HMFREQ_H_
#define SRC_HMFREQ_H_

#include "FreqMeasure.h"//Ƶ�ʲ���ͷ�ļ�

#define HMFREQ_CTL_FALLING_CHECK 			(0x00000001U) //�O��ʹ��������߀���½��ش������ڲ�����
#define HMFREQ_CTL_PL_SET_MOV_DIR			(0x00000002U)//���ö���������ʹ��PL���ֻ��
#define HMFREQ_CTL_DIR_HIGH					(0x00000004U) //���ڿ���PS����PL���������źŵ�bitλ
#define HMFREQ_CTL_PL_CHECK_DIR_POLAR		(0x00000008U)	//��������PL��⶯������ĳ�ʼ���ԣ�0/1
#define HMFREQ_CTL_ENABLE_PL_GET_SPEED      (0x00000010U)  //ps����PL��ȡ�����ٶ�

#define HMFREQ_CTL_TEST_SELF				(0x80000000U)  //����ģ���Բ�

#define  HMFREQ_HIGH						(1U)
#define  HMFREQ_LOW							(0U)
#define  HMFREQ_AXI_CLK_FREQ          		(148148163U)  //AXI��ʱ��Ƶ�ʣ���ԼΪ148MHz
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
