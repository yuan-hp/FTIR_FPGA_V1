#include "sys.h"
#include "uart.h"
#include "gpio.h"
#include "pit.h"
#include "adc.h"
#include "delay.h"
#include "code.h"
#include "kbi.h"
#include "kbi_chk60evb.h"
#include "lcd.h"
#include "flash.h"
#include "flashwrite.h"

#ifndef USE_UART1
#define USE_UART1  //Èô¶¨Òå¸Ã±äÁ¿¡£Êý¾Ý´«Êä½«Ê¹ÓÃUART1£¬·ñÔòÊ¹ÓÃUART2´«Êä
#endif

//³¬ºË ·ÉË¼¿¨¶û Kinetis¹Ì¼þ¿âÀý³Ì
//¹Ì¼þ¿â°æ±¾ V2.4
//startup:        Æô¶¯ÎÄ¼þ
//devices:        Ó²¼þ°å×Ó×ÊÔ´Ö§³Ö¿â(²¿·ÖÊµÑéÓÃµ½)
//utilities:      Èí¼þÄ£¿éÖ§³Ö¿â(²¿·ÖÊµÑéÓÃµ½)
//drivers:        ¹Ì¼þ¿âÔ´Âë
//use:            ÓÃ»§´úÂë °üº¬mianº¯ÊýºÍ ÖÐ¶Ï·þÎñ³ÌÐò 

/*
* ÈëÃÅÖªÊ¶:
* 
*/

void gpiooutinit()//³õÊ¼»¯gpioÊä³ö¿Ú
{   GPIO_InitTypeDef GPIO_InitStruct1;
    GPIO_InitStruct1.GPIOx = PTC;                       //PTC(PORTC) ¶Ë¿Ú
	  GPIO_InitStruct1.GPIO_InitState = Bit_RESET;        //Èç¹ûÉèÖÃÎªÊä³ö ÔòÊä³öµÍµçÆ½ Èç¹ûÉèÖÃÎªÊäÈë ´ËÏîÎÞÐ§
	  GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;    //½ûÖ¹´ËÒý½ÅµÄÖÐ¶ÏÇëÇó
	  GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_0;             //µç»ú·½ÏòÖ¸Áî
	  GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_OPP;         //ÉèÖÃÎªÍÆÍìÊä³öÄ£Ê½
	  GPIO_Init(&GPIO_InitStruct1);
	  GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_1;
	  GPIO_Init(&GPIO_InitStruct1);
	  GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_2;
	  GPIO_Init(&GPIO_InitStruct1);
	  GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_3;//µç»úÍÑ»úÖ¸Áî
	  GPIO_Init(&GPIO_InitStruct1);
	  GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_4;
	  GPIO_Init(&GPIO_InitStruct1);
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_5;
	  GPIO_Init(&GPIO_InitStruct1);
	  GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_6;//uÐ´ÐÅºÅÎ»
	  GPIO_Init(&GPIO_InitStruct1);
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_7;//vÐ´ÐÅºÅÎ»
	  GPIO_Init(&GPIO_InitStruct1);
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_8;//DirµØÖ·Î»
	  GPIO_Init(&GPIO_InitStruct1);	
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_9;//offineµØÖ·Î»
	  GPIO_Init(&GPIO_InitStruct1);
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_10;//Ô¤ÁôµØÖ·Î»
	  GPIO_Init(&GPIO_InitStruct1);
	  GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_11;//xÐ´ÐÅºÅÎ»
	  GPIO_Init(&GPIO_InitStruct1);
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_12;//yÐ´ÐÅºÅÎ»
	  GPIO_Init(&GPIO_InitStruct1);
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_13;//zÐ´ÐÅºÅÎ»
	  GPIO_Init(&GPIO_InitStruct1);	
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_14;//xÖáÂö³å
	  GPIO_Init(&GPIO_InitStruct1);
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_15;//yÖáÂö³å
	  GPIO_Init(&GPIO_InitStruct1);
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_16;//zÖáÂö³å
	  GPIO_Init(&GPIO_InitStruct1);
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_17;//uÖáÂö³å
	  GPIO_Init(&GPIO_InitStruct1);
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_18;//vÖáÂö³å
	  GPIO_Init(&GPIO_InitStruct1);
    GPIO_InitStruct1.GPIOx = PTD;						//CPLDµÄ¸´Î»Î»£¬Ê¼ÖÕÎª¸ßµçÆ½
	  GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_7;
	  GPIO_Init(&GPIO_InitStruct1);
		GPIO_SetBits(PTD,7);
		GPIO_InitStruct1.GPIOx = PTB;   				//PTB1¾Û½¹Ê±Îª¸ßµçÆ½
	  GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_1;
	  GPIO_Init(&GPIO_InitStruct1);
		/*
		GPIO_InitStruct1.GPIOx = PTB;   			//PTB3¾Û½¹Êä³ö
	  GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_3;
	  GPIO_Init(&GPIO_InitStruct1);
		*/
		
 }

void gpioininit()  //³õÊ¼»¯IO¿ÚÊäÈëÏÞÎ»¿ª¹ØÊäÈëÎ»
{   
		GPIO_InitTypeDef GPIO_InitStruct1;
  	GPIO_InitStruct1.GPIOx = PTA;                       	//PTA(PORTA) ¶Ë¿Ú
	  GPIO_InitStruct1.GPIO_InitState = Bit_RESET;        	//Èç¹ûÉèÖÃÎªÊä³ö ÔòÊä³öµÍµçÆ½ Èç¹ûÉèÖÃÎªÊäÈë ´ËÏîÎÞÐ§
	  GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;    	//½ûÖ¹´ËÒý½ÅµÄÖÐ¶ÏÇëÇó
	  GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_13;             	//PTA13Òý½Å   xÖáÏÞÎ»¿ª¹ØÊäÈëÎ»
	  GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_IPD;         	//ÉèÖÃÎªÏÂÀ­ÊäÈë
	  GPIO_Init(&GPIO_InitStruct1);
	  GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_14;             	//PTA14Òý½Å   yÖáÏÞÎ»¿ª¹ØÊäÈëÎ»
	  GPIO_Init(&GPIO_InitStruct1);
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_15;             	//PTA15Òý½Å   zÖáÏÞÎ»¿ª¹ØÊäÈëÎ»
	  GPIO_Init(&GPIO_InitStruct1);
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_16;             	//PTA16Òý½Å   uÖáÏÞÎ»¿ª¹ØÊäÈëÎ»
	  GPIO_Init(&GPIO_InitStruct1);
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_17;             	//PTA17Òý½Å   vÖáÏÞÎ»¿ª¹ØÊäÈëÎ»
	  GPIO_Init(&GPIO_InitStruct1);
}
void gpioint()
{ GPIO_InitTypeDef GPIO_InitStruct2;
	  GPIO_InitStruct2.GPIOx = PTB;                       	//PTB(PORTB) ¶Ë¿Ú
	  GPIO_InitStruct2.GPIO_InitState = Bit_RESET;        	//Èç¹ûÉèÖÃÎªÊä³ö ÔòÊä³öµÍµçÆ½ Èç¹ûÉèÖÃÎªÊäÈë ´ËÏîÎÞÐ§
	  GPIO_InitStruct2.GPIO_IRQMode = GPIO_IT_FALLING;    	//´ËÒý½ÅÏÂ½µÑØÖÐ¶Ï
	  GPIO_InitStruct2.GPIO_Pin = GPIO_Pin_2;             	//PTB2Òý½Å
	  GPIO_InitStruct2.GPIO_Mode = GPIO_Mode_IPU;         	//ÉèÖÃÎªÏÂÀ­ÊäÈë
	  GPIO_Init(&GPIO_InitStruct2);
    NVIC_EnableIRQ(PORTB_IRQn);
}


void ADC_init()//³õÊ¼»¯ADC
{   
	  ADC_InitTypeDef ADC_InitStruct1;
    ADC_InitStruct1.ADCxMap =  ADC0_SE9_PB1;              //PB1 Òý½Å×÷ÎªADC0 9Í¨µÀ
    ADC_InitStruct1.ADC_Precision = ADC_PRECISION_10BIT;  //10Î»¾«¶È
    ADC_InitStruct1.ADC_TriggerSelect = ADC_TRIGGER_SW;   //Èí¼þ´¥·¢(A Í¨µÀ¿ÉÊ¹ÓÃÈí/Ó²¼þ´¥·¢ B Í¨µÀÖ»ÄÜÊ¹ÓÃÓ²¼þ´¥·¢)
    ADC_Init(&ADC_InitStruct1);
		ADC_InitStruct1.ADCxMap =  ADC0_SE12_PB2;              //PB2 Òý½Å×÷ÎªADC0 12Í¨µÀ
		ADC_Init(&ADC_InitStruct1);
		ADC_InitStruct1.ADCxMap =  ADC0_SE13_PB3;              //PB3 Òý½Å×÷ÎªADC0 13Í¨µÀ
		ADC_Init(&ADC_InitStruct1);
}

