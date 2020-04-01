/*
 * main.h
 *
 *  Created on: 2019��6��19��
 *      Author: hpy
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

#ifndef _DEBUG_
	#define _DEBUG_
#endif

#include "ax_pwm.h"
#include "sleep.h"
#include "Hspi.h"
#include "Huart.h"
#include "Hak5394.h"
#include "Hgpio.h"
#include "HTimer.h"

/* defined by each RAW mode application */
void print_app_header();
int start_application();
//int start_application(struct tcp_pcb *mpcb);
int transfer_data();
void tcp_fasttmr(void);
void tcp_slowtmr(void);


/* missing declaration in lwIP */
void lwip_init();

extern volatile int TcpFastTmrFlag;
extern volatile int TcpSlowTmrFlag;

struct netif *echo_netif;
//����������ؽṹ��
typedef struct  Cmd_struct
{
//ָ�����
	u8 comd;//���յ���λ��������ָ��
	u8 dataChar[4];//���յ���λ������������
	u32 data;//ת���������
	u8 commond_over;//ָ��ִ�����  0δ��ɣ�1���
// ��־λ && ����
	u8 stopSendADData;
	u8 arc;//�Ƿ��Ӧ  0������Ӧ 1����Ӧ
	u8 dac8811_reg[2];
	u32 adDataLen;//ad���ݳ���
	u8 isSendDataOk;//���������Ƿ����  1�����   0δ���

	u8 *sendDataBuf0 ;
	u8 *sendDataBuf1 ;
	u8 sendSwitch;//�����ַ�������ѡ��
	char sendBuf[65535+1000];//���ͻ����ַ���
	u8 sendCmdArc;

	int len;
	int ak5394Data;//��ȡ��adֵ
	u32 ready_last;
	u32 idx ;

}cmd_struct;
void cmd_decode(cmd_struct * pcmd);//ָ������
void send_data(struct tcp_pcb *mpcb, cmd_struct *pcmd);
void cmd_init(cmd_struct * pcmd);

#endif /* SRC_MAIN_H_ */
