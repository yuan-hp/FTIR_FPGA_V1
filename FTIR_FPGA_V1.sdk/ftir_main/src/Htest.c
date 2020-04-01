/*
 * Htest.c
 *
 *  Created on: 2019��9��4��
 *      Author: hpy
 */


#include <sleep.h>
#include <stdbool.h>
#include <stdio.h>
#include <xil_cache.h>
#include <xil_types.h>
#include <xparameters.h>
#include "xtime_l.h"

#include "Hak5394.h"
#include "platform.h"
#include "ax_pwm.h"

#include "system.h"
#include "Haxi_gpio.h"
#include "math.h"
#include "axi_debug_io.h"
#include "Haxi_pwm.h"

//�������ݵ�arduino�Ĵ��ڻ�ͼ�������л�ͼ
/**
 * ��arduino�Ĵ��ڼ����������øú����ɽ�����a�е����ݷ�Ϊ���黭����
 */
void send2draw(float a[],int len)
{
	for(int i =0;i<len;i++)
	{
		printf("%f",a[i]);
		if(i<len-1)printf(",");
	}
	printf("\r\n");
}
void send2drawOneLine(float a[],int len)
{
	for(int i =0;i<len;i++)
	{
		printf("%f",a[i]);
		printf("\r\n");
	}
}

//��ʼ��PLָʾ��
//void axi_pwm_init(u32 T,u32 GT)
//{
//	AX_PWM_mWriteReg(XPAR_AX_PWM_0_S00_AXI_BASEADDR,AX_PWM_S00_AXI_SLV_REG0_OFFSET,T);
//	AX_PWM_mWriteReg(XPAR_AX_PWM_0_S00_AXI_BASEADDR,AX_PWM_S00_AXI_SLV_REG1_OFFSET,GT);
//}
void mcu2fpga_write(int id,u32 data)
{


}

u32 mcu2fpga_read(int id)
{

	return 1;
}

/***************************************************************/
/*ak5394��ȡ
 *
 ***************************************************************/
void ak_5394_test()
{
	//init_platform();
	int data = 0;
	printf("----------------------\n");
	while(1){
		AK5394_LF398_mWriteReg(XPAR_AK5394_LF398_0_S00_AXI_BASEADDR,AK5394_LF398_S00_AXI_SLV_REG0_OFFSET,data);
		int d = AK5394_LF398_mReadReg(XPAR_AK5394_LF398_0_S00_AXI_BASEADDR,AK5394_LF398_S00_AXI_SLV_REG0_OFFSET);



		printf("%d^^^^^^^^^^%d\n",data,d);
		usleep(1000000);
		if(data >= 100)data=0;
		else data++;
	}
}
/***************************************************************/
/*ak5394����д��DDR����ȡ�Բ�
 *
 ***************************************************************/
void ak_5394_selfTest()
{
	u32 state ;
	XTime tStart,tFinish ;
	double tTime;
	//init_platform();
	Hak5394_init();
	Hak5394_WriteReg((u32)15,(u32) 0);//���ò�������ƫ����
	Hak5394_WriteReg((u32)9,(u32)8192);//����AD��������
	Hak5394_WriteReg((u32)10,(u32) 1);//���ô洢ʹ�õ�DDR block��ʽ ��˫block����ʹ��
	Hak5394_WriteReg((u32)12,(u32) 1);//����ADͨ��2����ΪAD���ݶ�ʧ������
	Hak5394_WriteReg((u32)11,(u32) 3);//����AK5394Ϊ�Բ�ģʽ
	Hak5394_WriteReg((u32)14,(u32) 6000);//����Ӳ���������ɼ��
	Hak5394_WriteReg((u32)13,(u32) 1);//����Ӳ���ɼ�����
	int d = 0;
	int cnt =0;
	while(0){
		d++;
		u32 miss_cnt = Hak5394_GetData(2);//��ȡӲ��ץȡ��ǰ���ݴ���DDR�Ĺ����ж�ʧ��������
		if(miss_cnt>0)printf("%d��miss cnt:%d\n",d, miss_cnt);
		if(cnt>=100000){
			cnt =0;
			printf("���Գ��������У���ʧ������Ϊ%d\n", miss_cnt);
		}
		else cnt++;
		usleep(10);
	}
	while(0)
	{
		printf("----------------------\n");
		state = Hak5394_GetStatus();
		int ddr_slect = (state>>30) & 0x00000001;
		//start = clock();
		XTime_GetTime(&tStart);
		if(ddr_slect == 0)
		{
			Xil_DCacheInvalidateRange(0x30000000,sizeof(ak5394_m1));//��DDR�����ݸ��µ�cache��,ˢ��8192��32λ��������Ҫ30us��ʱ��

			for(int i =0;i<8192;i++)
				printf("a1[%5d]=%5d  |  ", i,ak5394_m1[i]);
			printf("\n");
		}
		else
		{
			Xil_DCacheInvalidateRange(0x30008000,sizeof(ak5394_m2));//��DDR�����ݸ��µ�cache��
			for(int i =0;i<8192;i++)
				printf("a2[%5d]=%5d  |  ", i,ak5394_m2[i]);
			printf("\n");
		}
		//finish = clock();
		XTime_GetTime(&tFinish);
		tTime =(double)(tFinish - tStart)/COUNTS_PER_SECOND ;
		printf("ˢ�»����ʱ��Ϊ:%fs\n",tTime);
		usleep(1000000*20);
	}
}