uint8_t  flagpit=0;//xÖá£¬yÖá´¥·¢PIT0±êÖ¾Î»
uint8_t  flaggo=0;//»Øµ½×î¼ÑÎ»ÖÃ
uint8_t  imagenum=0;//×î¼ÑÍ¼ÏñºÅ
uint8_t j=0;//Êý×éÖÐµÄ±êÖ¾Î»
uint32_t position[255];//´æ·Å²É¼¯Í¼ÏñÊ±×ø±êÎ»ÖÃ
uint8_t flagfc=0;//¾Û½¹±êÖ¾Î»
uint8_t flagfc1=0;//¾Û½¹±êÖ¾Î»1
uint8_t flagfc2=0;//¾Û½¹±êÖ¾Î»2
uint8_t length=0;//Ð´FLASHµÄ×Ö½Ú¸öÊý
uint8_t addr_start=0;//Ð´FLASHÆðÊ¼µØÖ·
uint8_t flash_buffer[128];
uint8_t AutoMode=1;
uint8_t LoadPara=0;
uint8_t SaveSteplen=0;
uint8_t SaveArea =0;
uint32_t UMax_Pos=0;
uint32_t UMin_Pos=0;
uint32_t VMax_Pos=0;
uint32_t VMin_Pos=0;
uint32_t XMax_Pos=0;
uint32_t XMin_Pos=0;
uint32_t YMax_Pos=0;
uint32_t YMin_Pos=0;
uint8_t flago=0;   //xÖá£¬yÖá»Øµ½Ô­µã±êÖ¾Î»£»
uint8_t flago1=0;  //uÖá£¬vÖá»Øµ½Ô­µã±êÖ¾Î»£»
uint8_t flagc=0;   
uint8_t flagsu=0;
uint8_t flagsv=0;
uint8_t flagsw=0;
uint8_t flagsx=0;
uint8_t flagsy=0;
uint8_t flagsz=0;
uint32_t CP_Valueu=0;   //uµç»úµ±Ç°ËÙ¶ÈÖµ
uint32_t CP_Valuev=0;   //vµç»úµ±Ç°ËÙ¶ÈÖµ
uint32_t CP_Valuew=0;   //wµç»úµ±Ç°ËÙ¶ÈÖµ
uint32_t CP_Valuex=0;   //xµç»úµ±Ç°ËÙ¶ÈÖµ
uint32_t CP_Valuey=0;   //yµç»úµ±Ç°ËÙ¶ÈÖµ
uint32_t CP_Valuez=0;   //zµç»úµ±Ç°ËÙ¶ÈÖµ
uint32_t P_Valueu=0;   //uµç»úËÙ¶ÈÖµ
uint32_t P_Valuev=0;   //vµç»úËÙ¶ÈÖµ
uint32_t P_Valuew=0;   //wµç»úËÙ¶ÈÖµ
uint32_t P_Valuex=0;   //xµç»úËÙ¶ÈÖµ
uint32_t P_Valuey=0;   //yµç»úËÙ¶ÈÖµ
uint32_t P_Valuez=0;   //zµç»úËÙ¶ÈÖµ
uint8_t buf2[4];
uint8_t command=0;//ÉÏÎ»»úÖ¸ÁîÊÇ·ñÓÐÐ§
uint32_t stepu=0; //uµç»ú²½ÊýÖµ
uint32_t stepv=0; //vµç»ú²½ÊýÖµ
uint32_t stepw=0; //wµç»ú²½ÊýÖµ
uint32_t stepx=0; //xµç»ú²½ÊýÖµ
uint32_t stepy=0; //yµç»ú²½ÊýÖµ
uint32_t stepz=0; //zµç»ú²½ÊýÖµ
uint32_t pulscntu=0; //uµç»úµ±Ç°×ø±ê
uint32_t pulscntv=0; //vµç»úµ±Ç°×ø±ê
uint32_t pulscntw=0; //wµç»úµ±Ç°×ø±ê
uint32_t pulscntx=0; //xµç»úµ±Ç°×ø±ê
uint32_t pulscnty=0; //yµç»úµ±Ç°×ø±ê
uint32_t pulscntz=0; //zµç»úµ±Ç°×ø±ê
uint32_t D_Valueu=0; //uµç»ú·½Ïò
uint32_t D_Valuev=0; //vµç»ú·½Ïò
uint32_t D_Valuew=0; //wµç»ú·½Ïò
uint32_t D_Valuex=0; //xµç»ú·½Ïò
uint32_t D_Valuey=0; //yµç»ú·½Ïò
uint32_t D_Valuez=0; //zµç»ú·½Ïò
uint32_t limitu=0; //uÖáÏÞÎ»¿ª¹Ø×´Ì¬
uint32_t limitv=0; //vÖáÏÞÎ»¿ª¹Ø×´Ì¬
uint32_t limitw=0; //wÖáÏÞÎ»¿ª¹Ø×´Ì¬
uint32_t limitx=0; //xÖáÏÞÎ»¿ª¹Ø×´Ì¬  ²éÑ¯·½Ê½È·¶¨
uint32_t limity=0; //yÖáÏÞÎ»¿ª¹Ø×´Ì¬	²éÑ¯·½Ê½È·¶¨
uint32_t limitz=0; //zÖáÏÞÎ»¿ª¹Ø×´Ì¬
uint32_t switchover=1;
uint32_t completeu=0; //uµç»ú¶¯×÷Íê³É±êÖ¾
uint32_t completev=0; //vµç»ú¶¯×÷Íê³É±êÖ¾
uint32_t completew=0; //wµç»ú¶¯×÷Íê³É±êÖ¾
uint32_t completex=0; //xµç»ú¶¯×÷Íê³É±êÖ¾
uint32_t completey=0; //yµç»ú¶¯×÷Íê³É±êÖ¾
uint32_t completez=1; //zµç»ú¶¯×÷Íê³É±êÖ¾
uint8_t flagru=0; //uÖá»Øµ½Ô­µã±êÖ¾Î»
uint8_t flagrv=0; //vÖá»Øµ½Ô­µã±êÖ¾Î»
uint8_t flagrw=0; //wÖá»Øµ½Ô­µã±êÖ¾Î»
uint8_t flagrx=0; //xÖá»Øµ½Ô­µã±êÖ¾Î»
uint8_t flagry=0; //yÖá»Øµ½Ô­µã±êÖ¾Î»
uint32_t Gradientu=0;  //uµç»úµÄÐ±ÂÊ
uint32_t Gradientv=0;  //vµç»úµÄÐ±ÂÊ
uint32_t Gradientw=0;  //wµç»úµÄÐ±ÂÊ
uint32_t Gradientx=0;  //xµç»úµÄÐ±ÂÊ
uint32_t Gradienty=0;  //yµç»úµÄÐ±ÂÊ
uint32_t Gradientz=0;  //zµç»úµÄÐ±ÂÊ
uint32_t flagupu=0; //uµç»ú¼ÓËÙ±êÖ¾Î»
uint32_t flagupv=0; //vµç»ú¼ÓËÙ±êÖ¾Î»
uint32_t flagupw=0; //wµç»ú¼ÓËÙ±êÖ¾Î»
uint32_t flagupx=0; //xµç»ú¼ÓËÙ±êÖ¾Î»
uint32_t flagupy=0; //yµç»ú¼ÓËÙ±êÖ¾Î»
uint32_t flagupz=0; //zµç»ú¼ÓËÙ±êÖ¾Î»
uint32_t step_groundu=1; //Ì¨½×ÄÚ²½Êý,×ß1²½×Ô¼õ1£¬¼õµ½0ºóÖØÐÂ¸³Öµ¿ªÊ¼ÏÂÒ»Ì¨½×
uint32_t step_groundv=1; //Ì¨½×ÄÚ²½Êý,×ß1²½×Ô¼õ1£¬¼õµ½0ºóÖØÐÂ¸³Öµ¿ªÊ¼ÏÂÒ»Ì¨½×
uint32_t step_groundw=1; //Ì¨½×ÄÚ²½Êý,×ß1²½×Ô¼õ1£¬¼õµ½0ºóÖØÐÂ¸³Öµ¿ªÊ¼ÏÂÒ»Ì¨½×
uint32_t step_groundx=1; //Ì¨½×ÄÚ²½Êý,×ß1²½×Ô¼õ1£¬¼õµ½0ºóÖØÐÂ¸³Öµ¿ªÊ¼ÏÂÒ»Ì¨½×
uint32_t step_groundy=1; //Ì¨½×ÄÚ²½Êý,×ß1²½×Ô¼õ1£¬¼õµ½0ºóÖØÐÂ¸³Öµ¿ªÊ¼ÏÂÒ»Ì¨½×
uint32_t step_groundz=1; //Ì¨½×ÄÚ²½Êý,×ß1²½×Ô¼õ1£¬¼õµ½0ºóÖØÐÂ¸³Öµ¿ªÊ¼ÏÂÒ»Ì¨½×
uint32_t step_groundu_B=1; //±¸·Ý
uint32_t step_groundv_B=1; //±¸·Ý
uint32_t step_groundw_B=1; //±¸·Ý
uint32_t step_groundx_B=1; //±¸·Ý
uint32_t step_groundy_B=1; //±¸·Ý
uint32_t step_groundz_B=1; //±¸·Ý
uint32_t speedup_benchu=0; //uµç»ú¼ÓËÙ¶ÎÌ¨½×Êý
uint32_t speedup_benchv=0; //vµç»ú¼ÓËÙ¶ÎÌ¨½×Êý
uint32_t speedup_benchw=0; //wµç»ú¼ÓËÙ¶ÎÌ¨½×Êý
uint32_t speedup_benchx=0; //xµç»ú¼ÓËÙ¶ÎÌ¨½×Êý
uint32_t speedup_benchy=0; //yµç»ú¼ÓËÙ¶ÎÌ¨½×Êý
uint32_t speedup_benchz=0; //zµç»ú¼ÓËÙ¶ÎÌ¨½×Êý
uint32_t speedup_benchu_B=0; //±¸·Ý
uint32_t speedup_benchv_B=0; //±¸·Ý
uint32_t speedup_benchw_B=0; //±¸·Ý
uint32_t speedup_benchx_B=0; //±¸·Ý
uint32_t speedup_benchy_B=0; //±¸·Ý
uint32_t speedup_benchz_B=0; //±¸·Ý
uint32_t speedcon_stepsu=0; //uµç»úÔÈËÙ²½Êý
uint32_t speedcon_stepsv=0; //vµç»úÔÈËÙ²½Ê
uint32_t speedcon_stepsw=0; //vµç»úÔÈËÙ²½Êýý
uint32_t speedcon_stepsx=0; //xµç»úÔÈËÙ²½Êý
uint32_t speedcon_stepsy=0; //yµç»úÔÈËÙ²½Êý
uint32_t speedcon_stepsz=0; //zµç»úÔÈËÙ²½Êý
int main(void)
{   uint8_t read_buffer[3];
	  uint32_t addrd=1; //µç»úËÙ·½ÏòÖ¸ÁîµØÖ·
	  uint32_t addro=2; //µç»úÍÑ»úÖ¸ÁîµØÖ·
	  uint32_t i=0,x=0,y=0;
    uint32_t offline=0;//°´Å¥¿ØÖÆÍÑ»ú
		uint8_t chu[5]={0,0,0,0,0};//u½ÓÊÕÊý¾ÝÊý
		uint8_t chv[5]={0,0,0,0,0};//v½ÓÊÕÊý¾ÝÊý
		uint8_t chw[5]={0,0,0,0,0};//v½ÓÊÕÊý¾ÝÊý
	  uint8_t chx[5]={0,0,0,0,0};//x½ÓÊÕÊý¾ÝÊý
	  uint8_t chy[5]={0,0,0,0,0};//y½ÓÊÕÊý¾ÝÊý
	  uint8_t chz[5]={0,0,0,0,0};//z½ÓÊÕÊý¾ÝÊý
		//PIT ³õÊ¼»¯½á¹¹
		PIT_InitTypeDef PIT_InitStruct1;
		//´®¿Ú³õÊ¼»¯½á¹¹
    UART_InitTypeDef UART_InitStruct1;
		//³õÊ¼»¯ÏµÍ³Ê±ÖÓ Ê¹ÓÃÍâ²¿50M¾§Õñ PLL±¶Æµµ½100M
    SystemClockSetup(ClockSource_EX50M,CoreClock_100M);
	  //GPIO¿ÚÊä³ö³õÊ¼»¯
    gpiooutinit();
	  //IO¿ÚÊäÈë³õÊ¼»¯
	  gpioininit();
		//IO¿Ú³õÊ¼»¯
	  gpioint();
		//ADÄ£¿é³õÊ¼»¯
		//ADC_init();
		//FLASH³õÊ¼»¯
    FLASH_Init();
//´®¿Ú1³õÊ¼»¯²Ù×÷
	 // UART_DebugPortInit(UART1_RX_PE0_TX_PE1 ,115200);
	 // DisplayCPUInfo();
		UART_InitStruct1.UARTxMAP = UART1_RX_PE0_TX_PE1; //UART1Ä£¿é Ó³ÉäÒý½Å:PTE0 PTE1
    UART_InitStruct1.UART_BaudRate = 115200;         //²¨ÌØÂÊ 115200
    UART_Init(&UART_InitStruct1); //³õÊ¼»¯´®¿Ú
    UART_ITConfig(UART1, UART_IT_RDRF, ENABLE); //¿ªÆô½ÓÊÕÖÐ¶Ï
    NVIC_EnableIRQ(UART1_RX_TX_IRQn);	 //½ÓÍ¨NVICÉÏ¶ÔÓ¦´®¿ÚÖÐ¶ÏÏß	
//´®¿Ú2³õÊ¼»¯²Ù×÷
		UART_InitStruct1.UARTxMAP = UART2_RX_D2_TX_D3; //UART2Ä£¿é Ó³ÉäÒý½Å:PTD2 PTD3
    UART_InitStruct1.UART_BaudRate = 115200;         //²¨ÌØÂÊ 115200
    UART_Init(&UART_InitStruct1); //³õÊ¼»¯´®¿Ú
    UART_ITConfig(UART2, UART_IT_RDRF, ENABLE);	//¿ªÆô½ÓÊÕÖÐ¶Ï
    NVIC_EnableIRQ(UART2_RX_TX_IRQn);	    //½ÓÍ¨NVICÉÏ¶ÔÓ¦´®¿ÚÖÐ¶ÏÏß
// //´®¿Ú3³õÊ¼»¯²Ù×÷
// 		UART_InitStruct1.UARTxMAP = UART3_RX_E4_TX_E5; //UART2Ä£¿é Ó³ÉäÒý½Å:PTE4 PTE5
//     UART_InitStruct1.UART_BaudRate = 115200;         //²¨ÌØÂÊ 115200
//     UART_Init(&UART_InitStruct1); //³õÊ¼»¯´®¿Ú
//     UART_ITConfig(UART3, UART_IT_RDRF, ENABLE);	//¿ªÆô½ÓÊÕÖÐ¶Ï
//     NVIC_EnableIRQ(UART3_RX_TX_IRQn);	    //½ÓÍ¨NVICÉÏ¶ÔÓ¦´®¿ÚÖÐ¶ÏÏß
   
	 //ÑÓÊ±³õÊ¼»¯		
		DelayInit();
		DelayUs(1);
		GPIO_ResetBits(PTD,7);//0
		DelayUs(1);
		GPIO_SetBits(PTD,7);//1
	  //¶¨Ê±Æ÷0 ÉèÖÃÑÓÊ±
    PIT_InitStruct1.PITx = PIT0;
    PIT_InitStruct1.PIT_Interval = 1000; //1000MS¶¨Ê±
    PIT_Init(&PIT_InitStruct1);
    PIT_ITConfig(PIT0, PIT_IT_TIF, ENABLE);
		NVIC_SetPriority(PIT0_IRQn,0);        //ÅäÖÃÓÅÏÈ¼¶ 
    NVIC_EnableIRQ(PIT0_IRQn);
	  //¶¨Ê±Æ÷1 ÉèÖÃÑÓÊ±
	  PIT_InitStruct1.PITx = PIT1;
    PIT_InitStruct1.PIT_Interval = 1000; //1000MS¶¨Ê±
    PIT_Init(&PIT_InitStruct1);
	  PIT_ITConfig(PIT1, PIT_IT_TIF, ENABLE);
		NVIC_SetPriority(PIT1_IRQn,1);        //ÅäÖÃÓÅÏÈ¼¶ 
    NVIC_EnableIRQ(PIT1_IRQn);
		//¶¨Ê±Æ÷2 ÉèÖÃÑÓÊ±
		PIT_InitStruct1.PITx = PIT2;
    PIT_InitStruct1.PIT_Interval = 1000; //1000MS¶¨Ê±
    PIT_Init(&PIT_InitStruct1);
	  PIT_ITConfig(PIT2, PIT_IT_TIF, ENABLE);
		NVIC_SetPriority(PIT2_IRQn,2);        //ÅäÖÃÓÅÏÈ¼¶ 
    NVIC_EnableIRQ(PIT2_IRQn);
		//¶¨Ê±Æ÷3 ÉèÖÃÑÓÊ±
		PIT_InitStruct1.PITx = PIT3;
    PIT_InitStruct1.PIT_Interval = 1000; //1000MS¶¨Ê±
    PIT_Init(&PIT_InitStruct1);
	  PIT_ITConfig(PIT3, PIT_IT_TIF, ENABLE);
		NVIC_SetPriority(PIT3_IRQn,3);        //ÅäÖÃÓÅÏÈ¼¶ 
    NVIC_EnableIRQ(PIT3_IRQn);
		PIT_Stop(PIT0); //Ä¬ÈÏ¹Ø±Õ¼ÆÊ±Æ÷£¬¿ªÆôÊ±´¥·¢ÖÐ¶Ïº¯Êý
		PIT_Stop(PIT1); //Ä¬ÈÏ¹Ø±Õ¼ÆÊ±Æ÷£¬¿ªÆôÊ±´¥·¢ÖÐ¶Ïº¯Êý
		PIT_Stop(PIT2); //Ä¬ÈÏ¹Ø±Õ¼ÆÊ±Æ÷£¬¿ªÆôÊ±´¥·¢ÖÐ¶Ïº¯Êý
		PIT_Stop(PIT3); //Ä¬ÈÏ¹Ø±Õ¼ÆÊ±Æ÷£¬¿ªÆôÊ±´¥·¢ÖÐ¶Ïº¯Êý
	 while(1) 
    {
			if(command==1)	//ÖÐ¶Ï½âÂë¸³Öµ
				{	   
//Ð´FLASH
					if((buf2[0]==87)&&(buf2[1]==82))//'w'
						{ 
							FLASH_WriteByte(CPUInfo.PFlashSize-128+addr_start,length,flash_buffer); 
					 // UART_SendData(UART1,'p');
			     // UART_SendData(UART1,'y');
			     // UART_SendData(UART1,flash_buffer[0]);
			     // UART_SendData(UART1,0); 
						} 
				 
//¶ÁFLASH
				 else if((buf2[0]==82)&&(buf2[1]==69))//'r''e'
						{
							FLASH_ReadByte(CPUInfo.PFlashSize-128+buf2[2],3,read_buffer);
#ifndef USE_UART1
							UART_SendData(UART2,'r');
							UART_SendData(UART2,read_buffer[0]);
							UART_SendData(UART2,read_buffer[1]);
							UART_SendData(UART2,read_buffer[2]);
#else
							UART_SendData(UART1,'r');
							UART_SendData(UART1,read_buffer[0]);
							UART_SendData(UART1,read_buffer[1]);
							UART_SendData(UART1,read_buffer[2]);							
#endif
						}
						
//²éÑ¯ÊÇ·ñµ½Î»uÖá
				else if((buf2[0]==70)&&(buf2[1]==85))//'f''u'
					{   
						if(completeu)
							{
#ifndef USE_UART1
								UART_SendData(UART2,'f');
								UART_SendData(UART2,'u');
								UART_SendData(UART2,1);
								UART_SendData(UART2,1);
#else
								UART_SendData(UART1,'f');
								UART_SendData(UART1,'u');
								UART_SendData(UART1,1);
								UART_SendData(UART1,1);
#endif
							}
						else
							{
#ifndef USE_UART1
								UART_SendData(UART2,'u');
								UART_SendData(UART2,'u');
								UART_SendData(UART2,1);
								UART_SendData(UART2,1);
#else
								UART_SendData(UART1,'u');
								UART_SendData(UART1,'u');
								UART_SendData(UART1,1);
								UART_SendData(UART1,1);								
#endif
							}
					}							
	
//²éÑ¯ÊÇ·ñµ½Î»vÖá
				else if((buf2[0]==70)&&(buf2[1]==86))//'f''v'
					{   
						if(completev)
							{
#ifndef USE_UART1
								UART_SendData(UART2,'f');
								UART_SendData(UART2,'v');
								UART_SendData(UART2,1);
								UART_SendData(UART2,1);
#else
								UART_SendData(UART1,'f');
								UART_SendData(UART1,'v');
								UART_SendData(UART1,1);
								UART_SendData(UART1,1);				
#endif
							}
						else
							{
#ifndef USE_UART1
								UART_SendData(UART2,'u');
								UART_SendData(UART2,'v');
								UART_SendData(UART2,1);
								UART_SendData(UART2,1);
#else
								UART_SendData(UART1,'u');
								UART_SendData(UART1,'v');
								UART_SendData(UART1,1);
								UART_SendData(UART1,1);								
#endif
							}
					}	
					
//²éÑ¯ÊÇ·ñµ½Î»wÖá
				else if((buf2[0]==70)&&(buf2[1]==87))//'f''w'
					{   
						if(completew)
							{
#ifndef USE_UART1
								UART_SendData(UART2,'f');
								UART_SendData(UART2,'w');
								UART_SendData(UART2,1);
								UART_SendData(UART2,1);
#else
								UART_SendData(UART1,'f');
								UART_SendData(UART1,'w');
								UART_SendData(UART1,1);
								UART_SendData(UART1,1);							
#endif
							}
						else
							{
#ifndef USE_UART1
								UART_SendData(UART2,'u');
								UART_SendData(UART2,'w');
								UART_SendData(UART2,1);
								UART_SendData(UART2,1);
#else
								UART_SendData(UART1,'u');
								UART_SendData(UART1,'w');
								UART_SendData(UART1,1);
								UART_SendData(UART1,1);							
#endif
							}
					}	
					
//²éÑ¯ÊÇ·ñµ½Î»xÖá/yÖá
				else if((buf2[0]==70)&&(buf2[1]==88))//'f''x'
					{   
						if(completex&&completey)
							{
#ifndef USE_UART1
								UART_SendData(UART2,'f');
								UART_SendData(UART2,'x');
								UART_SendData(UART2,1);
								UART_SendData(UART2,1);
#else
								UART_SendData(UART1,'f');
								UART_SendData(UART1,'x');
								UART_SendData(UART1,1);
								UART_SendData(UART1,1);								
#endif								
							}
						else
							{
#ifndef USE_UART1								
								UART_SendData(UART2,'u');
								UART_SendData(UART2,'x');
								UART_SendData(UART2,1);
								UART_SendData(UART2,1);
#else
								UART_SendData(UART1,'u');
								UART_SendData(UART1,'x');
								UART_SendData(UART1,1);
								UART_SendData(UART1,1);
#endif								
							}
					}	
					
					
//²éÑ¯ÊÇ·ñµ½Î»ZÖá
				else if((buf2[0]==70)&&(buf2[1]==90))//'f''z'
					{   
						if(completez)
							{
#ifndef USE_UART1	
								UART_SendData(UART2,'f');
								UART_SendData(UART2,'z');
								UART_SendData(UART2,pulscntz);
								UART_SendData(UART2,(pulscntz>>8));
#else
								UART_SendData(UART1,'f');
								UART_SendData(UART1,'z');
								UART_SendData(UART1,pulscntz);
								UART_SendData(UART1,(pulscntz>>8));
#endif								
							}
						else
							{
#ifndef USE_UART1	
								UART_SendData(UART2,'u');
								UART_SendData(UART2,'z');
								UART_SendData(UART2,pulscntz);
								UART_SendData(UART2,(pulscntz>>8));
#else
								UART_SendData(UART1,'u');
								UART_SendData(UART1,'z');
								UART_SendData(UART1,pulscntz);
								UART_SendData(UART1,(pulscntz>>8));								
#endif								
							}
					}						
					
// //²éÑ¯ÊÇ·ñµ½Î»uÖá/vÖá				
// 				else if((buf2[0]==70)&&(buf2[1]==85))//'f''u'
// 				{   
// 					if(completeu&&completev)
// 						{
// 							UART_SendData(UART2,'f');
// 							UART_SendData(UART2,'u');
// 							UART_SendData(UART2,pulscntu);
// 							UART_SendData(UART2,(pulscntu>>8));
// 						}
// 					else
// 						{
// 							UART_SendData(UART2,'u');
// 							UART_SendData(UART2,'u');
// 							UART_SendData(UART2,pulscntu);
// 							UART_SendData(UART2,(pulscntu>>8));
// 						}
// 				}	
// 					
		
// ÍÑ»ú
		   else if(buf2[0]==79)//'0'
			  {       
					if(buf2[1]==85)//'u'
						{
							GPIO_Write(PTC,(buf2[2]|(addro<<8)));//Ð´uÖáÖ¸ÁîºÍµØÖ·,buf2[2]=0x0f£¬addro=1£»
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,6);//CPLD¶ÁÖ¸Áî
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,6);//µÍµçÆ½½áÊø	
							D_Valueu=0;							 
						}							 
					else if(buf2[1]==86)//'v'
						{
							GPIO_Write(PTC,(buf2[2]|(addro<<8)));//Ð´vÖáÖ¸ÁîºÍµØÖ·,buf2[2]=0x0f£¬addro=1£»
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,7);//CPLD¶ÁÖ¸Áî
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,7);//µÍµçÆ½½áÊø
							D_Valuev=0;
						}
					else if(buf2[1]==87)//'w'
						{
							GPIO_Write(PTC,(buf2[2]|(addro<<8)));//Ð´wÖáÖ¸ÁîºÍµØÖ·,buf2[2]=0x0f£¬addro=1£»
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,10);//CPLD¶ÁÖ¸Áî
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,10);//µÍµçÆ½½áÊø
							D_Valuew=0;
						}
					else if(buf2[1]==88)//'x'
						{
							GPIO_Write(PTC,(buf2[2]|(addro<<8)));//Ð´xÖáÖ¸ÁîºÍµØÖ·,buf2[2]=0x0f£¬addro=1£»
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,11);//CPLD¶ÁÖ¸Áî
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,11);//µÍµçÆ½½áÊø	
							D_Valuex=0;							 
						}							 
					else if(buf2[1]==89)//'y'
						{
							GPIO_Write(PTC,(buf2[2]|(addro<<8)));//Ð´yÖáÖ¸ÁîºÍµØÖ·,buf2[2]=0x0f£¬addro=1£»
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,12);//CPLD¶ÁÖ¸Áî
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,12);//µÍµçÆ½½áÊø
							D_Valuey=0;
						}
						else if(buf2[1]==90)//'z'
						{
							GPIO_Write(PTC,(buf2[2]|(addro<<8)));//Ð´zÖáÖ¸ÁîºÍµØÖ·,buf2[2]=0x0f£¬addro=1£»
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,13);//CPLD¶ÁÖ¸Áî
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,13);//µÍµçÆ½½áÊø
							D_Valuez=0;
						}
        }
				 
