#ifndef __PID_H__
#define __PID_H__


/***************结构体定义**************************************************/
typedef struct PIDParameter
{
 uint32_t resolution;
 uint32_t speedtarget;
 short zero_offset;
}	PIDParameter;
//typedef struct PIDIndex 
//{	
//	float Proportion; //比例常数 Proportional Const
//	float Integral;   //积分常数 Integral Const
//	float Derivative; //微分常数 Derivative Const	
//} PIDIndex;
typedef struct PIDError 
{	
	int    LastError;  // Error[-1]
	int    PrevError;  // Error[-2]
	int    SumError;   // Sums of Errors	
	int    Std;   // 二次形指标函数的值//added by Kun
} PIDError;

/*************************************************/
void ControlIint(void);
void PIDReset(void);
void delay(unsigned int tim);
void PIDInit(void);
void PIDCalc(void);
void PhaseXPIDCalc(void);
void PhaseYPIDCalc(void);
void OutputDrive(void);
int  PIDSpeedCtrl(void);
int PIDPhaseCtrl(void);
void PhasePIDInit(void);

void preStartMove(void);
void MovingMirrorSpeedUp(void);
void StopMove(void);
void ReturnToGoToSplitter(void);
void ReturnToAwaySplitter(void);
#endif
