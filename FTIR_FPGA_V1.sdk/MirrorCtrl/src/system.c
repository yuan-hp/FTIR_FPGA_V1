//Organized by LinZp
#include "system.h"

XScuTimer LinTimerInstance;	/* Cortex A9 Scu Private Timer Instance */
XScuGic LinIntcInstance;		/* Interrupt Controller Instance */

//////////************************************************��������ģ��*******************************************************************///////////
//ϵͳȫ�ֱ���
///////////////////////////////////////////////////////////////////////////
int Tim4Count = 0;//tim4�жϼ�����������
int testcount = 0;
int testcount1 = 0;


/////////////////////////////////////////////////////������////////////////////////////////////////////////////////////
uint8_t Speed_Class=4;                              //�ٶȵ�λ
uint8_t Resolution_Class=20;                           //�ֱ��ʵ�λ
int phaseflag=0;                                       //�Ƿ���ж�̬У��
int phasecheck=0;                                      //�Ƿ���ж�̬У����鸴λ
int GatherDirection=1;                                 //���ÿ���ɨ��ķ���0��1��2
int WaitCount=600000;//4000000
//////////////default//////////////////

//uint8_t Speed_Class=4;                              //�ٶȵ�λ
//uint8_t Resolution_Class=40;                           //�ֱ��ʵ�λ
//int phaseflag=1;                                       //�Ƿ���ж�̬У��
//int phasecheck=1;                                      //�Ƿ���ж�̬У����鸴λ
//int GatherDirection=1;                                 //���ÿ���ɨ��ķ���0��1��2
//int WaitCount=600000;

///////////////////////////////////////////////////������////////////////////////////////////////////////////////
unsigned int MirrorSpeed;          									   //�����ٶ�
int PassedDistance;			                               //��ǰλ�þ������̵㣬�������崥���жϴ�����Ҳ��������
unsigned char IsNewPulse;                              //�Ƿ������������
unsigned char IsNewSpeed;                              //�ٶ��Ƿ����
unsigned char IsNewPhase;                              //��λ�Ƿ����
uint8_t MoveDirection;                                 //�����˶������־λ
int ReadSpeed[3];                                      //��¼3������
unsigned int NoPulseTimes;                             //���������
int ZeroArrivedFlag=0;                                 //�����㾭����־λ
int ZeroChecked=0;
int PhaseX=0;                                          //һ���������λ��
int PhaseY=0;                                          //��һ�������ϵ���λ��
int StartStopCount;                                    //���ƼӼ��ٵ�Ƶ��
int PasseddistanceRevise=0;                            //��翪�����е���ƫ������˴�����
int PasseddistanceRevisedFlag=0;                       //�Ƿ���ɹ�翪�����е���ƫ�����־λ
/////////////////////////////////////////////////////������//////////////////////////////////////////////////////////////////////////
int SpeedTarget;															         //�趨��ɨ���ٶ�ֵ
int StartSpeedTarget;                                  //�������̵�Ŀ���ٶ�
int StopSpeedTarget;                                   //�������̵�Ŀ���ٶ�
short ZeroOffset;                                      //���ƫ��
int UartSendFlag;                                      //���ƴ��ڷ���ʱ��
int GotoAccRate;
int GotoStopRate;
int AwayAccRate;
int AwayStopRate;
int HitWallCheck;
int AccStopPreset;
int AccStopWall;
////////////////////////////////////////////////////////������//////////////////////////////////////////////////////////////////
uint8_t ProcessFlag;																	 //Process��־λ
uint32_t ResetCount;                                   //��λ��ʱ������־λ
int OutValue=0;                                        //�����ռ�ձ�,���ƶ���
int XOutput=0;																				 //�����ռ�ձȣ���̬У��
int YOutput=0;																				 //�����ռ�ձȣ���̬У��
int GotoLimit;                                         //�������������г�
int AwayLimit;                                         //Զ����������г�
int OutValueMax;                                       //ռ�ձȵ����ֵ
int PhaseCtrlMax;
uint8_t	Uart1_Data_buffer[6];                          //���ڽ��ջ�����
uint8_t	Uart1_Data;                                    //����ʵ�е��ַ�����
uint8_t Uart1_Trans_buffer[20];                        //���ڷ��ͻ�����
int LimitCompensation=0;                               //�ڴ˳����У��ǹ�翪����������Ĳ���������ͨ����������
int PIDDoor;
///////////////////////////////////////////////////////////������λ/////////////////////////////////////////////////////////////////////
int PositionResetFlag = 0;                             //������λ�󶯾��������Ҷ���ɱ�־λ
int NoPassZeroDetect = 0;                              //����Ƿ�ͨ��������
int NeedReset = 0;                                     //������λ���
int OverSpeedCount=0;                                  //���ټ���
int PhaseErrorCount=0;                                 //��λУ���������
int OverSpeedCheck;
int OverSpeedDoor;
int NoPulseTimesDoor;
int NotPassedZeroDoor;
///////////////////////////////////////////////////////////����ר��/////////////////////////////////////////////////////////////////////
int watch1=0,watch2=0,watch3=0,watch4=0;
int speedtestbuffer[1000];
int outtestbuffer[1000];
int outxbuffer[1000];
int outybuffer[1000];
int newtr=0;
int newtest=0;
int stopcount=0;
/******************************************************************************/
//                               ����ռ�ձȵĺ���
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
void SetOutput(int compare)       //���ڶ���
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
//                               �����������ڲ�������
/*******************************************************************************/

