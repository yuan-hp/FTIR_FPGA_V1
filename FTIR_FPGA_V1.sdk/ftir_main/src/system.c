//Organized by LinZp
#include "system.h"
////////////////////////////////////////////////////////////////////////////////////
////////////动镜控制相关

u32 CPU0 = 0x1 ;
u32 CPU1 = 0x2 ;

u16 SoftIntrIdToCpu0 = 1 ;
u16 SoftIntrIdToCpu1 = 2 ;
u16 TTC0SoftIntrIdToCpu1 = 3 ;
u16 TTC1SoftIntrIdToCpu1 = 4 ;
u16 LaserSoftIntrIdToCpu1 = 5 ;
u16 ZeroCheckIntrIdToCpu1 = 6 ;

//初始化PL指示灯
void axi_pwm_init(u32 T,u32 GT)
{
	AX_PWM_mWriteReg(XPAR_AX_PWM_0_S00_AXI_BASEADDR,AX_PWM_S00_AXI_SLV_REG0_OFFSET,T);
	AX_PWM_mWriteReg(XPAR_AX_PWM_0_S00_AXI_BASEADDR,AX_PWM_S00_AXI_SLV_REG1_OFFSET,GT);
}

void Laser_SwitchIntrInit()//激光和光电开关的中断初始化函数
{
	Hcfg_sig_init();
	axi_pwm_init((u32)150000000*2,150000000*2/2);
	Hcfg_EnableLaserInt();
	Hcfg_EnableSwitchInt();

}



///////////////////////////////////////////////////////////////////////////////////

struct tcp_pcb *Mypcb;
//////////////////////////////////////////////////////////////LWIP发送缓冲区///////////////////////////////////
//unsigned char Lwipram[1000000];
//unsigned char LwipramBack[1000000];


unsigned char *Lwipram=(unsigned char *) LWIP_SEND_BUFFER_FORWARD;//
unsigned char *LwipramBack=(unsigned char *) LWIP_SEND_BUFFER_BACKWARD;//
XUartPs Uart;

int LwipForwordNum=0;
int LwipBackwordNum=0;
int ForwordNeedSendFlag=0;
int BackwordNeedSendFlag=0;
int ForwordSendPermission=1;
int BackwordSendPermission=1;




//////////////////////////////////////////////////////////////LWIP接收缓冲区///////////////////////////////////
unsigned char LwipRevBuf[100];
int LwipRevNum=0;
int LwipRevIndex=0;

//////////////////////////////////////////////////////////////LWIP发送缓冲区///////////////////////////////////
int ForwordSpec[1048576];
u32 ForwordTime[1048576];
u32 ForwordSpecNum=0;                            //实际数字

u32 ClkValue=0;
u32 ForwordSpeed[1048576];
u32 ForwordSpeedNum=0;

int BackwordSpec[1048576];
u32 BackwordTime[1048576];
u32 BackwordSpecNum=0;

u32 BackwordSpeed[1048576];
u32 BackwordSpeedNum=0;

unsigned char MsgHead[24];
unsigned char MsgTail[4]={'T','a','i','l'};

int SysFlag=0;
int TaskFlag=0;
int DataReadyFlag=0;
int LwipBusyFlag=0;
int PCNeedDataFlag=0;

u32 SystemStatus;
//int DirSignal[3]={7,7,7};
//int LaserSignal[3]={7,7,7};
int MirrorDirection=0;//定义0为正向，1为负向
float ADData;

int ErrorDataNum=0;
int TimerFull=0;


XScuTimer LinTimerInstance;	/* Cortex A9 Scu Private Timer Instance */
XScuGic LinIntcInstance;		/* Interrupt Controller Instance */

void LinLwipMsgCpy(unsigned int index, unsigned char *source, unsigned char *dest,unsigned int length)
{

	for (int i = 0; i < length; i++)
	{
		dest[index + i] = source[i];
	}

}

