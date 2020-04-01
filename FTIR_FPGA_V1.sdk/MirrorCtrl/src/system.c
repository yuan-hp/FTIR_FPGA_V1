//Organized by LinZp
#include "system.h"

XScuTimer LinTimerInstance;	/* Cortex A9 Scu Private Timer Instance */
XScuGic LinIntcInstance;		/* Interrupt Controller Instance */

//////////************************************************动镜控制模块*******************************************************************///////////
//系统全局变量
///////////////////////////////////////////////////////////////////////////
int Tim4Count = 0;//tim4中断计数，调试用
int testcount = 0;
int testcount1 = 0;


/////////////////////////////////////////////////////设置量////////////////////////////////////////////////////////////
uint8_t Speed_Class=4;                              //速度档位
uint8_t Resolution_Class=20;                           //分辨率档位
int phaseflag=0;                                       //是否进行动态校正
int phasecheck=0;                                      //是否进行动态校正检查复位
int GatherDirection=1;                                 //设置快速扫描的方向0，1，2
int WaitCount=600000;//4000000
//////////////default//////////////////

//uint8_t Speed_Class=4;                              //速度档位
//uint8_t Resolution_Class=40;                           //分辨率档位
//int phaseflag=1;                                       //是否进行动态校正
//int phasecheck=1;                                      //是否进行动态校正检查复位
//int GatherDirection=1;                                 //设置快速扫描的方向0，1，2
//int WaitCount=600000;

///////////////////////////////////////////////////测量量////////////////////////////////////////////////////////
unsigned int MirrorSpeed;          									   //动镜速度
int PassedDistance;			                               //当前位置距离零光程点，激光脉冲触发中断次数，也即脉冲数
unsigned char IsNewPulse;                              //是否有新脉冲进来
unsigned char IsNewSpeed;                              //速度是否更新
unsigned char IsNewPhase;                              //相位是否更新
uint8_t MoveDirection;                                 //动镜运动方向标志位
int ReadSpeed[3];                                      //记录3次数据
unsigned int NoPulseTimes;                             //无脉冲次数
int ZeroArrivedFlag=0;                                 //光电零点经过标志位
int ZeroChecked=0;
int PhaseX=0;                                          //一个方向的相位差
int PhaseY=0;                                          //另一个方向上的相位差
int StartStopCount;                                    //控制加减速的频率
int PasseddistanceRevise=0;                            //光电开关与机械零点偏差补偿，此处无用
int PasseddistanceRevisedFlag=0;                       //是否完成光电开关与机械零点偏差补偿标志位
/////////////////////////////////////////////////////给定量//////////////////////////////////////////////////////////////////////////
int SpeedTarget;															         //设定的扫描速度值
int StartSpeedTarget;                                  //加速流程的目标速度
int StopSpeedTarget;                                   //减速流程的目标速度
short ZeroOffset;                                      //零点偏移
int UartSendFlag;                                      //控制串口发送时机
int GotoAccRate;
int GotoStopRate;
int AwayAccRate;
int AwayStopRate;
int HitWallCheck;
int AccStopPreset;
int AccStopWall;
////////////////////////////////////////////////////////控制量//////////////////////////////////////////////////////////////////
uint8_t ProcessFlag;																	 //Process标志位
uint32_t ResetCount;                                   //复位延时计数标志位
int OutValue=0;                                        //输出的占空比,控制动镜
int XOutput=0;																				 //输出的占空比，动态校正
int YOutput=0;																				 //输出的占空比，动态校正
int GotoLimit;                                         //靠近分束器的行程
int AwayLimit;                                         //远离分束器的行程
int OutValueMax;                                       //占空比的最大值
int PhaseCtrlMax;
uint8_t	Uart1_Data_buffer[6];                          //串口接收缓冲区
uint8_t	Uart1_Data;                                    //串口实行单字符接收
uint8_t Uart1_Trans_buffer[20];                        //串口发送缓冲区
int LimitCompensation=0;                               //在此程序中，是光电开关与干涉零点的补偿，可以通过串口设置
int PIDDoor;
///////////////////////////////////////////////////////////软件复位/////////////////////////////////////////////////////////////////////
int PositionResetFlag = 0;                             //软件复位后动镜返回最右端完成标志位
int NoPassZeroDetect = 0;                              //检测是否通过光电零点
int NeedReset = 0;                                     //软件复位标记
int OverSpeedCount=0;                                  //超速计数
int PhaseErrorCount=0;                                 //相位校正错误计数
int OverSpeedCheck;
int OverSpeedDoor;
int NoPulseTimesDoor;
int NotPassedZeroDoor;
///////////////////////////////////////////////////////////调试专用/////////////////////////////////////////////////////////////////////
int watch1=0,watch2=0,watch3=0,watch4=0;
int speedtestbuffer[1000];
int outtestbuffer[1000];
int outxbuffer[1000];
int outybuffer[1000];
int newtr=0;
int newtest=0;
int stopcount=0;
/******************************************************************************/
//                               调整占空比的函数
/*******************************************************************************/
void PWMOutInit(void)
{
	Haxi_pwm_init();

	u32 data = 0u;
	Haxi_pwm_SetDir0(1);
	Haxi_pwm_SetHTime0(data);
	SetOutput(-10000);
	Htas5111_init();
	SetOutput(0);

}
void SetOutput(int compare)       //用于动镜
{
  if(compare>0)
	{
	  	Haxi_pwm_SetDir0(0);
	  	Haxi_pwm_SetHTime0((u32)(compare));
	}
	else
	{
		Haxi_pwm_SetDir0(1);
		Haxi_pwm_SetHTime0((u32)(-compare));
	}
}
/******************************************************************************/
//                               激光脉冲周期测量函数
/*******************************************************************************/

