//Organized by LinZp
#include <stdio.h>
#include "string.h"
#include "sleep.h"
#include "xil_printf.h"
#include "platform.h"
#include "Htest.h"
#include "xparameters.h"

#include "xil_io.h"


#include "platform.h"
#include "platform_config.h"
#if defined (__arm__) || defined(__aarch64__)

#include "xil_printf.h"
#endif

#include "xil_cache.h"
#include "PID.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "HTimer.h"
#include "Haxi_pwm.h"
#include "Hdac7631.h"
#include "Hmfreq.h"
#include "xparameters.h"
#include "xscugic.h"
#include "xgpio.h"
#include "math.h"
#include "xil_printf.h"
#include "xil_exception.h"
#include "xil_mmu.h"
#include "sleep.h"
#include "xtime_l.h"

#include "Htas5111.h"
#include "TAS5111.h"
#include "xparameters.h"
#include "xil_io.h"


extern XScuTimer LinTimerInstance;	/* Cortex A9 Scu Private Timer Instance */
extern XScuGic LinIntcInstance;		/* Interrupt Controller Instance */


#define LaserPeriority 0
#define TTC0Periority 8
#define TTC1Periority 32
#define ZeroCheckPeriority 0


void SoftHandler(void *CallbackRef) ;
void TTC0SoftHandler(void *CallbackRef) ;
void TTC1SoftHandler(void *CallbackRef) ;
void LaserSoftHandler(void *CallbackRef) ;
void ZeroCheckSoftHandler(void *CallbackRef) ;
///////////////////////////////***********************动镜控制模块******************************//////////////////////////////////
//系统宏定义

#define ResetFlag                    0                 //复位标志位
#define StartFlag                    1                 //起动加速标志位
//#define PIDCalFlag                 2           		   //PID计算
#define StopFlag                     3         				 //减速标志位
#define preStartFlag                 4                 //加速准备
#define PIDCtrlFlag                  5
#define DebugFlag                    6                 //调试标志位
#define ContinueFlag                 7                 //起动标志位
//#define PIDCalFlagt                8           		   //PID结算控制
//#define StopFlagt                  9         				 //减速标志位
//#define preStartFlagt              10                 //加速准备
//#define PIDCtrlFlagt               11
//#define DebugFlagt                 12
#define MoveGoToSplitter             1      					 // 朝向分束器方向
#define MoveAwaySplitter             2     						 // 远离分束器方向




void SetOutput(int compare);
void SystemReset(void);
void PWMOutInit(void);
void LaserFreqMeasureInit(void);
int LaserFreqMeasure(void);



extern uint32_t ResetCount;
extern uint8_t ProcessFlag;
extern unsigned int MirrorSpeed;
extern int Error;
extern int XError;
extern int YError;
extern int StartSpeedTarget;
extern uint8_t ProcessFlag;
extern uint8_t MoveDirection;
extern int PassedDistance;
extern int SpeedTarget;
extern short ZeroOffset;
extern unsigned char IsNewPulse;
extern unsigned char IsNewSpeed;
extern unsigned char IsNewPhase;
extern unsigned int NoPulseTimes;
extern int OutValue;
extern int XOutput;
extern int YOutput;
extern int GotoLimit;
extern int AwayLimit;
extern uint8_t	Uart1_Data;
extern int ReadSpeed[3];
extern int StartStopCount;
extern uint8_t	Uart1_Data_buffer[6];
extern uint8_t Uart1_Trans_buffer[20];
extern int UartSendFlag;
extern int StopSpeedTarget;
extern uint8_t Speed_Class;
extern uint8_t Resolution_Class;
extern int OverSpeedCount;
extern int PhaseErrorCount;
extern int Output;
extern int OutValueMax;
extern int PhaseCtrlMax;
extern int PasseddistanceRevise;
extern int PasseddistanceRevisedFlag;
extern int LimitCompensation;
extern int ZeroArrivedFlag;
extern int ZeroChecked;
extern int PhaseX;
extern int PhaseY;
extern int PositionResetFlag;
extern int NoPassZeroDetect;
extern int NeedReset;
extern int watch1,watch2,watch3,watch4;
extern int speedtestbuffer[1000];
extern int outtestbuffer[1000];
extern int outxbuffer[1000];
extern int outybuffer[1000];
extern int newtr;
extern int newtest;
extern int stopcount;
extern int PIDDoor;
extern int GatherDirection;
extern int WaitCount;
extern int GotoAccRate;
extern int GotoStopRate;
extern int AwayAccRate;
extern int AwayStopRate;
extern int OverSpeedCheck;
extern int OverSpeedDoor;
extern int NoPulseTimesDoor;
extern int NotPassedZeroDoor;
extern int HitWallCheck;
extern int phaseflag;
extern int AccStopPreset;
extern int AccStopWall;
extern int phasecheck;








































///////////////////////////////*************************************************************//////////////////////////////////
