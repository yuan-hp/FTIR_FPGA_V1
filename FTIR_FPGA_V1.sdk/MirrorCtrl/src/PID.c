#include "system.h"
/***************************************************************************/
//���ļ����ø����Ǳ��0321A,Ħ�����ϴ�


/***************PID��������**************************************************/

int dError=0, Error=0, SubError=0,Total=0;

int	LastError   ;
int	PrevError   ;
int	SumError    ;

int	Proportion;
int	Integral  ;
int	Derivative;

int MechanicalZeroOffset;	           //��е��㣨ƽ��㣩�͹��ԭ��ƫ�ƣ����ڴ˶������ã�
int Coefficient;                     //480���Զ��ɱ���ϵ��             �����ڴ˶������ã�

/****************************************************��λPID��������**************************************************/
int dXError=0, XError=0, SubXError=0,XTotal=0;

int	LastXError   ;
int	PrevXError   ;
int	SumXError    ;

int	XProportion;
int	XIntegral  ;
int	XDerivative;

//int XOutput;
////////////////////////////////////////////////
int dYError=0, YError=0, SubYError=0,YTotal=0;

int	LastYError   ;
int	PrevYError   ;
int	SumYError    ;

int	YProportion;
int	YIntegral  ;
int	YDerivative;

//int YOutput;


/***********************************************/
/*PID�����㷨�ܳ���� ��ʼ��               */
/***********************************************/