void LaserFreqMeasureInit(void)
{
	Hmfreq_init();
}


int LaserFreqMeasure(void)
{
	return 148148163/Hmfreq_GetCycleT();//获取脉冲的计数值;
}


/******************************************************************************/
//                               系统复位
/*******************************************************************************/
void SystemReset()
{

	SetOutput(0);                                  //停止动作


	MoveDirection=MoveGoToSplitter;
	NeedReset=0;
	PIDReset();
	MirrorSpeed                  = 0;
	PassedDistance               = 0;
	OutValue                     = 0;
	XOutput                      = 0;
	YOutput                      = 0;
	NoPulseTimes                 = 0;

	UartSendFlag                 = 0;
	PasseddistanceRevise		 = 0;
	PasseddistanceRevisedFlag    = 0;
	ZeroArrivedFlag				 = 0;
	IsNewPhase                   = 0;
	IsNewSpeed                   = 0;

	PositionResetFlag            = 0;
}



//////////***************************************************************************************************************************///////////
void SoftHandler(void *CallbackRef)
{
	xil_printf("Soft Interrupt from CPU0\r\n") ;
	//xil_printf("Hello World%d\n\r",XTtcPs_GetCounterValue(Httc_GetTimerInst(1,0)));
}
void ZeroCheckSoftHandler(void *CallbackRef)
{
	PassedDistance=0;
	NoPassZeroDetect=0;
	if(MoveDirection==MoveAwaySplitter)
	{		                                     //光电零点
		//if(MoveDirection==MoveAwaySplitter)                       //此处按理应该使用PositionResetFlag作为判据，但是由此可以歪打正着的规避某些软硬件运行错误
		ZeroArrivedFlag = 1;                                    //确定到达光电开关，开始根据行程换向
	}

	xil_printf("zeor_checked");

}
void TTC0SoftHandler(void *CallbackRef)//控制定时器
{
//	testcount++;
//
//	if(testcount>1000000)
//	{
//		xil_printf("1") ;
//		testcount=0;
//	}
//
//	//xil_printf("Hello World%d\n\r",XTtcPs_GetCounterValue(Httc_GetTimerInst(1,0)));
	if(ProcessFlag==DebugFlag)  //调试模式，进去了就出不来。激光信号消失时会进入
	{
		//SetOutput(-4500);
		SetOutput(0);

		if(newtr<1000)
		//if(stopcount>50)
		{
			newtr=2000;
			char str[50];
			for(int i=0;i<stopcount;i++)
			{
				sprintf(str,"%d,",speedtestbuffer[i]);
				//HAL_UART_Transmit(&huart1,(uint8_t*)str,strlen(str),1000);
			}
			sprintf(str,"\n\n\n\n\n");
			//HAL_UART_Transmit(&huart1,(uint8_t*)str,strlen(str),1000);
			for(int i=0;i<stopcount;i++)
			{
				sprintf(str,"%d,",outtestbuffer[i]);
				//HAL_UART_Transmit(&huart1,(uint8_t*)str,strlen(str),1000);
			}
			sprintf(str,"\n\n\n\n\n");
			//HAL_UART_Transmit(&huart1,(uint8_t*)str,strlen(str),1000);
			for(int i=0;i<stopcount;i++)
			{
				sprintf(str,"%d,",outxbuffer[i]);
				//HAL_UART_Transmit(&huart1,(uint8_t*)str,strlen(str),1000);
			}
			sprintf(str,"\n\n\n\n\n");
			//HAL_UART_Transmit(&huart1,(uint8_t*)str,strlen(str),1000);
			for(int i=0;i<stopcount;i++)
			{
				sprintf(str,"%d,",outybuffer[i]);
				//HAL_UART_Transmit(&huart1,(uint8_t*)str,strlen(str),1000);
			}
		 }
	}
	if(ProcessFlag==ContinueFlag)
	{
		ResetCount += 1;

		if(ResetCount>WaitCount/2)
		{
			ResetCount=0;
		  ProcessFlag = preStartFlag;
		}
	}
	if(ProcessFlag==ResetFlag)  //复位流程，进行初始化操作，一定延时后跳出
	{
		SystemReset();
		PIDInit();
		PhasePIDInit();
		ResetCount += 1;

		SetOutput(0);

		if(ResetCount>WaitCount)
		{
			ResetCount=0;
//			OutValue=3000;
//			OutputDrive();
		  ProcessFlag = preStartFlag;
		}
	}
	if(ProcessFlag==preStartFlag)//预加速流程
	{
		preStartMove();//加速之前给一个初始输出，缩短加速时间
		ProcessFlag = StartFlag;
		if(NeedReset==1)
			ProcessFlag = ResetFlag;
	}
		//if((ProcessFlag==StartFlag)&&(IsNewSpeed==1))                                       //开环
	if(ProcessFlag==StartFlag)   //开环加速流程
	{
		//IsNewSpeed = 0;
		StartStopCount++;
		if(StartStopCount>50)//50平超11000；40上超11250；//60下超10650
		{
			StartStopCount=0;
			MovingMirrorSpeedUp();
		}
		////////////////////////////////////////////////////////////////////////////
		if((ReadSpeed[0]>=StartSpeedTarget)&&(ReadSpeed[1]>=StartSpeedTarget)&&(ReadSpeed[0]<=StartSpeedTarget+1000)&&(ReadSpeed[1]<=StartSpeedTarget+1000)&&(fabs(OutValue)>2000))//达到目标速度后跳出，开始PID
//		if((ReadSpeed[0]>=StartSpeedTarget)&&(ReadSpeed[1]>=StartSpeedTarget))//达到目标速度后跳出，开始PID
//		if((ReadSpeed[0]>=StartSpeedTarget)&&(ReadSpeed[1]>=StartSpeedTarget)&&(abs(OutValue)>2000))//达到目标速度后跳出，开始PID
		{
			//UartSendFlag=1;
			if(MoveDirection==MoveGoToSplitter)
			{
				//DIROUT_1;   //给采集板方向信号，高电平//PG7
				Hmfreq_PsPushMoveDir2PL(HMFREQ_HIGH);//通知PL部分动镜的移动方向1   这个值将会传入到PL，代表检测到的动镜方向

			}
			if(MoveDirection==MoveAwaySplitter)
			{

				//DIROUT_0;//给采集板方向信号，低电平
				Hmfreq_PsPushMoveDir2PL(HMFREQ_LOW);//通知PL部分动镜的移动方向为0

			}
			ProcessFlag=PIDCtrlFlag;
				//ProcessFlag=ResetFlag;
		}

		if(NeedReset==1)
			ProcessFlag = ResetFlag;

	}

	//if((ProcessFlag==StopFlag)&&(IsNewSpeed==1))
	if(ProcessFlag==StopFlag)																								//减速流程
	{
		//IsNewSpeed = 0;
		IsNewPulse = 0;
		UartSendFlag=0;

		if(MoveDirection==MoveGoToSplitter)
		{
			if(OutValue>AccStopPreset)OutValue=AccStopPreset;
			if(OutValue>-AccStopWall)//-5800
				OutValue-=GotoStopRate;		//16
			OutputDrive();
		}else if(MoveDirection==MoveAwaySplitter)
		{
			if(OutValue<-AccStopPreset)OutValue=-AccStopPreset;
			if(OutValue<AccStopWall)//6300
				OutValue+=AwayStopRate;   //15
			OutputDrive();
		}
		if(MirrorSpeed<StopSpeedTarget)//3500
		{
			//方向标志应做改变
			if(MoveDirection==MoveGoToSplitter)
				MoveDirection=MoveAwaySplitter;
			else
				MoveDirection=MoveGoToSplitter;
			//ProcessFlag=StartFlag;

//			if(MoveDirection==MoveGoToSplitter)
//			{
//				//DIROUT_1;   //给采集板方向信号，高电平//PG7
//
////				Hmfreq_PsPushMoveDir2PL(HMFREQ_LOW);//通知PL部分动镜的移动方向为0
//			}
//			if(MoveDirection==MoveAwaySplitter)
//			{
//				//Hmfreq_PsPushMoveDir2PL(HMFREQ_HIGH);//通知PL部分动镜的移动方向1
//			}

			ProcessFlag=preStartFlag;
			//ProcessFlag=ResetFlag;
		}
		if(NeedReset==1)
			ProcessFlag = ResetFlag;
	}

	//if((abs(PassedDistance)>7600)&&(ProcessFlag!=ResetFlag))
	if((fabs(PassedDistance)>4096)&&(ProcessFlag!=ResetFlag))
	{
		if(newtr<990)
		{
			if(newtest==1)
			{
				newtest=0;
				speedtestbuffer[newtr]=MirrorSpeed;
				outtestbuffer[newtr]=OutValue;
				outxbuffer[newtr]=ProcessFlag*1000;
				outybuffer[newtr]=PassedDistance;

				newtr++;
			}
		}
	}
	else
	{
		if(newtr!=0)stopcount=newtr;
		if(ProcessFlag!=DebugFlag)newtr = 0;

	}
	//if(stopcount==5)ProcessFlag = DebugFlag;

}