//²éÑ¯Éè±¸	
		   else if(buf2[0]==68)//'d'
					{ 
						//FLASH_ReadByte(CPUInfo.PFlashSize-124,4,buf2);
						//if((buf2[0]==1)&&(buf2[1]==2)&&(buf2[2]==3)&&(buf2[3]==4))
#ifndef USE_UART1	
						UART_SendData(UART2,'d');
						UART_SendData(UART2,1);
						UART_SendData(UART2,1);
						UART_SendData(UART2,1);
#else
						UART_SendData(UART1,'d');
						UART_SendData(UART1,1);
						UART_SendData(UART1,1);
						UART_SendData(UART1,1);
#endif						
						/*GPIO_SetBits(PTB,3);
						DelayUs(1);
						GPIO_ResetBits(PTB,3);*/
					}
			
		  	
//²éÑ¯Î»ÖÃ×ø±ê
       else if(buf2[0]==80)//'p'
					{	
						if(buf2[1]==85)//'u'
							{
#ifndef USE_UART1	
							 UART_SendData(UART2,'p');
							 UART_SendData(UART2,'u');
							 UART_SendData(UART2,pulscntu);
							 UART_SendData(UART2,(pulscntu>>8));
#else
							 UART_SendData(UART1,'p');
							 UART_SendData(UART1,'u');
							 UART_SendData(UART1,pulscntu);
							 UART_SendData(UART1,(pulscntu>>8));
#endif								
							}
						else 
						if(buf2[1]==86)//'v'
							{
#ifndef USE_UART1	
								UART_SendData(UART2,'p');
								UART_SendData(UART2,'v');
								UART_SendData(UART2,pulscntv);
								UART_SendData(UART2,(pulscntv>>8));
#else
								UART_SendData(UART1,'p');
								UART_SendData(UART1,'v');
								UART_SendData(UART1,pulscntv);
								UART_SendData(UART1,(pulscntv>>8));
#endif								
							 }
						else
						if(buf2[1]==88)//'x'
							{
#ifndef USE_UART1	
							 UART_SendData(UART2,'p');
							 UART_SendData(UART2,'x');
							 UART_SendData(UART2,pulscntx);
							 UART_SendData(UART2,(pulscntx>>8));
#else
							 UART_SendData(UART1,'p');
							 UART_SendData(UART1,'x');
							 UART_SendData(UART1,pulscntx);
							 UART_SendData(UART1,(pulscntx>>8));								
#endif								
							}
						else if(buf2[1]==89)//'y'
							{
#ifndef USE_UART1	
								UART_SendData(UART2,'p');
								UART_SendData(UART2,'y');
								UART_SendData(UART2,pulscnty);
								UART_SendData(UART2,(pulscnty>>8));
#else
								UART_SendData(UART1,'p');
								UART_SendData(UART1,'y');
								UART_SendData(UART1,pulscnty);
								UART_SendData(UART1,(pulscnty>>8));			
#endif
							 }
				}
        