void PIDInit(void)
{
	OverSpeedCheck=5000;//5000
	OverSpeedDoor=300;//300
	NoPulseTimesDoor=500;//500
	NotPassedZeroDoor=6;
	
	AccStopPreset=5000;
	AccStopWall=6300;
	
	
	HitWallCheck=3000;
	
	switch(Resolution_Class)
	{
		case 3:////////////////////////////////0.125��
			GotoLimit=32768;
			AwayLimit=131072;
			break;
		case 4:////////////////////////////////0.25��
			GotoLimit=32768;
			AwayLimit=65536;
			break;
		case 5:////////////////////////////////0.5��
			GotoLimit=32768;
			AwayLimit=32768;
			break;
		case 10:
			GotoLimit=16384;
			AwayLimit=16384;				
		break;
		case 20:
			GotoLimit=8192;
			AwayLimit=8192;
		break;
		case 40:
			GotoLimit=4096+256;
			AwayLimit=4096+256;				
		break;
		case 80:
			GotoLimit=2048;
			AwayLimit=2048;				
		break;
		case 160:
			GotoLimit=1024;
			AwayLimit=1024;				
		break;
		}
	
	switch(Speed_Class)
	{
		case 2:
			SpeedTarget 				 = 40000;
			PIDDoor                      = 5;//4
			Proportion                   = 350;//450
			Integral                     = 10;
			Derivative                   = 150;//250
			StartSpeedTarget			 = 40000;
			StopSpeedTarget              = 4000;
			GotoAccRate=10;//15
			GotoStopRate=16;//15
			AwayAccRate=13;//15
			AwayStopRate=15;//15
		break;
		case 3:
			SpeedTarget 				 = 30000;
			PIDDoor                      = 2;
			Proportion                   = 600;//250��300;//310;//300;//150;//1000;//ֻ�ñ���ϵ�����ɵ�1200
			Integral                     = 10;//10��;//40;//30;//17 60;//����ϵ��//��ʱ70���ٽ�ֵ
			Derivative                   = 300;//120��0;//50;//40/150;//΢��ϵ��
			StartSpeedTarget			 = 30000;
			StopSpeedTarget              = 4000;
			GotoAccRate=10;//15
			GotoStopRate=16;//15
			AwayAccRate=13;//15
			AwayStopRate=15;//15
		break;
		case 4:
			SpeedTarget 				 = 10000;
			PIDDoor                      = 1;
			Proportion                   = 250;//120��300;//310;//300;//150;//1000;//ֻ�ñ���ϵ�����ɵ�1200
			Integral                     = 10;//5��;//40;//30;//17 60;//����ϵ��//��ʱ70���ٽ�ֵ
			Derivative                   = 120;//20��0;//50;//40/150;//΢��ϵ��
			StartSpeedTarget			 = 10000;
			StopSpeedTarget              = 3000;
		
			GotoAccRate=10;//10
			GotoStopRate=16;//16
			AwayAccRate=13;//13
			AwayStopRate=15;//15
		
		break;
		case 5:
			SpeedTarget 				 = 7500;
			StartSpeedTarget			 = 7500;
			StopSpeedTarget              = 3000;	
			GotoAccRate=10;//15
			GotoStopRate=16;//15
			AwayAccRate=13;//15
			AwayStopRate=15;//15
		break;
	}
	
	if(Resolution_Class<=40)
	{
	
		if((MoveDirection==MoveGoToSplitter)&&((PositionResetFlag==1))&&(GatherDirection==1))
		{
//			SpeedTarget 				 = 40000;
//			PIDDoor                      = 5;//4
//	
//			Proportion                   = 350;//450
//			Integral                     = 10;
//			Derivative                   = 150;//250
//			StartSpeedTarget			 = 40000;
//			StopSpeedTarget              = 4000;
		
			SpeedTarget 				 = 30000;
			PIDDoor                      = 2;
			Proportion                   = 600;//250��300;//310;//300;//150;//1000;//ֻ�ñ���ϵ�����ɵ�1200
			Integral                     = 10;//10��;//40;//30;//17 60;//����ϵ��//��ʱ70���ٽ�ֵ
			Derivative                   = 300;//120��0;//50;//40/150;//΢��ϵ��
			StartSpeedTarget			 = 30000;
			StopSpeedTarget              = 4000;
			GotoAccRate=10;//10//15
			GotoStopRate=16;//16//15
			AwayAccRate=13;//13//15
			AwayStopRate=15;//15//15
		
		}
	  	
		if((MoveDirection==MoveAwaySplitter)&&((PositionResetFlag==1))&&(GatherDirection==2))
		{
//			SpeedTarget 				 = 40000;
//			PIDDoor                      = 5;//4
//	
//			Proportion                   = 350;//450
//			Integral                     = 10;
//			Derivative                   = 150;//250
//			StartSpeedTarget			 = 40000;
//			StopSpeedTarget              = 4000;
		
			SpeedTarget 				 = 30000;
			PIDDoor                      = 2;
			Proportion                   = 600;//250��300;//310;//300;//150;//1000;//ֻ�ñ���ϵ�����ɵ�1200
			Integral                     = 10;//10��;//40;//30;//17 60;//����ϵ��//��ʱ70���ٽ�ֵ
			Derivative                   = 300;//120��0;//50;//40/150;//΢��ϵ��
			StartSpeedTarget			 = 30000;
			StopSpeedTarget              = 4000;
			GotoAccRate=10;//15
			GotoStopRate=16;//15
			AwayAccRate=13;//15
			AwayStopRate=15;//15
		
		}
		
	}
}