void LaserFreqMeasureInit(void)
{
	Hmfreq_init();
}


int LaserFreqMeasure(void)
{
	return 148148163/Hmfreq_GetCycleT();//��ȡ����ļ���ֵ;
}


/******************************************************************************/
//                               ϵͳ��λ
/*******************************************************************************/
void SystemReset()
{

	SetOutput(0);                                  //ֹͣ����


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
	{		                                     //������
		//if(MoveDirection==MoveAwaySplitter)                       //�˴�����Ӧ��ʹ��PositionResetFlag��Ϊ�оݣ������ɴ˿���������ŵĹ��ĳЩ��Ӳ�����д���
		ZeroArrivedFlag = 1;                                    //ȷ�������翪�أ���ʼ�����г̻���
	}

	xil_printf("zeor_checked");

}
void TTC0SoftHandler(void *CallbackRef)//���ƶ�ʱ��
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
	if(ProcessFlag==DebugFlag)  //����ģʽ����ȥ�˾ͳ������������ź���ʧʱ�����
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
	if(ProcessFlag==ResetFlag)  //��λ���̣����г�ʼ��������һ����ʱ������
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
	if(ProcessFlag==preStartFlag)//Ԥ��������
	{
		preStartMove();//����֮ǰ��һ����ʼ��������̼���ʱ��
		ProcessFlag = StartFlag;
		if(NeedReset==1)
			ProcessFlag = ResetFlag;
	}
		//if((ProcessFlag==StartFlag)&&(IsNewSpeed==1))                                       //����
	if(ProcessFlag==StartFlag)   //������������
	{
		//IsNewSpeed = 0;
		StartStopCount++;
		if(StartStopCount>50)//50ƽ��11000��40�ϳ�11250��//60�³�10650
		{
			StartStopCount=0;
			MovingMirrorSpeedUp();
		}
		////////////////////////////////////////////////////////////////////////////
		if((ReadSpeed[0]>=StartSpeedTarget)&&(ReadSpeed[1]>=StartSpeedTarget)&&(ReadSpeed[0]<=StartSpeedTarget+1000)&&(ReadSpeed[1]<=StartSpeedTarget+1000)&&(fabs(OutValue)>2000))//�ﵽĿ���ٶȺ���������ʼPID
//		if((ReadSpeed[0]>=StartSpeedTarget)&&(ReadSpeed[1]>=StartSpeedTarget))//�ﵽĿ���ٶȺ���������ʼPID
//		if((ReadSpeed[0]>=StartSpeedTarget)&&(ReadSpeed[1]>=StartSpeedTarget)&&(abs(OutValue)>2000))//�ﵽĿ���ٶȺ���������ʼPID
		{
			//UartSendFlag=1;
			if(MoveDirection==MoveGoToSplitter)
			{
				//DIROUT_1;   //���ɼ��巽���źţ��ߵ�ƽ//PG7
				Hmfreq_PsPushMoveDir2PL(HMFREQ_HIGH);//֪ͨPL���ֶ������ƶ�����1   ���ֵ���ᴫ�뵽PL��������⵽�Ķ�������

			}
			if(MoveDirection==MoveAwaySplitter)
			{

				//DIROUT_0;//���ɼ��巽���źţ��͵�ƽ
				Hmfreq_PsPushMoveDir2PL(HMFREQ_LOW);//֪ͨPL���ֶ������ƶ�����Ϊ0

			}
			ProcessFlag=PIDCtrlFlag;
				//ProcessFlag=ResetFlag;
		}

		if(NeedReset==1)
			ProcessFlag = ResetFlag;

	}

	//if((ProcessFlag==StopFlag)&&(IsNewSpeed==1))
	if(ProcessFlag==StopFlag)																								//��������
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
			//�����־Ӧ���ı�
			if(MoveDirection==MoveGoToSplitter)
				MoveDirection=MoveAwaySplitter;
			else
				MoveDirection=MoveGoToSplitter;
			//ProcessFlag=StartFlag;