void MakeTcpHead(void)
{
	u32 SpeedNumTemp=0;
	u32 SpecNumTemp=0;
	if(MirrorDirection==0)
	{
		SpeedNumTemp = ForwordSpeedNum*4;
		SpecNumTemp = ForwordSpecNum*4;
	}
	if(MirrorDirection==1)
	{
		SpeedNumTemp = BackwordSpeedNum*4;
		SpecNumTemp = BackwordSpecNum*4;
	}
	MsgHead[4]= (unsigned char)MirrorDirection;

	MsgHead[12]=(unsigned char)(SpecNumTemp%256);SpecNumTemp/=256;
	MsgHead[13]=(unsigned char)(SpecNumTemp%256);SpecNumTemp/=256;
	MsgHead[14]=(unsigned char)(SpecNumTemp%256);SpecNumTemp/=256;
	MsgHead[15]=(unsigned char)(SpecNumTemp);

	MsgHead[16]=(unsigned char)(SpeedNumTemp%256);SpeedNumTemp/=256;
	MsgHead[17]=(unsigned char)(SpeedNumTemp%256);SpeedNumTemp/=256;
	MsgHead[18]=(unsigned char)(SpeedNumTemp%256);SpeedNumTemp/=256;
	MsgHead[19]=(unsigned char)(SpeedNumTemp);

}

u32 MakeSpecTcpData(void)
{
	u32 datanum=0;

	if(MirrorDirection==0)//正向数据
	{
		datanum = 24+ForwordSpecNum*4*2+ForwordSpeedNum*4+4;

		LinLwipMsgCpy(0,MsgHead,Lwipram,24);
		LinLwipMsgCpy(24,(unsigned char*)ak5394_m1,Lwipram,ForwordSpecNum*4);
		//LinLwipMsgCpy(24,(unsigned char*)ak5394_laser_t1,ForwordSpecNum*4);hpy

		LinLwipMsgCpy(24+ForwordSpecNum*4,(unsigned char*)AK_5394_TIME1_BASE_ADDR,Lwipram,ForwordSpecNum*4);
		LinLwipMsgCpy(24+ForwordSpecNum*4*2,(unsigned char*)AK_5394_LASER_PULSE_TIME1_BASE_ADDR,Lwipram,ForwordSpeedNum*4);
//		LinLwipMsgCpy(24+ForwordSpecNum*4,(unsigned char*)ForwordTime,ForwordSpecNum*4);
//		LinLwipMsgCpy(24+ForwordSpecNum*4*2,(unsigned char*)ForwordSpeed,ForwordSpeedNum*4);
		LinLwipMsgCpy(24+ForwordSpecNum*4*2+ForwordSpeedNum*4,MsgTail,Lwipram,4);

		ForwordSpecNum=0;
		ForwordSpeedNum=0;

	}
	if(MirrorDirection==1)//反向数据
	{
		datanum = 24+BackwordSpecNum*4*2+BackwordSpeedNum*4+4;
		LinLwipMsgCpy(0,MsgHead,LwipramBack,24);
		LinLwipMsgCpy(24,(unsigned char*)ak5394_m2,LwipramBack,BackwordSpecNum*4);
		//LinLwipMsgCpy(24,(unsigned char*)ak5394_laser_t2,ForwordSpecNum*4);//hpy
		LinLwipMsgCpy(24+BackwordSpecNum*4,(unsigned char*)AK_5394_TIME2_BASE_ADDR,LwipramBack,BackwordSpecNum*4);

		LinLwipMsgCpy(24+BackwordSpecNum*4*2,(unsigned char*)AK_5394_LASER_PULSE_TIME2_BASE_ADDR,LwipramBack,BackwordSpeedNum*4);
		LinLwipMsgCpy(24+BackwordSpecNum*4*2+BackwordSpeedNum*4,MsgTail,LwipramBack,4);

		BackwordSpecNum=0;
		BackwordSpeedNum=0;

	}
	return datanum;
}