void PhasePIDInit(void)
{
	
	if (MoveDirection==MoveGoToSplitter)
	{
		XProportion                   = 80;//40
		XIntegral                     = 60;//20
		XDerivative                   = 10;
	
		YProportion                   = 80;//40
		YIntegral                     = 60;//20
		YDerivative                   = 10;
	}
	else
	{
		XProportion                   = 80;//40
		XIntegral                     = 60;//20
		XDerivative                   = 10;
	
		YProportion                   = 80;//40
		YIntegral                     = 60;//20
		YDerivative                   = 10;
	}
	
//	if((MoveDirection==MoveGoToSplitter)&&(Resolution_Class<=40))
//	{
//		XProportion                   = 80;
//		XIntegral                     = 20;
//		XDerivative                   = 10;
//	
//		YProportion                   = 80;
//		YIntegral                     = 20;
//		YDerivative                   = 10;
//	}
//	
}
//PID�ڲ�������λ
void PIDReset(void)
{
	LastError                    = 0;  // Error[-1]
	PrevError                    = 0;  // Error[-2]
	SumError                     = 0;   // Sums of Errors	
	dError                       = 0; 
	Error                        = 0; 
	SubError                     = 0;
	Total                        = 0;
	
	LastXError                   = 0;  // Error[-1]
	PrevXError                   = 0;  // Error[-2]
	SumXError                    = 0;   // Sums of Errors	
	dXError                      = 0; 
	XError                       = 0; 
	SubXError                    = 0;
	XTotal                       = 0;
	
	LastYError                   = 0;  // Error[-1]
	PrevYError                   = 0;  // Error[-2]
	SumYError                    = 0;   // Sums of Errors	
	dYError                      = 0; 
	YError                       = 0; 
	SubYError                    = 0;
	YTotal                       = 0;
	
	StartStopCount               = 0;
	//SeizedCount           = 0;
	MechanicalZeroOffset         = 0;
	
	OutValueMax                  = 20000;
	PhaseCtrlMax                 = 2047;
	/////////////////////////****************usartctrl****************//////////////////////////
}
/***************************************/
/*PID�㷨���� 
���ܣ�����PID�ı������ ������� ΢�������ڽṹ����

�������:������,΢��,�������ṹ���ָ�룬
����ʽPID
*/
/***************************************/
void PIDCalc(void)		
{
	/*********************************************************************/
	SubError = Error - LastError;                         // Delta(e(k))=e(k)-e(k-1)������
	dError = Error - 2*LastError + PrevError;   // ΢����
	/*********************************************************************/	
	PrevError = LastError;	
	LastError = Error;
	/*********************************************************************/	
	SubError = Proportion * SubError;	                            // ������SubError
	
	Error = Integral * Error;			                                // ������
	dError = Derivative * dError; 		                            // ΢����
	Total = (SubError+ Error + dError)/200;
}

void PhaseXPIDCalc(void)		
{
	/*********************************************************************/
	SubXError = XError - LastXError;                         // Delta(e(k))=e(k)-e(k-1)������
	dXError = XError - 2*LastXError + PrevXError;   // ΢����
	/*********************************************************************/	
	PrevXError = LastXError;	
	LastXError = XError;
	/*********************************************************************/	
	SubXError = XProportion * SubXError;	                            // ������SubError
	
	XError = XIntegral * XError;			                                // ������
	dXError = XDerivative * dXError; 		                            // ΢����
	XTotal = (SubXError+ XError + dXError)/2000;
}

void PhaseYPIDCalc(void)		
{
	/*********************************************************************/
	SubYError = YError - LastYError;                         // Delta(e(k))=e(k)-e(k-1)������
	dYError = YError - 2*LastYError + PrevYError;   // ΢����
	/*********************************************************************/	
	PrevYError = LastYError;	
	LastYError = YError;
	/*********************************************************************/	
	SubYError = YProportion * SubYError;	                            // ������SubError
	
	YError = YIntegral * YError;			                                // ������
	dYError = YDerivative * dYError; 		                            // ΢����
	YTotal = (SubYError+ YError + dYError)/2000;
}


/****************************************************
��������:�������������ռ�ձ�
������� ��
������� ��
*****************************************************/
void OutputDrive(void)
{	

	if(OutValue>18000)OutValue=18000;
	if(OutValue<-18000)OutValue=-18000;
	SetOutput(OutValue);
}


