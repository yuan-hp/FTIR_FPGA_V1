/*
 * main.h
 *
 *  Created on: 2019年6月19日
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
//定义命令相关结构体
typedef struct  Cmd_struct
{
//指令段落
	u8 comd;//接收的上位机发来的指令
	u8 dataChar[4];//接收的上位机发来的数据
	u32 data;//转化后的数据
	u8 commond_over;//指令执行完成  0未完成，1完成
// 标志位 && 数据
	u8 stopSendADData;
	u8 arc;//是否回应  0：不回应 1：回应
	u8 dac8811_reg[2];
	u32 adDataLen;//ad数据长度
	u8 isSendDataOk;//发送数据是否完成  1已完成   0未完成

	u8 *sendDataBuf0 ;
	u8 *sendDataBuf1 ;
	u8 sendSwitch;//发送字符串数据选择
	char sendBuf[65535+1000];//发送缓冲字符串
	u8 sendCmdArc;

	int len;
	int ak5394Data;//获取的ad值
	u32 ready_last;
	u32 idx ;

}cmd_struct;
void cmd_decode(cmd_struct * pcmd);//指令译码
void send_data(struct tcp_pcb *mpcb, cmd_struct *pcmd);
void cmd_init(cmd_struct * pcmd);

#endif /* SRC_MAIN_H_ */