//²éÑ¯ÊÇ·ñÒÑ»Øµ½Ô­µã
			 else if(buf2[0]==71)//'g'
				{	
					if(buf2[1]==85)//'u'
						{ 
							limitu=GPIO_ReadInputDataBit(PTA,16);
							if(limitu)
// 							if(pulscntu==32768)
								{
									UART_SendData(UART2,'g');
									UART_SendData(UART2,'u');
									UART_SendData(UART2,1);
									UART_SendData(UART2,1);
								}
							else
								{
									UART_SendData(UART2,'a');
									UART_SendData(UART2,'u');
									UART_SendData(UART2,1);
									UART_SendData(UART2,1);
								}
						}	
						
					else if(buf2[1]==86)//'v'
						{ 
							limitv=GPIO_ReadInputDataBit(PTA,17);
							if(limitv)
								{
									UART_SendData(UART2,'g');
									UART_SendData(UART2,'v');
									UART_SendData(UART2,1);
									UART_SendData(UART2,1);
								}
							else
								{
									UART_SendData(UART2,'a');
									UART_SendData(UART2,'v');
									UART_SendData(UART2,1);
									UART_SendData(UART2,1);
								}
						}

					else if(buf2[1]==87)//'w'
						{ 
							if(pulscntw==32768)//µ±wÖáÊÕµ½»Øµ½Ô­µãÖ¸Áîºó
								{
									UART_SendData(UART2,'g');
									UART_SendData(UART2,'w');
									UART_SendData(UART2,1);
									UART_SendData(UART2,1);
								}
							else
								{
									UART_SendData(UART2,'a');
									UART_SendData(UART2,'w');
									UART_SendData(UART2,1);
									UART_SendData(UART2,1);
								}
						}	
						
          else if(buf2[1]==88)//'x'
						{ 
							limitx=GPIO_ReadInputDataBit(PTA,13);
							if(limitx)
								{
									UART_SendData(UART2,'g');
									UART_SendData(UART2,'x');
									UART_SendData(UART2,1);
									UART_SendData(UART2,1);
								}
							else
								{
									UART_SendData(UART2,'a');
									UART_SendData(UART2,'x');
									UART_SendData(UART2,1);
									UART_SendData(UART2,1);
								}
						}	
					else if(buf2[1]==89)//'y'
						{ 
							limity=GPIO_ReadInputDataBit(PTA,14);
							if(limity)
								{
									UART_SendData(UART2,'g');
									UART_SendData(UART2,'y');
									UART_SendData(UART2,1);
									UART_SendData(UART2,1);
								}
							else
								{
									UART_SendData(UART2,'a');
									UART_SendData(UART2,'y');
									UART_SendData(UART2,1);
									UART_SendData(UART2,1);
								}
						}
					else if(buf2[1]==90)//'z'
						{ 
							if(pulscntz==32768)//µ±zÖáÊÕµ½»Øµ½Ô­µãÖ¸Áîºó
								{
									UART_SendData(UART2,'g');
									UART_SendData(UART2,'z');
									UART_SendData(UART2,1);
									UART_SendData(UART2,1);
								}
							else
								{
									UART_SendData(UART2,'a');
									UART_SendData(UART2,'z');
									UART_SendData(UART2,1);
									UART_SendData(UART2,1);
								}
						}	
				}
       