/******************************************
�������� �������ٺ���
�������  ��
�������  ��
�����Ŵ�����м�̸��ţ���Դ����Ӱ�죬
*********************************************/
void preStartMove(void)
{
	ReadSpeed[0]=0;
	ReadSpeed[1]=0;
	//PIDInit();
	PIDInit();
	PhasePIDInit();
	PIDReset();
	
	if(MoveDirection==MoveGoToSplitter)
	{
		if(PositionResetFlag==0)OutValue=3000;//6300;//7000;//6500;//6000;
		OutputDrive();
		//SetOutput(50000);
	}else if(MoveDirection==MoveAwaySplitter)
	{
		if(PositionResetFlag==0)OutValue=(-5000);//(-7000);//(-5000);
		OutputDrive();
	}
	
}

void MovingMirrorSpeedUp(void)
{

	
	if(PositionResetFlag==0)
	{
		if(MoveDirection==MoveGoToSplitter)
		{
			if((OutValue>=8500)&&(PositionResetFlag==0))
			{
				ProcessFlag=preStartFlag;//�ʼ����ʱ���ж϶����Ƿ������ұߡ�����ǣ��͸ı��˶�����
				MoveDirection=MoveAwaySplitter;
				PositionResetFlag=1;//����Ϊ������ʼ�����˶�
			}
			if(OutValue<10000)
			{
				OutValue+=GotoAccRate;//30
				OutputDrive();
			}

		}else if(MoveDirection==MoveAwaySplitter)
		{
			if(OutValue>-10000)
			{
				OutValue-=AwayAccRate;//30
				OutputDrive();
			}
		}
	}
	if(PositionResetFlag==1)
	{
		if(MoveDirection==MoveGoToSplitter)
		{
			if(OutValue<10000)
			{
				OutValue+=GotoAccRate;		 //10
				OutputDrive();
			}

		}else if(MoveDirection==MoveAwaySplitter)
		{
			if(OutValue>-10000)
			{
				OutValue-=AwayAccRate;		 //13
				OutputDrive();
			}
		}
	}
	

	
}
/**********************************************
		�������� �������ٺ���
***********************************************/
void StopMove(void)
{	

}
/********************************************************* 
�������� PID�ٶȿ��ƺ���
*********************************************************/
int PIDSpeedCtrl(void)
{	
	int Temp;
	//*****************************************************

	PIDCalc();//Speed );	//PID
	
	//watch1 = Total;
	
		
	if (MoveDirection==MoveGoToSplitter) 
		Temp=OutValue + Total;
	else 
		Temp=OutValue - Total;   
	
	if(Temp<-OutValueMax) 
		Temp=-OutValueMax;
	else if(Temp>OutValueMax) 
		Temp=OutValueMax; //�趨Ŀǰ���ֵ
	
	OutValue=Temp;
	OutputDrive();
	return 1;
}
/********************************************************* 
�������� PID��λУ������

*********************************************************/
int PIDPhaseCtrl(void)
{	
	int XTemp,YTemp;
	//*****************************************************
	PhaseXPIDCalc();
	PhaseYPIDCalc();
	
	if (MoveDirection==MoveGoToSplitter) 
	{
		XTemp=XOutput + XTotal;
		YTemp=YOutput + YTotal;
	}
	else 
	{
		XTemp=XOutput - XTotal;
		YTemp=YOutput - YTotal;	
	}

	
	if(XTemp<-PhaseCtrlMax) 
		XTemp=-PhaseCtrlMax;
	else if(XTemp>PhaseCtrlMax) 
		XTemp=PhaseCtrlMax; //�趨Ŀǰ���ֵ
	/////////////////////////////////////////////////////////////
	if(YTemp<-PhaseCtrlMax) 
		YTemp=-PhaseCtrlMax;
	else if(YTemp>PhaseCtrlMax) 
		YTemp=PhaseCtrlMax; //�趨Ŀǰ���ֵ
	
	XOutput=XTemp;
	YOutput=YTemp;
	
	watch1=XOutput;
	

	//PhaseCtrlX(XOutput);
	//PhaseCtrlY(YOutput);
	return 1;
}