//			if(MoveDirection==MoveGoToSplitter)
//			{
//				//DIROUT_1;   //���ɼ��巽���źţ��ߵ�ƽ//PG7
//
////				Hmfreq_PsPushMoveDir2PL(HMFREQ_LOW);//֪ͨPL���ֶ������ƶ�����Ϊ0
//			}
//			if(MoveDirection==MoveAwaySplitter)
//			{
//				//Hmfreq_PsPushMoveDir2PL(HMFREQ_HIGH);//֪ͨPL���ֶ������ƶ�����1
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
	uint8_t SpeedTransmit=0;//��������

	////////////////////////////////////////////////////////////////////////
	Tim4Count++;
	////////////////////////******CPLD test*****///////////////////////////


	////////////////////////******��������λ�����Ͷ����ٶ�*****///////////////////////////
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
	////////////////////////******��δ������ٶȲ�����/����/����������ʧ��λ��*****///////////////////////////

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

		/////////////////////////////////////////////�������//////////////////////////////////////////////////////
		if(NoPassZeroDetect>NotPassedZeroDoor)
		{
			NoPassZeroDetect=0;
 			ProcessFlag = ResetFlag;
			NeedReset=1;//������λ�ĵڶ�������
			//////////////////////����/////////////////////////////
			char str[50];
			sprintf(str,"NoPassZero!!!\r\n");
			xil_printf(str);
		}
		/////////////////////////////////////////////��̬У������//////////////////////////////////////////////////////
		if((PhaseErrorCount>5000)&&(phasecheck==1))
		{
			PhaseErrorCount=0;
			ProcessFlag = ResetFlag;
			NeedReset=1;//������λ�ĵڶ�������
			//////////////////////����/////////////////////////////
			char str[50];
			sprintf(str,"PhaseCtrlError!!!qwertyuiop\r\n");
			xil_printf(str);
		}
		//if((OverSpeedCount>5)||(SeizedCount>90000)||(PhaseErrorCount>1000))//3700
		/////////////////////////////////////////////����//////////////////////////////////////////////////////
		if(OverSpeedCount>OverSpeedDoor)//3700
		{
			OutValue=0;
			OutputDrive();
			ProcessFlag = ResetFlag;
			NeedReset=1;//������λ�ĵڶ�������
			OverSpeedCount=0;

			//////////////////////����/////////////////////////////
			char str[50];
			sprintf(str,"OverSpeed!!!\r\n");
			xil_printf(str);
		}


//		if((SeizedCount>10000)&&(ProcessFlag==PIDCtrlFlag))//3700//�˴����ܵ��²�����0.1������
//		{
//			OverSpeedCount=0;
// 			ProcessFlag = ResetFlag;
//			//////////////////////����/////////////////////////////
//			char str[50];
//			sprintf(str,"Seize up!!!\r\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*)str,strlen(str),1000);
//		}
//
		/////////////////////////////////////////////�������嶪ʧ//////////////////////////////////////////////////////
		if(NoPulseTimes>NoPulseTimesDoor)//400
		{
			OutValue=0;
			OutputDrive();
			//ProcessFlag = DebugFlag;
			ProcessFlag = ResetFlag;
			NeedReset=1;//������λ�ĵڶ�������
			//////////////////////����/////////////////////////////
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
	//���������ж�

//	xil_printf("Laser Soft Interrupt \r\n") ;


	IsNewPulse=1;                                             //���յ��µ�����
	IsNewSpeed++;                                             //�����ٶ�ֵ
	IsNewPhase=1;                                             //������λֵ
	newtest=1;

	NoPulseTimes = 0;                                         //���յ��µ�����

	MirrorSpeed=LaserFreqMeasure();                       //���㶯���ٶ�

	ReadSpeed[0]=ReadSpeed[1];
	ReadSpeed[1]=MirrorSpeed;

	Error = SpeedTarget - MirrorSpeed;                          //�����ٶ�ƫ��


	//watch3 = PulsePeriodY;
	if (MoveDirection==MoveGoToSplitter)
	{
		PassedDistance+=1;                                        //���¶�������
		//XError=PhaseX;
		//YError=PhaseY;
	}
	else
	{
		PassedDistance-=1;                                        //���¶�������
		//XEr��32ror=0 - PhaseX;
		//YError=0 - PhaseY;
	}

	if((ProcessFlag==PIDCtrlFlag)&&(IsNewSpeed>=PIDDoor))                          //PID�ٶȵ�������
	{
		//UartSendFlag=1;
		IsNewSpeed = 0;
		IsNewPulse = 0;
		PIDSpeedCtrl();

		if((MirrorSpeed<HitWallCheck)&&(PositionResetFlag == 0)&&(PassedDistance>500))                         //�ٶ��½����϶�Ϊײǽ����ʼ�����˶�3000//1000//4000
		{
			OutValue=0;
			OutputDrive();

//				char str[50];

//				sprintf(str,"%d,%d,%d,\r\n",MirrorSpeed,PassedDistance,OutValue);
//

//				HAL_UART_Transmit(&huart1,(uint8_t*)str,strlen(str),1000);
//
//				ProcessFlag = ResetFlag;
//				NeedReset=1;//������λ�ĵڶ�������

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

		if(PositionResetFlag==1)                                               //����Ѿ���ʼ�����˶����Ҿ����˹�翪�أ���ô�������˶��г̻���
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