/***************************************************************/
/*ak5394�ж϶�����
 *
 ***************************************************************/
XGpio Gpio;
void AxiGpioHandler(void *CallbackRef)
{


	printf("�жϺ���\n");
	XGpio *GpioPtr = (XGpio *)CallbackRef;
	u32 rt = XGpio_InterruptGetStatus(GpioPtr);
	if(rt & 0x00000001 == 1u)
	{
		printf("AD inte\r\n");
	}
	else if((rt>>1) & 0x00000001 == 1u)
	{
		printf("La inte\r\n");
	}

	u32 state = Hak5394_GetStatus();
	u32 ddr_slect = (state>>30) & 0x00000001;
	u32 dnum = Hak5394_GetData(1);
	u32 dnum1 = Hak5394_GetData(2);
	printf("����������:%u\n",dnum);
	printf("AD������:%u\n",dnum1);
	static u32 mcnt = 0;
	printf("int cnt:%d\n",++mcnt);
	int len = 100;
	if(mcnt == 1)//�鿴��ַ��ȷ���
	{
		printf("ADDR1:%x\n",AK_5394_DATA1_BASE_ADDR);
		printf("ADDR2:%x\n",AK_5394_DATA2_BASE_ADDR);
		printf("ADDR3:%x\n",AK_5394_TIME1_BASE_ADDR);
		printf("ADDR4:%x\n",AK_5394_TIME2_BASE_ADDR);
		printf("ADDR5:%x\n",AK_5394_LASER_PULSE_TIME1_BASE_ADDR);
		printf("ADDR6:%x\n",AK_5394_LASER_PULSE_TIME2_BASE_ADDR);
	}
	if(ddr_slect == 0)
	{
		printf("-------------0------------\n");
		Xil_DCacheInvalidateRange(AK_5394_DATA1_BASE_ADDR,4*len);//ˢ�¶�Ӧ���ݵĻ�����������1��Ϊ�������ݣ�2��Ϊһ���������
		Xil_DCacheInvalidateRange(AK_5394_DATA2_BASE_ADDR,4*len);
		Xil_DCacheInvalidateRange(AK_5394_TIME1_BASE_ADDR,4*len);
		Xil_DCacheInvalidateRange(AK_5394_TIME2_BASE_ADDR,4*len);
		Xil_DCacheInvalidateRange(AK_5394_LASER_PULSE_TIME1_BASE_ADDR,4*len);
		Xil_DCacheInvalidateRange(AK_5394_LASER_PULSE_TIME2_BASE_ADDR,4*len);

		for(int i = 0;i<len;++i)
		{
			//printf("a[%5d]=%5u  |  ", i,ak5394_m2[i]);//ak5394_laser_t1
			//printf("%5u",ak5394_t2[i] - ak5394_t2[i-1] );
			//printf("%5u\n",ak5394_laser_t2[i] - ak5394_laser_t2[i-1]);
			//printf("a[%5d]=%5u  |  ", i,ak5394_laser_t2[i]);//ak5394_laser_t1
			//printf("%5u   ",ak5394_laser_t1[i] - ak5394_laser_t1[i-1] );
			//printf("%5u  ",ak5394_laser_t2[i]);
			//printf("\r\n");
		}


		printf("\n");

	}
	else
	{
		printf("------------1-------------\n");
		Xil_DCacheInvalidateRange(AK_5394_DATA1_BASE_ADDR,4*len);
		Xil_DCacheInvalidateRange(AK_5394_DATA2_BASE_ADDR,4*len);
		Xil_DCacheInvalidateRange(AK_5394_TIME1_BASE_ADDR,4*len);
		Xil_DCacheInvalidateRange(AK_5394_TIME2_BASE_ADDR,4*len);
		Xil_DCacheInvalidateRange(AK_5394_LASER_PULSE_TIME1_BASE_ADDR,4*len);
		Xil_DCacheInvalidateRange(AK_5394_LASER_PULSE_TIME2_BASE_ADDR,4*len);

		for(int i = 0;i<len;++i)
		{
			//printf("a[%5d]=%5u  |  ", i,ak5394_m2[i]);//ak5394_laser_t1
			//printf("%5u",ak5394_t2[i] - ak5394_t2[i-1] );
			//printf("%5u\n",ak5394_laser_t2[i] - ak5394_laser_t2[i-1]);
			//printf("a[%5d]=%5u  |  ", i,ak5394_laser_t2[i]);//ak5394_laser_t1
			printf("%5u   ",ak5394_laser_t2[i] - ak5394_laser_t2[i-1] );
			//printf("%5u  ",ak5394_laser_t2[i]);
			//printf("\r\n");
		}
		printf("\n");
	}
	//usleep(1);
	//IntrFlag = 1;
	/* Clear the Interrupt */
	XGpio_InterruptClear(GpioPtr, 0x00000003);
}
/**
 * AK5394�ж϶����ݲ���
 */