//»Øµ½Ô­µã
			else if(buf2[0]==82)//'r'
				{
					if(buf2[1]==88)//'x'
						{
							 P_Valuex=buf2[2];
							 D_Valuex=0;
							 GPIO_Write(PTC,(D_Valuex|(addrd<<8)));//µÍµçÆ½Ð´xËÙ¶È·½Ïò
							 DelayUs(1);//ÑÓÊ±
							 GPIO_ToggleBit(PTC,11); //¸ßµçÆ½£¬cpld¶Á
							 DelayUs(1);//ÑÓÊ±
							 GPIO_ToggleBit(PTC,11);//µÍµçÆ½
							 if(P_Valuex&&(!limitx))
								{ 
										flagrx=1;
										PIT_SetLoadValue(PIT0,(500/P_Valuex));
										PIT_Start(PIT0); 
								}
					  }						 
						else if(buf2[1]==89)//'y'
						{ 
							P_Valuey=buf2[2];
						  D_Valuey=0;
						  GPIO_Write(PTC,(D_Valuey|(addrd<<8)));//µÍµçÆ½Ð´xËÙ¶È·½Ïò
					    DelayUs(1);//ÑÓÊ±
					    GPIO_ToggleBit(PTC,12); //¸ßµçÆ½£¬cpld¶Á
				      DelayUs(1);//ÑÓÊ±
					    GPIO_ToggleBit(PTC,12);//µÍµçÆ½
						  if(P_Valuey&&(!limity))
						  { flagry=1;
               PIT_SetLoadValue(PIT1,(500/P_Valuey));
						   PIT_Start(PIT1); 
						  }
						}
					
/***************************************¶ÁÈ¡IO¿ÚµÄ·½Ê½È·¶¨ÏÞÎ»µã***********************************************************************/					
	/*				if(buf2[1]==88)//'x'
						{
								P_Valuex=buf2[2];
								D_Valuex=0;//ËÙ¶È·½ÏòÎª0£¬µç»ú·´×ªÏòÁãµãÒÆ¶¯£»
								GPIO_Write(PTC,(D_Valuex|(addrd<<8)));//Ð´ÈëxÖáµÄÖ¸ÁîºÍÖ¸ÁîÎ»
								DelayUs(1);//ÑÓÊ±
								GPIO_ToggleBit(PTC,11); //CPLD¶ÁÖ¸Áî
								DelayUs(1);//ÑÓÊ±
								GPIO_ToggleBit(PTC,11);//µÍµçÆ½
								//limitx=GPIO_ReadInputDataBit(PTA,13);
								if(P_Valuex&&(!limitx))
								{ 
									flagrx=1;//»Øµ½Ô­µã±êÖ¾Î»
									PIT_SetLoadValue(PIT0,(500/P_Valuex));//Éè¶¨¶¨Ê±Æ÷µÄÔ¤ÉèÖµ£¬ÐÎ³ÉÂö³å£»
									PIT_Start(PIT0); //PIT0¿ªÊ¼¹¤×÷£»
								}
								while(!GPIO_ReadInputDataBit(PTA,13));
								PIT_Stop(PIT0);
								limitx=GPIO_ReadInputDataBit(PTA,13);
								pulscntx=0;
								flagrx=0;
							}						 
						else if(buf2[1]==89)//'y'
							{ 
								P_Valuey=buf2[2];
								D_Valuey=0;//ËÙ¶È·½ÏòÎª0£¬µç»ú·´×ªÏòÁãµãÒÆ¶¯£»
								GPIO_Write(PTC,(D_Valuey|(addrd<<8)));//Ð´ÈëyÖáµÄÖ¸ÁîºÍÖ¸ÁîÎ»
								DelayUs(1);//ÑÓÊ±
								GPIO_ToggleBit(PTC,12);  //CPLD¶ÁÖ¸Áî
								DelayUs(1);//ÑÓÊ±
								GPIO_ToggleBit(PTC,12);//µÍµçÆ½
								//limity=GPIO_ReadInputDataBit(PTA,14);
								if(P_Valuey&&(!limity))
								{ 
									flagry=1;//»Øµ½Ô­µã±êÖ¾Î»
									PIT_SetLoadValue(PIT1,(500/P_Valuey));//Éè¶¨¶¨Ê±Æ÷µÄÔ¤ÉèÖµ£¬ÐÎ³ÉÂö³å£»
									PIT_Start(PIT1); //PIT0¿ªÊ¼¹¤×÷£»
								}
								while(!GPIO_ReadInputDataBit(PTA,14));
								PIT_Stop(PIT1);
								limity=GPIO_ReadInputDataBit(PTA,14);
								pulscnty=0;
								flagry=0;
							}*/
/**************************************************************************************************************/	
            else if(buf2[1]==85)//'u'
							{
								flagpit=1;//uÖá´¥·¢±êÖ¾Î»
								P_Valueu=buf2[2];
								D_Valueu=0;//ËÙ¶È·½ÏòÎª0£¬µç»ú·´×ªÏòÁãµãÒÆ¶¯£»
								GPIO_Write(PTC,(D_Valueu|(addrd<<8)));//Ð´ÈëuÖáµÄÖ¸ÁîºÍÖ¸ÁîÎ»
								DelayUs(1);//ÑÓÊ±
								GPIO_ToggleBit(PTC,6); //CPLD¶ÁÖ¸Áî
								DelayUs(1);//ÑÓÊ±
								GPIO_ToggleBit(PTC,6);//µÍµçÆ½
								limitu=GPIO_ReadInputDataBit(PTA,16);
								if(P_Valueu&&(!limitu))
								{ 
									flagpit=1;
									flagru=1;//»Øµ½Ô­µã±êÖ¾Î»
									PIT_SetLoadValue(PIT3,(500/P_Valueu));//Éè¶¨¶¨Ê±Æ÷µÄÔ¤ÉèÖµ£¬ÐÎ³ÉÂö³å£»
									PIT_Start(PIT3); //PIT1¿ªÊ¼¹¤×÷£»
								}
								while(!GPIO_ReadInputDataBit(PTA,16));
								PIT_Stop(PIT3);
								limitu=GPIO_ReadInputDataBit(PTA,16);
								pulscntu=0;
								flagru=0;
								pulscntu=32768;//µ±uÖáÊÕµ½»Øµ½Ô­µãÖ¸Áî£¬pulscntz¸³Öµ32768£¬16Î»×î¸ßÎ»ÊÇ1£»
							}	
							
						else if(buf2[1]==86)//'v'
							{ 
								flagpit=2;
								P_Valuev=buf2[2];
								D_Valuev=0;//ËÙ¶È·½ÏòÎª0£¬µç»ú·´×ªÏòÁãµãÒÆ¶¯£»
								GPIO_Write(PTC,(D_Valuev|(addrd<<8)));//Ð´ÈëvÖáµÄÖ¸ÁîºÍÖ¸ÁîÎ»
								DelayUs(1);//ÑÓÊ±
								GPIO_ToggleBit(PTC,7); //CPLD¶ÁÖ¸Áî
								DelayUs(1);//ÑÓÊ±
								GPIO_ToggleBit(PTC,7);//µÍµçÆ½
								limitv=GPIO_ReadInputDataBit(PTA,17);
								if(P_Valuev&&(!limitv))
								{ 
									flagpit=2;
									flagrv=1;//»Øµ½Ô­µã±êÖ¾Î»
									PIT_SetLoadValue(PIT3,(500/P_Valuev));//Éè¶¨¶¨Ê±Æ÷µÄÔ¤ÉèÖµ£¬ÐÎ³ÉÂö³å£»
									PIT_Start(PIT3); //PIT3¿ªÊ¼¹¤×÷£»
								}
								while(!GPIO_ReadInputDataBit(PTA,17));
								PIT_Stop(PIT3);
								limitv=GPIO_ReadInputDataBit(PTA,17);
								pulscntv=0;
								flagrv=0;												
							}
							
						else if(buf2[1]==87)//'w'
							{ 
								pulscntw=32768;//µ±wÖáÊÕµ½»Øµ½Ô­µãÖ¸Áî£¬pulscntw¸³Öµ32768£¬16Î»×î¸ßÎ»ÊÇ1£»
							}
							
						else if(buf2[1]==90)//'z'
							{ 
								pulscntz=32768;//µ±zÖáÊÕµ½»Øµ½Ô­µãÖ¸Áî£¬pulscntz¸³Öµ32768£¬16Î»×î¸ßÎ»ÊÇ1£»
							}
				}			 
			
//ËÙ¶ÈÖµ
			else if(buf2[0]==69)//'e' ¡°2F 45 buf2[1] buf2[2] 01 23¡±
				{
					if(buf2[1]==85)//'u'
							chu[0]=buf2[2];
					else if(buf2[1]==86)//'v'
							chv[0]=buf2[2];
					else if(buf2[1]==87)//'v'
							chw[0]=buf2[2];					
          else if(buf2[1]==88)//'x'
							chx[0]=buf2[2];
					else if(buf2[1]==89)//'y'
							chy[0]=buf2[2];
					else if(buf2[1]==90)//'z'
						  chz[0]=buf2[2];
							
        }
		
//²½ÊýÖµ
			else if(buf2[0]==84)//'t'  ¡°2F 54 buf2[1] buf2[2] buf2[3] 23¡±
				{
					if(buf2[1]==85)//'u'
						{ 
							chu[1]=buf2[2];
						  chu[2]=buf2[3];
              flagsu=1;//±êÖ¾Î»À­¸ß£¬½øÈëºóÐøÑ­»·;
						}
					else if(buf2[1]==86)//'v'
						{ 
							chv[1]=buf2[2];
						  chv[2]=buf2[3];
              flagsv=1;//±êÖ¾Î»À­¸ß£¬½øÈëºóÐøÑ­»·;
						}
					else if(buf2[1]==87)//'w'
						{ 
							chw[1]=buf2[2];
						  chw[2]=buf2[3];
              flagsw=1;//±êÖ¾Î»À­¸ß£¬½øÈëºóÐøÑ­»·;
						}						
					else if(buf2[1]==88)//'x'
						{ 
// 							flagpit=1;
							chx[1]=buf2[2];
						  chx[2]=buf2[3];
              flagsx=1;//±êÖ¾Î»À­¸ß£¬½øÈëºóÐøÑ­»·;
						}
					else if(buf2[1]==89)//'y'
						{ 
// 							flagpit=2;
							chy[1]=buf2[2];
						  chy[2]=buf2[3];
              flagsy=1;//±êÖ¾Î»À­¸ß£¬½øÈëºóÐøÑ­»·£»
						}
					else if(buf2[1]==90)//'z'
						{ 
							chz[1]=buf2[2];
						  chz[2]=buf2[3];
              flagsz=1;//±êÖ¾Î»À­¸ß£¬½øÈëºóÐøÑ­»·£»
						} 
				}
				
