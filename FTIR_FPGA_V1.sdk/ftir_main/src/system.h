//Organized by LinZp
#include <stdio.h>
#include "string.h"
#include "Ethernet.h"
#include "xparameters.h"

#include "Hak5394.h"
#include "xil_io.h"

#include "netif/xadapter.h"

#include "platform.h"
#include "platform_config.h"
#if defined (__arm__) || defined(__aarch64__)
#include "xil_printf.h"
#endif

#include "lwip/tcp.h"
#include "xil_cache.h"

#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "HTimer.h"

#include "Hdac7631.h"
#include "Hmfreq.h"
#include "Hcfg_sig.h"
#include "Htas5111.h"

#if LWIP_IPV6==1
#include "lwip/ip.h"
#else
#if LWIP_DHCP==1
#include "lwip/dhcp.h"
#endif
#endif
#include "main.h"



#define TIMER_LOAD_VALUE XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 8

////////////////////////////////////////////////////////////////////////////////////
////////////动镜控制相关

//#define LaserPeriority 8
//#define TTC0Periority 16
//#define TTC1Periority 32
//#define ZeroCheckPeriority 8
#define LaserPeriority 0
#define TTC0Periority 0
#define TTC1Periority 0
#define ZeroCheckPeriority 0


extern u32 CPU0;
extern u32 CPU1;

extern u16 SoftIntrIdToCpu0 ;
extern u16 SoftIntrIdToCpu1 ;
extern u16 TTC0SoftIntrIdToCpu1 ;
extern u16 TTC1SoftIntrIdToCpu1 ;
extern u16 LaserSoftIntrIdToCpu1;
extern u16 ZeroCheckIntrIdToCpu1;

void Laser_SwitchIntrInit();//激光和光电开关的中断初始化函数
///////////////////////////////////////////////////////////////////////////////////

extern struct tcp_pcb *Mypcb;

//extern unsigned char Lwipram[1000000];
//extern unsigned char LwipramBack[1000000];
extern unsigned char *Lwipram;
extern unsigned char *LwipramBack;

extern int LwipForwordNum;
extern int LwipBackwordNum;
extern int ForwordNeedSendFlag;
extern int BackwordNeedSendFlag;
extern int ForwordSendPermission;
extern int BackwordSendPermission;

//////////////////////////////////////////////////////////////LWIP接收缓冲区///////////////////////////////////
extern unsigned char LwipRevBuf[100];
extern int LwipRevNum;
extern int LwipRevIndex;


extern int SysFlag;
extern int TaskFlag;
u32 SystemStatus;
extern int MirrorDirection;

extern int DirSignal[3];
extern int LaserSignal[3];

extern int  ForwordSpec[1048576];
extern u32 ForwordSpecNum;
extern u32 ForwordSpeed[1048576];
extern u32 ForwordSpeedNum;
extern int BackwordSpec[1048576];
extern u32 BackwordSpecNum;
extern u32 BackwordSpeed[1048576];
extern u32 BackwordSpeedNum;

extern unsigned char MsgHead[24];
extern unsigned char MsgTail[4];

extern float ADData;
extern int LwipBusyFlag;
extern int PCNeedDataFlag;
extern int ErrorDataNum;
extern u32 ClkValue;
u32 ForwordTime[1048576];
u32 BackwordTime[1048576];

extern int TimerFull;

extern XScuTimer LinTimerInstance;	/* Cortex A9 Scu Private Timer Instance */
extern XScuGic LinIntcInstance;		/* Interrupt Controller Instance */

extern XUartPs Uart;

void TaskManager(void);
void ADDataAcquire(void);
void MakeTcpHead(void);
u32 MakeSpecTcpData(void);
int LiwpScuTimerIntr(void);
void axi_pwm_init(u32 T,u32 GT);