int ak5394_int_test()
{
	ak_5394_selfTest();
	//XGpio Gpio;
	int Status;
	printf("��ʼ����AXI GPIO\n");
	//��ʼ��AXI GPIO
	Status = XGpio_Initialize(&Gpio, XPAR_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	printf("��ʼ����AXI GPIO:1\n");

	XScuGic_Config *IntcConfig;

		/*
		 * Initialize the interrupt controller driver so that it is ready to
		 * use.
		 */
	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(&LinIntcInstance, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	XScuGic_SetPriorityTriggerType(&LinIntcInstance, XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR,0xA0, 0x1);
	/*
	 * Connect the interrupt handler that will be called when an
	 * interrupt occurs for the device.
	 */
	Status = XScuGic_Connect(&LinIntcInstance, XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR,
				 (Xil_ExceptionHandler)AxiGpioHandler, &Gpio);
	if (Status != XST_SUCCESS) {
		return Status;
	}
	printf("��ʼ����AXI GPIO:2\n");
	/* Enable the interrupt for the GPIO device.*/
	XScuGic_Enable(&LinIntcInstance, XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR);
	printf("��ʼ����AXI GPIO:3\n");
	/*
	 * Enable the GPIO channel interrupts so that push button can be
	 * detected and enable interrupts for the GPIO device
	 */
	XGpio_InterruptEnable(&Gpio, 0x00000001);
	printf("��ʼ����AXI GPIO:4\n");
	XGpio_InterruptGlobalEnable(&Gpio);
	printf("��ʼ����AXI GPIO:5\n");
	/*
	* Initialize the exception table and register the interrupt
	* controller handler with the exception table
	*/
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler, &LinIntcInstance);
	/* Enable non-critical exceptions */
	Xil_ExceptionEnable();
	printf("����AXI GPIO ����\n");

	int d = 0;
	int cnt =0;
	while(1){
		d++;
		u32 miss_cnt = Hak5394_GetData(2);//��ȡӲ��ץȡ��ǰ���ݴ���DDR�Ĺ����ж�ʧ��������
		if(miss_cnt>0)printf("%d��miss cnt:%d\n",d, miss_cnt);
		if(cnt>=100000){
			cnt =0;
			printf("���Գ��������У���ʧ������Ϊ%d\n", miss_cnt);
		}
		else cnt++;
		usleep(10);
	}
}
/**
 * ��ģ�����ak5394��FPGA����������˴洢ʱ�������
 */
int ak5394_int_v1()
{
	printf("��ʼak5394ģ���Բ�");
	u32 Status ;
	XTime tStart,tFinish ;
	double tTime;
	//init_platform();
	u32 ad_count = 204800;//ad���ݵ�����
	Hak5394_init();
	Hak5394_WriteReg((u32) 9, AK_5394_DATA_BUF_LEN); //����AD��������  204800���ò���������Ӳ������һƬDDR�н�������д��
	Hak5394_WriteReg((u32) 12, (u32) 2); //����ADͨ��2����ΪAD����Ӳ��ʵ�ʲɼ������ڶ������ж�ʱͨ��2ͨ����ȡ
	Hak5394_WriteReg((u32) 11, (u32) 1); //����AK5394ģʽ  0�������ɼ� 1����ʱ�ɼ� 2�������ɼ� 3���Բ�ģʽ
	Hak5394_WriteReg((u32) 5, 15000u); //����AK5394��ʱ�ɼ���ʱ��   150MHz����15000�Σ�10us�ɼ�һ�Σ�
	Hak5394_Reuse_ch1(1); //����ͨ��1Ϊ��������ʱ��������
	Hak5394_SetTimerFeq(15U); //���ü�ʱ������ʱ��Ƶ�� 150MHz����15�Σ�10MHz��ʱ�����ݵĻ���ʱ�ӣ�
	//Hak5394_SetOption(AK_ENABLE_PL_FILTER,0);//����PL �˲�
	//Hak5394_WriteReg((u32)14,(u32) 6000);//����Ӳ���������ɼ��
	Hak5394_WriteReg((u32) 13, (u32) 1);	//����Ӳ���ɼ�����


	printf("��ʼ����AXI GPIO\n");
	//��ʼ��AXI GPIO
	Status = XGpio_Initialize(&Gpio, XPAR_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	printf("��ʼ����AXI GPIO:1\n");

	XScuGic_Config *IntcConfig;

		/*
		 * Initialize the interrupt controller driver so that it is ready to
		 * use.
		 */
	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(&LinIntcInstance, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	XScuGic_SetPriorityTriggerType(&LinIntcInstance, XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR,0xA0, 0x1);
	/*
	 * Connect the interrupt handler that will be called when an
	 * interrupt occurs for the device.
	 */
	Status = XScuGic_Connect(&LinIntcInstance, XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR,
				 (Xil_ExceptionHandler)AxiGpioHandler, &Gpio);
	if (Status != XST_SUCCESS) {
		return Status;
	}
	printf("��ʼ����AXI GPIO:2\n");
	/* Enable the interrupt for the GPIO device.*/
	XScuGic_Enable(&LinIntcInstance, XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR);
	printf("��ʼ����AXI GPIO:3\n");
	/*
	 * Enable the GPIO channel interrupts so that push button can be
	 * detected and enable interrupts for the GPIO device
	 */
	XGpio_InterruptEnable(&Gpio, 0x00000003);
	printf("��ʼ����AXI GPIO:4\n");
	XGpio_InterruptGlobalEnable(&Gpio);
	printf("��ʼ����AXI GPIO:5\n");
	/*
	* Initialize the exception table and register the interrupt
	* controller handler with the exception table
	*/
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler, &LinIntcInstance);
	/* Enable non-critical exceptions */
	Xil_ExceptionEnable();
	printf("����AXI GPIO ����\n");
	int d = 0;
	int cnt =0;
	while(1)usleep(100);
	while(0){
		d++;
		u32 miss_cnt = Hak5394_GetData(2);//��ȡӲ��ץȡ��ǰ���ݴ���DDR�Ĺ����ж�ʧ��������
		if(miss_cnt>0)printf("%d��miss cnt:%d\n",d, miss_cnt);
		if(cnt>=100000){
			cnt =0;
			printf("���Գ��������У���ʧ������Ϊ%d\n", miss_cnt);
		}
		else cnt++;
		usleep(10);
	}
}
/************************************
 * ��������ģ�����
 ************************************/
void lmd18200_test_v1()
{
	u32 MatchValue = 30000 ;
	Haxi_gpio_init();//��ʼ��AXI GPIO
	Httc_init_ttc0_timer0(&MatchValue);//TTC0���ڿ��ƶ������˶�
	u32 data = 1;
	printf("lmd18200\r\n");
	while(1)
	{
		Haxi_gpio_write(1);
		//data = data==1?0:1;
		usleep(50000);
	}

}
/**
 * PS��DDR�Ķ�д���Լ���֤�豸�����ڴ�˴洢����С�˴洢
 */
void DDR_PS_TEST()
{
	Xil_Out32(0x30000000,0x12345678);
	u32 data1 = Xil_In8(0x30000000);
	u32 data2 = Xil_In8(0x30000000 + 1);
	u32 data3 = Xil_In8(0x30000000 + 2);
	u32 data4 = Xil_In8(0x30000000 + 3);
	printf("��ַ:%x      8λ����:%x\n",0x30000000,data1);
	printf("��ַ:%x      8λ����:%x\n",0x30000001,data2);
	printf("��ַ:%x      8λ����:%x\n",0x30000002,data3);
	printf("��ַ:%x      8λ����:%x\n",0x30000003,data4);
	while(1)usleep(1000000);
}
/************************************************
 * ����ak5394����ģʽ������ʹ��
 ***********************************************/
void ddr_get_data_test()
{
	Hmfreq_init();//��ʼ����Ƶģ��
	Hgpio_axi_int_init();//�������ݲɼ�����ж��ź�
	Hak5394_PL_init();//����ak5394����ģʽ
	while(1);
	while(0)
	{
		Hmfreq_PsPushMoveDir2PL(HMFREQ_HIGH);//֪ͨPL���ֶ������ƶ�����1   ���ֵ���ᴫ�뵽PL��������⵽�Ķ�������
		usleep(1000000);
		Hmfreq_PsPushMoveDir2PL(HMFREQ_LOW);//֪ͨPL���ֶ������ƶ�����Ϊ0
		usleep(1000000);
		xil_printf("runing\r\n");
	}
}
void draw_test()
{
	float a[4]={0,0};
	float d1 = 0;
	while(1)
	{
		send2draw(a,4);
		a[0]=10*sin(d1);
		a[1]=5*cos(d1);
		a[2]=10*sin(d1)-4;
		a[3]=5*cos(d1)+3;
		d1+=0.02;
		usleep(10000);
	}
}
/**********************************
 * debug io test
 **********************************/
void debug_io_test()
{
	DAC7631_mWriteReg(XPAR_AXI_DEBUG_IO_0_S00_AXI_BASEADDR , AXI_DEBUG_IO_S00_AXI_SLV_REG0_OFFSET,0u);
	DAC7631_mWriteReg(XPAR_AXI_DEBUG_IO_0_S00_AXI_BASEADDR , AXI_DEBUG_IO_S00_AXI_SLV_REG3_OFFSET,3u);
	u32 data = DAC7631_mReadReg(XPAR_AXI_DEBUG_IO_0_S00_AXI_BASEADDR , AXI_DEBUG_IO_S00_AXI_SLV_REG3_OFFSET);
	printf("debug_io:%u\n",data);
	while(0)
	{
		usleep(1000);
	}
}
/*******************************************
 * DACA7631����
 *************************************/
void dac7631_test()
{
	usleep(1000000);
	Hdac7631_init();
	u16 da1 = 0;
	u16 da2 = 0;
	while(1)
	{
		Hdac7631_SetDa(da1,  da2);
		da1 += 100;
		da2 += 100;
		usleep(1000000);
	}
	//Hdac7631_SetCoil(HDAC_5394_COIL_A , 37768);

}
/*******************************************************
 * ����/Ƶ�ʲ���ģ��
 *******************************************************/
void test_measureFreq()
{
	Hmfreq_init();//��ʼ����Ƶģ��
	u32 t = 0;
	u32 ctl,st;
	u32 sig_in;
	int flag=0;
	while(1)
	{
		////////////////////////////////////////////////////////////////////////////////////////////
		if(flag)Hmfreq_PsPushMoveDir2PL(HMFREQ_HIGH);//֪ͨPL���ֶ������ƶ�����1   ���ֵ���ᴫ�뵽PL��������⵽�Ķ�������
		else Hmfreq_PsPushMoveDir2PL(HMFREQ_LOW);//֪ͨPL���ֶ������ƶ�����Ϊ0
		flag = flag?0:1;
		//Hmfreq_PsPushMoveDir2PL(HMFREQ_LOW);//֪ͨPL���ֶ������ƶ�����Ϊ0
		//////////////////////////////////////////////////////////
		t = 148148163/Hmfreq_GetCycleT();//��ȡ����ļ���ֵ
		ctl = Hmfreq_GetCtlFromPL();
		st = Hmfreq_GetPLState();
		ctl = (ctl>>2) &0x00000001;
		sig_in = st & 0x00000001;
		st = (st>>2) &0x00000001;
		printf("��ȡ�����ڼ���ֵΪ:%u   ���������ƽΪ:%u\n",t,sig_in);
		usleep(1000000);
	}
}
//tas5111ģ����ԣ�ע���ģ��һ��Ҫ�Ϳ���lmd18200��ģ��һ��ʹ�û�
void tas5111_t()
{
	Haxi_pwm_init();
	Htas5111_init();
	//Haxi_pwm_StartC0();
	AX_PWM_mWriteReg(XPAR_AX_PWM_0_S00_AXI_BASEADDR,AX_PWM_S00_AXI_SLV_REG0_OFFSET,50000000u);
	AX_PWM_mWriteReg(XPAR_AX_PWM_0_S00_AXI_BASEADDR,AX_PWM_S00_AXI_SLV_REG1_OFFSET,25000000U);
	float duty = 0.6;
	u32 din = 0;
	u32 dir = 0;
	Haxi_pwm_SetDir0(0);
	while(1)
	{
		din = (u32)(duty*20000);
		Haxi_pwm_SetHTime0(din);
		usleep(1000000*3);
		Haxi_pwm_SetDir0(dir);
		if(dir==0)dir=1;
		else dir = 0;
		printf("hdata=%u\r\n",din);
	}
}
//void LaserIntrHandler1(void *CallbackRef)
//{
//	u32 dst = 0xf8f01284;
//	u32 it_flag = Xil_In32(dst);
//	it_flag |= 0x40000000;
//	//it_flag = 0xffffffff;
//	Xil_Out32(dst,it_flag);
//	printf("����La it\r\n");
//	//XScuGic_SoftwareIntr(&LinIntcInstance, LaserSoftIntrIdToCpu1, CPU1) ;
//
//}
//void LaserIntrInit1()
//{
//	axi_pwm_init(150000000u,150000000-50);
//	//ak5394_int_v1();
//
//	XScuGic_Connect(&LinIntcInstance, 62,(Xil_InterruptHandler)LaserIntrHandler1, NULL);
//	XScuGic_Enable(&LinIntcInstance,62);
//	//XScuGic_SetPriorityTriggerType(&LinIntcInstance,62,0, 0b11);//�������������ش���
//	while(1)
//	{
//		usleep(1000);
//	}
//}

void test_irq_PL()
{
	axi_pwm_init(150000000,150000000/2);
	Hcfg_sig_init();
	axi_pwm_init((u32)150000000*2,150000000*2/2);
	Hcfg_EnableLaserInt();
	Hcfg_EnableSwitchInt();
	while(1)
	{
		//usleep(1000000*2);
		sleep(1);
		//xil_printf("------------running--------------\r\n");
		//dis_info();
		//printf("%d\r\n",TestCounter);
		//TestCounter = 0;

	}
}
///////////////////////�����õ�������///////////////////////////////////////////////
int test_main()
{
//	axi_pwm_init((u32)150000000,1000000);//��ʼ������ָʾ��
	//LaserIntrInit();
	//pl_write_ddr_ps_read_ddr_test();
	//ak_5394_test();
	//pl_write_ddr_lite/ps_read_ddr_test();
	//write_ddr_lite_test();
	//ak_5394_selfTest();
	//ak5394_int_test();
	//ak5394_int_v1();
	//lmd18200_test_v1();
	//DDR_PS_TEST();

	//ddr_get_data_test();
	//debug_io_test();
	//test_measureFreq();//Ƶ�ʲ���ģ�����
	//LaserIntrInit1();
	//test_irq_PL();
	//ddr_get_data_test();
	tas5111_t();
	/*
	int len = 4000;
	//int data[1000];
	float dataf[len];
	for(int i = 0;i<len;++i)
	{
		int td = Hak5394_GetData(2);
		dataf[i] = td*1.0;
		usleep(100);
	}
	send2drawOneLine(dataf,len);
	*/
	//dac7631_test();
    return 0;
}