//Ð±ÂÊÖµ
			else if(buf2[0]==65)//'a'  ¡°2F 54 buf2[1] buf2[2] buf2[3] 23¡±
			  {
						if(buf2[1]==85)//'u'
						{	
							chu[3]=buf2[2];//Ð±ÂÊ
						  chu[4]=buf2[3];
						}
						else if(buf2[1]==86)//'v'
						{	
							chv[3]=buf2[2];//Ð±ÂÊ
						  chv[4]=buf2[3];
						}
						else if(buf2[1]==87)//'w'
						{	
							chw[3]=buf2[2];//Ð±ÂÊ
						  chw[4]=buf2[3];
						}						
            else if(buf2[1]==88)//'x'
						{	
							chx[3]=buf2[2];//Ð±ÂÊ
						  chx[4]=buf2[3];
						}
						else if(buf2[1]==89)//'y'
						{	
							chy[3]=buf2[2];//Ð±ÂÊ
						  chy[4]=buf2[3];
						}
						else if(buf2[1]==90)//'z'
						{
							chz[3]=buf2[2];//Ð±ÂÊ
						  chz[4]=buf2[3];
						}
				} 
				
//µç»ú×Ô¶¯×ßÒ»È¦
      else if((buf2[0]==70)&&(buf2[1]==67))//'f''c'
				{
            chz[1]=buf2[2];
						chz[2]=buf2[3];
				    flagfc=1;//±êÖ¾Î»À­¸ß£¬½øÈëºóÐøÑ­»·£»
				    flagfc1=1;//±êÖ¾Î»À­¸ß£¬½øÈëºóÐøÑ­»·£»
				}	
			
//µÃµ½×î¼ÑÍ¼ÏñºÅ
     else	if((buf2[0]==73)&&(buf2[1]==78))//'i'n''		
				{
           imagenum=buf2[2];
			     flaggo=1;
 
				}

//*************************²âÊÔ´úÂë********************************************//		 
		 else	if(buf2[0]==75)//ÑéÖ¤»Øµ½Ô­µã¹¦ÄÜ
				{
						flago=1;
				}
				
//*************************²âÊÔ´úÂë********************************************//		 		 
				command=0;
		}
    
			

//ÉÏÎ»»ú´®¿Ú¿ØÖÆ
//uÖáÉè¶¨²½³¤	   
   	if(flagsu && 0)
			  {  
					if(AutoMode)//³õÊ¼»¯Ä¬ÈÏÎª1
						{ 
							P_Valueu=chu[0];//ËÙ¶È
							D_Valueu=chu[2]>>7;//È¡chu[2]µÚ°ËÎ»Îª·½Ïò±êÖ¾Î»£¬¸³ÖµÒÔ0x80Îª·Ö½çÏß£¬´óÓÚµÈÓÚ0x80Õý×ª£»Ð¡ÓÚµÈÓÚ0x80·´×ª£»
							stepu=chu[1]|((chu[2]&0x7f)<<8);//²½Êý£¬È¡chu[2]µÍ7Î»×÷ÎªÐÂÊý¾ÝµÄ¸ß°ËÎ»£¨×î¸ßÎ»ÎªÁã£©£¬µÍ°ËÎ»Îªchu[1]£»
							Gradientu=(chu[4]<<8)|chu[3];//Ð±ÂÊÖµ£¬È¡chx[4]Îª¸ß8Î»£¬chx[3]ÎªµÍ°ËÎ»£»
						}
					step_groundu=1;//Ã¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
					step_groundu_B=step_groundu;//±¸·ÝÃ¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
					
			    if((D_Valueu==0)&&(stepu>pulscntu+30))
						stepu=pulscntu+30;
				  
					if(P_Valueu&&stepu)
						 {
							 if((P_Valueu>1)&&(stepu/step_groundu/3)&&Gradientu&&(((P_Valueu-1)*1000)/(Gradientu*step_groundu)))
								{
									if((((P_Valueu-1)*1000)/(Gradientu*step_groundu))<(stepu/step_groundu/3))
										speedup_benchu=((P_Valueu-1)*1000)/(Gradientu*step_groundu);
									else 
										speedup_benchu=stepu/step_groundu/3; //×ÜÂö³åÆ½¾ù·Ö³É3¶Î£¬¼ÆËãÃ¿¶ÎµÄÌ¨½×Êý
									flagupu=1;//¼ÓËÙ±êÖ¾Î»Îª1£»
									P_Valueu=1;//ËÙ¶ÈÎª1£»
									CP_Valueu=1000;//µ±Ç°ËÙ¶ÈÎªËÙ¶ÈÎª1000£»
								}
							 else   
								{  
									flagupu=0;
									speedup_benchu=0;
								}
							speedcon_stepsu=stepu-speedup_benchu*2*step_groundu;
							speedup_benchu_B=speedup_benchu;
							GPIO_Write(PTC,(D_Valueu|(addrd<<8)));//µÍµçÆ½Ð´xËÙ¶È·½Ïò
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,6); //¸ßµçÆ½£¬cpld¶Á ËÙ¶È·½Ïò
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,6);//µÍµçÆ½
							completeu=0;
 							PIT_SetLoadValue(PIT3,(500/P_Valueu));
							PIT_Start(PIT3);
							limitu=GPIO_ReadInputDataBit(PTA,16);
							if(limitu)
							 { 
								 if(D_Valueu)//µç»úÕý×ª
									 {
											flagpit=1;
											PIT_SetLoadValue(PIT3,(500/P_Valueu));
											PIT_Start(PIT3);
											//limitu=0;
									 }
							 }
							else
							 {
									flagpit=1;
									PIT_SetLoadValue(PIT3,(500/P_Valueu));
									PIT_Start(PIT3);
							 }
						}
					else completeu=1;
			  flagsu=0;
		   }
			 
//vÖáÉè¶¨²½³¤	   
   		 if(flagsv && 0)
			  {  
					if(AutoMode)//³õÊ¼»¯Ä¬ÈÏÎª1
						{ 
							P_Valuev=chv[0];//ËÙ¶È
							D_Valuev=chv[2]>>7;//È¡chx[2]µÚ°ËÎ»Îª·½Ïò±êÖ¾Î»£¬¸³ÖµÒÔ0x80Îª·Ö½çÏß£¬´óÓÚµÈÓÚ0x80Õý×ª£»Ð¡ÓÚµÈÓÚ0x80·´×ª£»
							stepv=chv[1]|((chv[2]&0x7f)<<8);//²½Êý
							Gradientv=(chv[4]<<8)|chv[3];//Ð±ÂÊÖµ£¬È¡chx[4]Îª¸ß8Î»£¬chx[3]ÎªµÍ°ËÎ»£»
						}
					step_groundv=1;//Ã¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
					step_groundv_B=step_groundv;//±¸·ÝÃ¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
					
					if((D_Valuev==0)&&(stepv>pulscntv+30))
						stepv=pulscntv+30;
				  
					if(P_Valuev&&stepv)
						 {
							 if((P_Valuev>1)&&(stepv/step_groundv/3)&&Gradientv&&(((P_Valuev-1)*1000)/(Gradientv*step_groundv)))
								{
										if((((P_Valuev-1)*1000)/(Gradientv*step_groundv))<(stepv/step_groundv/3))
											 speedup_benchv=((P_Valuev-1)*1000)/(Gradientv*step_groundv);
										else 
											speedup_benchv=stepv/step_groundv/3; //×ÜÂö³åÆ½¾ù·Ö³É3¶Î£¬¼ÆËãÃ¿¶ÎµÄÌ¨½×Êý
										
										flagupv=1;//¼ÓËÙ±êÖ¾Î»Îª1£»
										P_Valuev=1;//ËÙ¶ÈÎª1£»
										CP_Valuev=1000;//µ±Ç°ËÙ¶ÈÎªËÙ¶ÈÎª1000£»
								}
							 else   
								{  
										flagupv=0;
										speedup_benchv=0;
								}
							speedcon_stepsv=stepv-speedup_benchv*2*step_groundv;
							speedup_benchv_B=speedup_benchv;
							GPIO_Write(PTC,(D_Valuev|(addrd<<8)));//µÍµçÆ½Ð´xËÙ¶È·½Ïò
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,7); //¸ßµçÆ½£¬cpld¶Á ËÙ¶È·½Ïò
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,7);//µÍµçÆ½
							completev=0;
							limitv=GPIO_ReadInputDataBit(PTA,17);
							if(limitv)
							 { 
								 if(D_Valuev)//µç»úÕý×ª
									 {
											flagpit=2;
											PIT_SetLoadValue(PIT3,(500/P_Valuev));
											PIT_Start(PIT3);
											//limitv=0;
									 }
							 }
							else
							 {
									flagpit=2;
									PIT_SetLoadValue(PIT3,(500/P_Valuev));
									PIT_Start(PIT3);
							 }
						}
					else completev=1;
			  flagsv=0;
		   }