void TTC1SoftHandler(void *CallbackRef)
{
//	testcount1++;
//
//	if(testcount1>1000)
//	{
//		//xil_printf("2") ;
//		//printf("%d\r\n",PassedDistance);
//		printf("%d,",MirrorSpeed);
//
//		testcount1=0;
//	}

	//xil_printf("TTC1 Interrupt from CPU0\r\n") ;
	//xil_printf("Hello World%d\n\r",XTtcPs_GetCounterValue(Httc_GetTimerInst(1,0)));
	uint8_t SpeedTransmit=0;//软件开关

	////////////////////////////////////////////////////////////////////////
	Tim4Count++;
	////////////////////////******CPLD test*****///////////////////////////


	////////////////////////******串口向上位机发送动镜速度*****///////////////////////////
	if((SpeedTransmit==1)&&(ProcessFlag == PIDCtrlFlag))
	{
			UartSendFlag=0;
			//char str[50];
			//int num;
			//num = MirrorSpeed;
			//sprintf(str,"%d,%d,%d,\r\n",MirrorSpeed,PassedDistance,OutValue);
			printf("%d,",MirrorSpeed);
			//sprintf(str,"%d,\r\n",OutValue);
			//sprintf(str,"%d,",watch1);
			//sprintf(str,"%d,%d,%d,%d\r\n",PhaseX,PhaseY,XOutput,YOutput);
			//sprintf(str,"%d,",PhaseY);
			//if(MoveDirection==MoveGoToSplitter)
			//HAL_UART_Transmit(&huart1,(uint8_t*)str,strlen(str),1000);
	}
	////////////////////////******这段代码是速度不收敛/卡死/激光脉冲消失后复位的*****///////////////////////////

	if((ProcessFlag != ResetFlag)&&(ProcessFlag != ContinueFlag)&&(ProcessFlag != 100))
	{

		if((MirrorSpeed>SpeedTarget+OverSpeedCheck)&&(MirrorSpeed<SpeedTarget+15000))
			OverSpeedCount++;
		else
			OverSpeedCount = 0;

		NoPulseTimes++;

		//if((PhaseX>250)||(PhaseY>250)||(PhaseX<-250)||(PhaseY<-250))
		if((PhaseX>150)||(PhaseY>150)||(PhaseX<-150)||(PhaseY<-150))
		{
			PhaseErrorCount++;
		}
		else
			PhaseErrorCount=0;

		/////////////////////////////////////////////不过零点//////////////////////////////////////////////////////
		if(NoPassZeroDetect>NotPassedZeroDoor)
		{
			NoPassZeroDetect=0;
 			ProcessFlag = ResetFlag;
			NeedReset=1;//软件复位的第二个保险
			//////////////////////发送/////////////////////////////
			char str[50];
			sprintf(str,"NoPassZero!!!\r\n");
			xil_printf(str);
		}
		/////////////////////////////////////////////动态校正错误//////////////////////////////////////////////////////
		if((PhaseErrorCount>5000)&&(phasecheck==1))
		{
			PhaseErrorCount=0;
			ProcessFlag = ResetFlag;
			NeedReset=1;//软件复位的第二个保险
			//////////////////////发送/////////////////////////////
			char str[50];
			sprintf(str,"PhaseCtrlError!!!qwertyuiop\r\n");
			xil_printf(str);
		}
		//if((OverSpeedCount>5)||(SeizedCount>90000)||(PhaseErrorCount>1000))//3700
		/////////////////////////////////////////////超速//////////////////////////////////////////////////////
		if(OverSpeedCount>OverSpeedDoor)//3700
		{
			OutValue=0;
			OutputDrive();
			ProcessFlag = ResetFlag;
			NeedReset=1;//软件复位的第二个保险
			OverSpeedCount=0;

			//////////////////////发送/////////////////////////////
			char str[50];
			sprintf(str,"OverSpeed!!!\r\n");
			xil_printf(str);
		}


//		if((SeizedCount>10000)&&(ProcessFlag==PIDCtrlFlag))//3700//此处可能导致不能走0.1个波数
//		{
//			OverSpeedCount=0;
// 			ProcessFlag = ResetFlag;
//			//////////////////////发送/////////////////////////////
//			char str[50];
//			sprintf(str,"Seize up!!!\r\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*)str,strlen(str),1000);
//		}
//
		/////////////////////////////////////////////激光脉冲丢失//////////////////////////////////////////////////////
		if(NoPulseTimes>NoPulseTimesDoor)//400
		{
			OutValue=0;
			OutputDrive();
			//ProcessFlag = DebugFlag;
			ProcessFlag = ResetFlag;
			NeedReset=1;//软件复位的第二个保险
			//////////////////////发送/////////////////////////////
			char str[50];
			sprintf(str,"NoPulse!!!\r\n");
			xil_printf(str);
		}
	}
}

