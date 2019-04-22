#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

int led_light ()
{
	int i=0;
	delay_init();	    //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	for(i=0;i<10;i++)
	{
		LED0=0;
		LED1=1;
		delay_ms(1000);	 //延时1000ms
		LED0=1;
		LED1=0;
		delay_ms(1000);	//延时1000ms
	}
	return 0;
}

 /**
 *****************下面注视的代码是通过调用库函数来实现IO控制的方法*****************************************
int led_light(void)
{ 
 
	delay_init();		  //初始化延时函数
	LED_Init();		        //初始化LED端口
	while(1)
	{
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);  //LED0对应引脚GPIOB.5拉低，亮  等同LED0=0;
			GPIO_SetBits(GPIOE,GPIO_Pin_5);   //LED1对应引脚GPIOE.5拉高，灭 等同LED1=1;
			delay_ms(300);  		   //延时300ms
			GPIO_SetBits(GPIOB,GPIO_Pin_5);	   //LED0对应引脚GPIOB.5拉高，灭  等同LED0=1;
			GPIO_ResetBits(GPIOE,GPIO_Pin_5); //LED1对应引脚GPIOE.5拉低，亮 等同LED1=0;
			delay_ms(300);                     //延时300ms
	}
} 
 
 ****************************************************************************************************
 ***/
 

	
/**
*******************下面注释掉的代码是通过 直接操作寄存器 方式实现IO口控制**************************************
int led_light(void)
{ 
 
	delay_init();		  //初始化延时函数
	LED_Init();		        //初始化LED端口
	while(1)
	{
     GPIOB->BRR=GPIO_Pin_5;//LED0亮
	   GPIOE->BSRR=GPIO_Pin_5;//LED1灭
		 delay_ms(300);
     GPIOB->BSRR=GPIO_Pin_5;//LED0灭
	   GPIOE->BRR=GPIO_Pin_5;//LED1亮
		 delay_ms(300);

	 }
 }
**************************************************************************************************
**/