//ÉÏÎ»»ú´®¿Ú¿ØÖÆ
//wÖáÉè¶¨²½³¤	   
   	if(flagsw)
			  {  
					if(AutoMode)//³õÊ¼»¯Ä¬ÈÏÎª1
						{ 
							P_Valuew=chw[0];//ËÙ¶È
							D_Valuew=chw[2]>>7;//È¡chu[2]µÚ°ËÎ»Îª·½Ïò±êÖ¾Î»£¬¸³ÖµÒÔ0x80Îª·Ö½çÏß£¬´óÓÚµÈÓÚ0x80Õý×ª£»Ð¡ÓÚµÈÓÚ0x80·´×ª£»
							stepw=chw[1]|((chw[2]&0x7f)<<8);//²½Êý£¬È¡chu[2]µÍ7Î»×÷ÎªÐÂÊý¾ÝµÄ¸ß°ËÎ»£¨×î¸ßÎ»ÎªÁã£©£¬µÍ°ËÎ»Îªchu[1]£»
							Gradientw=(chw[4]<<8)|chw[3];//Ð±ÂÊÖµ£¬È¡chx[4]Îª¸ß8Î»£¬chx[3]ÎªµÍ°ËÎ»£»
						}
					step_groundw=1;//Ã¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
					step_groundw_B=step_groundw;//±¸·ÝÃ¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
					
				  
					if(P_Valuew&&stepw)
						 {
							 if((P_Valuew>1)&&(stepw/step_groundw/3)&&Gradientw&&(((P_Valuew-1)*1000)/(Gradientw*step_groundw)))
								{
									if((((P_Valuew-1)*1000)/(Gradientw*step_groundw))<(stepw/step_groundw/3))
										speedup_benchw=((P_Valuew-1)*1000)/(Gradientw*step_groundw);
									else 
										speedup_benchw=stepw/step_groundw/3; //×ÜÂö³åÆ½¾ù·Ö³É3¶Î£¬¼ÆËãÃ¿¶ÎµÄÌ¨½×Êý
									flagupw=1;//¼ÓËÙ±êÖ¾Î»Îª1£»
									P_Valuew=1;//ËÙ¶ÈÎª1£»
									CP_Valuew=1000;//µ±Ç°ËÙ¶ÈÎªËÙ¶ÈÎª1000£»
								}
							 else   
								{  
									flagupw=0;
									speedup_benchw=0;
								}
							speedcon_stepsw=stepw-speedup_benchw*2*step_groundw;
							speedup_benchw_B=speedup_benchw;
							GPIO_Write(PTC,(D_Valuew|(addrd<<8)));//µÍµçÆ½Ð´xËÙ¶È·½Ïò
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,10); //¸ßµçÆ½£¬cpld¶Á ËÙ¶È·½Ïò
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,10);//µÍµçÆ½
							completew=0;
							flagpit=3;
 							PIT_SetLoadValue(PIT3,(500/P_Valuew));
							PIT_Start(PIT3);
						}
					else completew=1;
			  flagsw=0;
		   }
			 
			 
//xÖáÉè¶¨²½³¤	   
   		 if(flagsx)
			  {  
					if(AutoMode)//³õÊ¼»¯Ä¬ÈÏÎª1
						{ 
							P_Valuex=chx[0];//ËÙ¶È
							D_Valuex=chx[2]>>7;//È¡chx[2]µÚ°ËÎ»Îª·½Ïò±êÖ¾Î»£¬¸³ÖµÒÔ0x80Îª·Ö½çÏß£¬´óÓÚµÈÓÚ0x80Õý×ª£»Ð¡ÓÚµÈÓÚ0x80·´×ª£»
							stepx=chx[1]|((chx[2]&0x7f)<<8);//²½Êý£¬È¡chx[2]µÍ7Î»×÷ÎªÐÂÊý¾ÝµÄ¸ß°ËÎ»£¨×î¸ßÎ»ÎªÁã£©£¬µÚ°ËÎ»Îªchx[1]£»
							Gradientx=(chx[4]<<8)|chx[3];//Ð±ÂÊÖµ£¬È¡chx[4]Îª¸ß8Î»£¬chx[3]ÎªµÍ°ËÎ»£»
						}
					step_groundx=1;//Ã¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
					step_groundx_B=step_groundx;//±¸·ÝÃ¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
					
					if((D_Valuex==0)&&(stepx>pulscntx+30))
						stepx=pulscntx+30;
				  
					if(P_Valuex&&stepx)
						 {
							 if((P_Valuex>1)&&(stepx/step_groundx/3)&&Gradientx&&(((P_Valuex-1)*1000)/(Gradientx*step_groundx)))
								{
										if((((P_Valuex-1)*1000)/(Gradientx*step_groundx))<(stepx/step_groundx/3))
											 speedup_benchx=((P_Valuex-1)*1000)/(Gradientx*step_groundx);
										else 
											speedup_benchx=stepx/step_groundx/3; //×ÜÂö³åÆ½¾ù·Ö³É3¶Î£¬¼ÆËãÃ¿¶ÎµÄÌ¨½×Êý
										
										flagupx=1;//¼ÓËÙ±êÖ¾Î»Îª1£»
										P_Valuex=1;//ËÙ¶ÈÎª1£»
										CP_Valuex=1000;//µ±Ç°ËÙ¶ÈÎªËÙ¶ÈÎª1000£»
								}
							 else   
								{  
										flagupx=0;
										speedup_benchx=0;
								}
							speedcon_stepsx=stepx-speedup_benchx*2*step_groundx;
							speedup_benchx_B=speedup_benchx;
							GPIO_Write(PTC,(D_Valuex|(addrd<<8)));//µÍµçÆ½Ð´xËÙ¶È·½Ïò
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,11); //¸ßµçÆ½£¬cpld¶Á ËÙ¶È·½Ïò
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,11);//µÍµçÆ½
							completex=0;
							limitx=GPIO_ReadInputDataBit(PTA,13);
							if(limitx)
							 { 
								 if(D_Valuex)//µç»úÕý×ª
									 {
											PIT_SetLoadValue(PIT0,(500/P_Valuex));
											PIT_Start(PIT0);
											limitx=0;
									 }
							 }
							else
							 {
									PIT_SetLoadValue(PIT0,(500/P_Valuex));
									PIT_Start(PIT0);
							 }
						}
					else completex=1;
			  flagsx=0;
		   }
			 
//yÖáÉè¶¨²½³¤	 
			 		 
			 if(flagsy)
			  {  
					if(AutoMode)//³õÊ¼»¯Ä¬ÈÏÎª1
						{ 
							P_Valuey=chy[0];//ËÙ¶È
							D_Valuey=chy[2]>>7;//È¡chx[2]µÚ°ËÎ»Îª·½Ïò±êÖ¾Î»£¬¸³ÖµÒÔ0x80Îª·Ö½çÏß£¬´óÓÚµÈÓÚ0x80Õý×ª£»Ð¡ÓÚµÈÓÚ0x80·´×ª£»
							stepy=chy[1]|((chy[2]&0x7f)<<8);//²½Êý£¬È¡chx[2]µÍ7Î»×÷ÎªÐÂÊý¾ÝµÄ¸ß°ËÎ»£¨×î¸ßÎ»ÎªÁã£©£¬µÚ°ËÎ»Îªchx[1]£»
							Gradienty=(chy[4]<<8)|chy[3];//Ð±ÂÊÖµ£¬È¡chx[4]Îª¸ß8Î»£¬chx[3]ÎªµÍ°ËÎ»£»
						}
					step_groundy=1;//Ã¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
					step_groundy_B=step_groundy;//±¸·ÝÃ¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
					
					if((D_Valuey==0)&&(stepy>pulscnty+30))
						stepy=pulscnty+30;
				  
					if(P_Valuey&&stepy)
						 {
							 if((P_Valuey>1)&&(stepy/step_groundy/3)&&Gradienty&&(((P_Valuey-1)*1000)/(Gradienty*step_groundy)))
								{
										if((((P_Valuey-1)*1000)/(Gradienty*step_groundy))<(stepy/step_groundy/3))
											 speedup_benchy=((P_Valuey-1)*1000)/(Gradienty*step_groundy);
										else 
											speedup_benchy=stepy/step_groundy/3; //×ÜÂö³åÆ½¾ù·Ö³É3¶Î£¬¼ÆËãÃ¿¶ÎµÄÌ¨½×Êý
										
										flagupy=1;//¼ÓËÙ±êÖ¾Î»Îª1£»
										P_Valuey=1;//ËÙ¶ÈÎª1£»
										CP_Valuey=1000;//µ±Ç°ËÙ¶ÈÎªËÙ¶ÈÎª1000£»
								}
							 else   
								{  
										flagupy=0;
										speedup_benchy=0;
								}
							speedcon_stepsy=stepy-speedup_benchy*2*step_groundy;
							speedup_benchy_B=speedup_benchy;
							GPIO_Write(PTC,(D_Valuey|(addrd<<8)));//µÍµçÆ½Ð´xËÙ¶È·½Ïò
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,12); //¸ßµçÆ½£¬cpld¶Á ËÙ¶È·½Ïò
							DelayUs(1);//ÑÓÊ±
							GPIO_ToggleBit(PTC,12);//µÍµçÆ½
							completey=0;
							limity=GPIO_ReadInputDataBit(PTA,14);
							if(limity)
							 { 
								 if(D_Valuey)//µç»úÕý×ª
									 {
											PIT_SetLoadValue(PIT1,(500/P_Valuey));
											PIT_Start(PIT1);
											limity=0;
									 }
							 }
							else
							 {
								PIT_SetLoadValue(PIT1,(500/P_Valuey));
								PIT_Start(PIT1);
							 }
						}
					else completey=1;
			  flagsy=0;
		   }
			 
//zÖáÉè¶¨²½³¤	
			if(flagsz)
				{   
						if(AutoMode)
							{ 
								P_Valuez=chz[0];
								D_Valuez=chz[2]>>7;
								stepz=chz[1]|((chz[2]&0x7f)<<8);
								Gradientz=(chz[4]<<8)|chz[3];//Ð±ÂÊÖµ//Ð±ÂÊÖµ					 
							}
						step_groundz=1;//Ã¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
						step_groundz_B=step_groundz;//±¸·ÝÃ¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
						if(P_Valuez&&stepz)
							{ 
								if((P_Valuez>1)&&(stepz/step_groundz/3)&&Gradientz&&(((P_Valuez-1)*1000)/(Gradientz*step_groundz)))
									{
										if((((P_Valuez-1)*1000)/(Gradientz*step_groundz))<(stepz/step_groundz/3))
											speedup_benchz=((P_Valuez-1)*1000)/(Gradientz*step_groundz);
										else speedup_benchz=stepz/step_groundz/3; //×ÜÂö³åÆ½¾ù·Ö³É3¶Î£¬¼ÆËãÃ¿¶ÎµÄÌ¨½×Êý
                    flagupz=1;
								    P_Valuez=1;
								    CP_Valuez=1000;
									}
									else   
									{  
										flagupz=0;
										speedup_benchz=0;
									}
									speedcon_stepsz=stepz-speedup_benchz*2*step_groundz;
									speedup_benchz_B=speedup_benchz;
									GPIO_Write(PTC,(D_Valuez|(addrd<<8)));//µÍµçÆ½Ð´Z·½Ïò
									DelayUs(1);//ÑÓÊ±
									GPIO_ToggleBit(PTC,13);//¸ßµçÆ½£¬cpld¶Á
									DelayUs(1);//ÑÓÊ±
									GPIO_ToggleBit(PTC,13);//µÍµçÆ½½áÊø		  
									completez=0;
									PIT_SetLoadValue(PIT2,(500/P_Valuez));
									PIT_Start(PIT2);
							}
						else 
							completez=1;
					flagsz=0;
				}
				
