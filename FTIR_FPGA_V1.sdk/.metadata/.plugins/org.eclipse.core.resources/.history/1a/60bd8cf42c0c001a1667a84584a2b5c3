/*
 * Copyright (C) 2009 - 2018 Xilinx, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 */
#include "system.h"


#define _H_DEBUG_  //定义调试模式宏

#ifdef _H_DEBUG_
#include "Htest.h"
#endif

/****************************************************/



void SoftHandler(void *CallbackRef)
{
	xil_printf("Soft Interrupt from CPU1\r\n") ;
}

int InterruptConnnect(XScuGic *InstancePtr, u16 IntId, void * Handler,void *CallBackRef)
{
	XScuGic_Connect(InstancePtr, IntId,
			(Xil_InterruptHandler)Handler,
			CallBackRef) ;

	XScuGic_Enable(InstancePtr, IntId) ;
	return XST_SUCCESS ;
}

/****************************************************/
u32 MatchValue = 25;

int main()
{
	for(int i=0;i<4;i++)
	{
		MsgHead[i]='A';
	}
	xil_printf("cpu0 started\r\n");
	/************LWIP全局定时器*************/

	LiwpScuTimerIntr();

	/* start the application (web server, rxtest, txtest, etc..) */
#ifdef _H_DEBUG_
	//test_main();//单独模块测试函数
#endif

	////////////////////////////////////////


	/************LWIP*************/
	//Hlwip_init();//lwip配置
	//start_application();
	/*****************************/
	Laser_SwitchIntrInit();//激光和光电开关的中断初始化函数
	Httc_init_ttc0_timer0(NULL);
	Httc_init_ttc0_timer1(NULL);

//	Hgpio_axi_int_init();//配置数据采集完成中断信号
//	Hak5394_PL_init();//配置ak5394工作模式
//	while(1)
//	{
//    	sleep(1);
//    	Hmfreq_PsPushMoveDir2PL(HMFREQ_LOW);//通知PL部分动镜的移动方向为0
//    	sleep(1);
//    	Hmfreq_PsPushMoveDir2PL(HMFREQ_HIGH);//通知PL部分动镜的移动方向1
//	}

	//InterruptConnnect(&LinIntcInstance, SoftIntrIdToCpu0, (void *)SoftHandler, (void *)&LinIntcInstance) ;///
	/* receive and process packets */
	while (1)
	{
		if (TcpFastTmrFlag) {
			tcp_fasttmr();
			TcpFastTmrFlag = 0;
		}
		if (TcpSlowTmrFlag) {
			tcp_slowtmr();
			TcpSlowTmrFlag = 0;
			//XScuGic_SoftwareIntr(&LinIntcInstance, SoftIntrIdToCpu1, CPU1) ;//////////////////////////////////
		}

//		sleep(1);
//		XScuGic_SoftwareIntr(&LinIntcInstance, TTC0SoftIntrIdToCpu1, CPU1) ;//////////////////////////////////
//		sleep(1);
//		XScuGic_SoftwareIntr(&LinIntcInstance, TTC1SoftIntrIdToCpu1, CPU1) ;//////////////////////////////////
//		sleep(1);
//		XScuGic_SoftwareIntr(&LinIntcInstance, LaserSoftIntrIdToCpu1, CPU1) ;//////////////////////////////////
//		sleep(1);
//    	sleep(1);
//    	Hmfreq_PsPushMoveDir2PL(HMFREQ_LOW);//通知PL部分动镜的移动方向为0
//    	sleep(1);
//    	Hmfreq_PsPushMoveDir2PL(HMFREQ_HIGH);//通知PL部分动镜的移动方向1
//------------begin 用户代码-------------------------





		if(LwipBusyFlag==0)
		{
			if(ForwordNeedSendFlag==1)
			{
				if(ForwordSendPermission==1)
					LinTcpSend(LwipForwordNum);
				ForwordNeedSendFlag=0;
			}
			else if(BackwordNeedSendFlag==1)
			{
				if(BackwordSendPermission==1)
					LinTcpSendBack(LwipBackwordNum);
				BackwordNeedSendFlag=0;
			}
		}

		//------------end 用户代码---------------------------

		xemacif_input(echo_netif);
		transfer_data();
		usleep(1);
//------------end 用户代码---------------------------
	}
	/* never reached */
	cleanup_platform();
	return 0;
}