void LaserSoftHandler(void *CallbackRef)
{

//	xil_printf("Laser Interrupt from CPU0\r\n") ;
//  xil_printf("Hello World%d\n\r",XTtcPs_GetCounterValue(Httc_GetTimerInst(1,0)));
	//激光脉冲中断

//	xil_printf("Laser Soft Interrupt \r\n") ;


	IsNewPulse=1;                                             //接收到新的脉冲
	IsNewSpeed++;                                             //更新速度值
	IsNewPhase=1;                                             //更新相位值
	newtest=1;

	NoPulseTimes = 0;                                         //接收到新的脉冲

	MirrorSpeed=LaserFreqMeasure();                       //计算动镜速度

	ReadSpeed[0]=ReadSpeed[1];
	ReadSpeed[1]=MirrorSpeed;

	Error = SpeedTarget - MirrorSpeed;                          //计算速度偏差


	//watch3 = PulsePeriodY;
	if (MoveDirection==MoveGoToSplitter)
	{
		PassedDistance+=1;                                        //更新动镜坐标
		//XError=PhaseX;
		//YError=PhaseY;
	}
	else
	{
		PassedDistance-=1;                                        //更新动镜坐标
		//XEr、32ror=0 - PhaseX;
		//YError=0 - PhaseY;
	}

	if((ProcessFlag==PIDCtrlFlag)&&(IsNewSpeed>=PIDDoor))                          //PID速度调节流程
	{
		//UartSendFlag=1;
		IsNewSpeed = 0;
		IsNewPulse = 0;
		PIDSpeedCtrl();

		if((MirrorSpeed<HitWallCheck)&&(PositionResetFlag == 0)&&(PassedDistance>500))                         //速度下降，认定为撞墙，开始反向运动3000//1000//4000
		{
			OutValue=0;
			OutputDrive();

//				char str[50];

//				sprintf(str,"%d,%d,%d,\r\n",MirrorSpeed,PassedDistance,OutValue);
//

//				HAL_UART_Transmit(&huart1,(uint8_t*)str,strlen(str),1000);
//
//				ProcessFlag = ResetFlag;
//				NeedReset=1;//软件复位的第二个保险

			if(MoveDirection==MoveGoToSplitter)
			{
				MoveDirection=MoveAwaySplitter;
				PositionResetFlag = 1;
			}
			else
			{
				MoveDirection=MoveGoToSplitter;
				PositionResetFlag = 0;
			}
			//ProcessFlag=ContinueFlag;
			ProcessFlag=preStartFlag;
			//ProcessFlag=StartFlag;
            //PositionResetFlag = 1;
		}

		if(PositionResetFlag==1)                                               //如果已经开始正常运动，且经过了光电开关，那么将依据运动行程换向
		{
			if((MoveDirection==MoveGoToSplitter)&&(PassedDistance>(LimitCompensation +GotoLimit))&&(ZeroArrivedFlag))
			{
				ProcessFlag=StopFlag;
				NoPassZeroDetect++;
				ZeroChecked=0;
				//ProcessFlag=ResetFlag;
			}
			if((MoveDirection==MoveAwaySplitter)&&(PassedDistance<(LimitCompensation-AwayLimit))&&(ZeroArrivedFlag))
			{
				ProcessFlag=StopFlag;
				NoPassZeroDetect++;
				ZeroChecked=0;
				//ProcessFlag=ResetFlag;
			}
		}

		if(NeedReset==1)
			ProcessFlag = ResetFlag;
	}

//	if((MirrorSpeed < 20000)&&(MirrorSpeed > 5000)&&(phaseflag)&&(PositionResetFlag))//1500//2000
//	{
//		//PIDPhaseCtrl();
//	}

}