//¾Û½¹Ä£Ê½µç»ú×Ô¶¯×ßÒ»È¦
			if(flagfc)
				{  			
					if(flagfc1)
					{
							P_Valuez=chz[0];
							D_Valuez=chz[2]>>7;
							stepz=(chz[1]|((chz[2]&0x7f)<<8))/2;
							Gradientz=(chz[4]<<8)|chz[3];//Ð±ÂÊÖµ					  
					}	
          if(flagfc2)
          {
							P_Valuez=chz[0];
							D_Valuez=1-D_Valuez;
							stepz=chz[1]|((chz[2]&0x7f)<<8);
							Gradientz=(chz[4]<<8)|chz[3];//Ð±ÂÊÖµ	
 							GPIO_SetBits(PTB,1);
					}					
				  step_groundz=1;//Ã¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
					step_groundz_B=step_groundz;//±¸·ÝÃ¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
					if(P_Valuez&&stepz)
						{ 
							if((P_Valuez>1)&&(stepz/step_groundz/3)&&Gradientz&&(((P_Valuez-1)*1000)/(Gradientz*step_groundz)))
								{
									if((((P_Valuez-1)*1000)/(Gradientz*step_groundz))<(stepz/step_groundz/3))
									   speedup_benchz=((P_Valuez-1)*1000)/(Gradientz*step_groundz);
									else 
										speedup_benchz=stepz/step_groundz/3; //×ÜÂö³åÆ½¾ù·Ö³É3¶Î£¬¼ÆËãÃ¿¶ÎµÄÌ¨½×Êý
                  flagupz=1;
								  P_Valuez=1;
								  CP_Valuez=1000;
								}
							else   
								{  
									flagupz=0;
									speedup_benchz=0;
								}
								speedcon_stepsz=stepz-speedup_benchz*2*step_groundz;
								speedup_benchz_B=speedup_benchz;
								GPIO_Write(PTC,(D_Valuez|(addrd<<8)));//µÍµçÆ½Ð´Z·½Ïò
								DelayUs(1);//ÑÓÊ±
								GPIO_ToggleBit(PTC,13);//¸ßµçÆ½£¬cpld¶Á
								DelayUs(1);//ÑÓÊ±
								GPIO_ToggleBit(PTC,13);//µÍµçÆ½½áÊø		  
								completez=0;
								PIT_SetLoadValue(PIT2,(500/P_Valuez));
								PIT_Start(PIT2);
					 }
					else completez=1;
					flagfc=0;
      }
			
//¾Û½¹Ä£Ê½µç»ú»Øµ½×î¼ÑÍ¼ÏñÎ»ÖÃ
			if(flaggo)
				{      
							P_Valuez=chz[0];
							if(position[imagenum]>pulscntz)
								{
										D_Valuez=1;
								} 
							else
								{
										D_Valuez=0;
								}
							stepz=abs(position[imagenum]-pulscntz);
							Gradientz=(chz[4]<<8)|chz[3];
							j=0;
							step_groundz=1;//Ã¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
							step_groundz_B=step_groundz;//±¸·ÝÃ¿¸öÌ¨½×µÄ²½Êý..............Ì¨½×Êý±ÈÈç¸Ä³É5
							if(P_Valuez&&stepz)
								{ 
									if((P_Valuez>1)&&(stepz/step_groundz/3)&&Gradientz&&(((P_Valuez-1)*1000)/(Gradientz*step_groundz)))
										{
											if((((P_Valuez-1)*1000)/(Gradientz*step_groundz))<(stepz/step_groundz/3))
												speedup_benchz=((P_Valuez-1)*1000)/(Gradientz*step_groundz);
											else 
												speedup_benchz=stepz/step_groundz/3; //×ÜÂö³åÆ½¾ù·Ö³É3¶Î£¬¼ÆËãÃ¿¶ÎµÄÌ¨½×Êý
											flagupz=1;
											P_Valuez=1;
											CP_Valuez=1000;
										}
									else   
										{  
											flagupz=0;
											speedup_benchz=0;
										}
									speedcon_stepsz=stepz-speedup_benchz*2*step_groundz;
									speedup_benchz_B=speedup_benchz;
									GPIO_Write(PTC,(D_Valuez|(addrd<<8)));//µÍµçÆ½Ð´Z·½Ïò
									DelayUs(1);//ÑÓÊ±
									GPIO_ToggleBit(PTC,13);//¸ßµçÆ½£¬cpld¶Á
									DelayUs(1);//ÑÓÊ±
									GPIO_ToggleBit(PTC,13);//µÍµçÆ½½áÊø		  
									completez=0;
									PIT_SetLoadValue(PIT2,(500/P_Valuez));
									PIT_Start(PIT2);
								}
							else 
								completez=1;
					flaggo=0; 
				}

/*
//uÖá¡¢vÖá»Øµ½Ô­µã
		if(flago1==1)	 
				{      
				  D_Valueu=0;
					D_Valuev=0;
				  P_Valueu=10;
					P_Valuev=10;
					GPIO_Write(PTC,(D_Valueu|(addrd<<8)));//µÍµçÆ½Ð´xËÙ¶È·½Ïò--yÖáËÙ¶È·½ÏòÓëxÖáÒ»ÖÂ
					DelayUs(1);//ÑÓÊ±
					GPIO_ToggleBit(PTC,11); //¸ßµçÆ½£¬cpld¶Á
				  DelayUs(1);//ÑÓÊ±
					GPIO_ToggleBit(PTC,11);//µÍµçÆ½
					DelayUs(1);//ÑÓÊ±
					GPIO_ToggleBit(PTC,12); //¸ßµçÆ½£¬cpld¶Á
				  DelayUs(1);//ÑÓÊ±
					GPIO_ToggleBit(PTC,12);//µÍµçÆ½
					
					limitu=GPIO_ReadInputDataBit(PTA,16);
					limitv=GPIO_ReadInputDataBit(PTA,17);
					if(!limitu|!limitv)
						{ 
							flagpit=3;
							PIT_SetLoadValue(PIT3,(500/P_Valuex));
							PIT_Start(PIT3); 
						}
					while(!GPIO_ReadInputDataBit(PTA,16));
					while(!GPIO_ReadInputDataBit(PTA,17));
					PIT_Stop(PIT3);
					pulscntu=0;
					pulscntv=0;
					WriteV(0xf3,0); 
					WriteV(0xf4,0); 
					flago=0;
				}	
	*/			
//xÖá¡¢yÖá»Øµ½Ô­µã
			if(flago==1)	 
				{      
				    GPIO_Write(PTC,(D_Valuex|(addrd<<8)));//µÍµçÆ½Ð´xËÙ¶È·½Ïò
					  DelayUs(1);//ÑÓÊ±
					  GPIO_ToggleBit(PTC,11); //¸ßµçÆ½£¬cpld¶Á
				    DelayUs(1);//ÑÓÊ±
					  GPIO_ToggleBit(PTC,11);//µÍµçÆ½
						limitx=GPIO_ReadInputDataBit(PTA,13);
						if(!limitx)
							{ 
									flagrx=1;
									PIT_SetLoadValue(PIT0,(500/P_Valuex));
									PIT_Start(PIT0); 
							}
							
						GPIO_Write(PTC,(D_Valuey|(addrd<<8)));//µÍµçÆ½Ð´yËÙ¶È·½Ïò
						DelayUs(1);//ÑÓÊ±
						GPIO_ToggleBit(PTC,12); //¸ßµçÆ½£¬cpld¶Á
						DelayUs(1);//ÑÓÊ±
						GPIO_ToggleBit(PTC,12);//µÍµçÆ½
						limity=GPIO_ReadInputDataBit(PTA,14);
						 if(!limity)
						  { 
									flagry=1;
									PIT_SetLoadValue(PIT1,(500/P_Valuey));
									PIT_Start(PIT1); 
						  }	
						WriteV(0xf1,0); 
						WriteV(0xf2,0); 
						flago=0;		
				}	

// //ÏÔÊ¾Òº¾§ÉÏµÄ²ÎÊý			
// 			if(LoadPara)
// 				{  
// 					LoadLCDPara();
// 					DelayMs(150);
// 					ReadV(0xe1,0x01);
// 					DelayMs(50);
// 					ReadV(0xe2,0x01);
// 					DelayMs(50);
// 					ReadV(0xe3,0x01);
// 					DelayMs(50);
// 					ReadV(0xe4,0x01);
// 					DelayMs(50);
// 					ReadV(0x08,0x01);
// 					DelayMs(50);
// 					LoadPara=0;
// 				}

// //±£´æ²½³¤				
// 		if(SaveSteplen)
// 				{  
// 					SaveLCDPara();
// 					DelayMs(50);
// 					SaveSteplen=0;
// 				}
				
//±£´æÇøÓò±ß½çÖµ				
// 		if(SaveArea)//±£´æÇøÓò±ß½çÖµ
// 				{
// 					WriteV(0xe5,UMax_Pos);//¾ßÌåµØÖ·ÐèÒªºóÆÚÈ·ÈÏ
// 					WriteV(0xe6,UMin_Pos);//¾ßÌåµØÖ·ÐèÒªºóÆÚÈ·ÈÏ
// 					WriteV(0xe7,VMax_Pos);//¾ßÌåµØÖ·ÐèÒªºóÆÚÈ·ÈÏ
// 					WriteV(0xe8,VMin_Pos);//¾ßÌåµØÖ·ÐèÒªºóÆÚÈ·ÈÏ
// 					WriteV(0xe1,XMax_Pos);
// 					WriteV(0xe2,XMin_Pos);
// 					WriteV(0xe3,YMax_Pos);
// 					WriteV(0xe4,YMin_Pos);
// 					SaveLCDPara();
// 					DelayMs(100);
// 					SaveArea=0; 
// 				}
  	 
//²éÑ¯µÄ·½Ê½¼ì²âÏÞÎ»µã
 	if(D_Valuex==0)
				{   
					x=0;
					for(i=0;i<1000;i++)
						{ 
							if(GPIO_ReadInputDataBit(PTA,13))
								{
									x++;
								}
						}
					if(x>999)
					{
						PIT_Stop(PIT0);  
						limitx=1;
						pulscntx=0;
						flagrx=0;
					}
					else 
						limitx=0;
					}
					
		if(D_Valuey==0)
				{   
					y=0;
					for(i=0;i<1000;i++)
						{ 
							if(GPIO_ReadInputDataBit(PTA,14))
								{
									y++;
								}
						}
					if(y>999)
						{
								PIT_Stop(PIT1);
								limity=1;
								pulscnty=0;
								flagry=0;
						}
					else 
						limity=0;
				}
 
		}
			
 }
 
