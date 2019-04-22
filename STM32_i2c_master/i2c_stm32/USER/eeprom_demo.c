#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "usmart.h"	 
#include "24cxx.h"	 

/************************************************
 ALIENTEK精英STM32开发板实验22
 IIC 实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

			 	
//要写入到24c02的字符串数组
const u8 TEXT_Buffer[]={"Elite STM32 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)	
	

 int i2c_eeprom_demo(void)
 {
	
	u8 key;
	u16 i=0;
	u8 datatemp[SIZE];
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	KEY_Init();				//按键初始化		 	 	
	AT24CXX_Init();			//IIC初始化 
 	while(AT24CXX_Check())//检测不到24c02
	{
		printf("CHECK ERROR I2C EEPROM\n");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	} 
	printf("CHECK I2C EEPROM OK\n");
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY1_PRES)//KEY_1按下,写入24C02
		{
			AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE);
			printf("24C02 Write Finished!\n");//提示传送完成
		}
		if(key==KEY0_PRES)//KEY0按下,读取字符串并显示
		{
 			printf("Start Read 24C02....\n");
			AT24CXX_Read(0,datatemp,SIZE);
			printf("The Data Readed Is: %s\n ",datatemp);//提示传送完成
		}
		
		for(i=0;i<20;i++)
		{
			delay_ms(2);
			LED0=!LED0;//提示系统正在运行	
		}		   
	}
}
 


int i2c_byte_demo(u8 eeprom_addr,u8 data)
{
	u8 key;
	u16 i=0;
	u8 temp;
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	KEY_Init();				//按键初始化	
	
	IIC_Init();
	printf("\n\rhello world\n\r");
 	while(AT24CXX_Check())//检测不到24c02
	{
		printf("\n\rCHECK ERROR I2C EEPROM\n\r");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	} 	
	printf("\n\rCHECK I2C EEPROM OK\n\r");
	
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY1_PRES)//KEY_1按下,写入24C02
		{
			IIC_Start();  
			IIC_Send_Byte(0XA0);//divice address
			IIC_Wait_Ack();	
			IIC_Send_Byte(eeprom_addr);
			IIC_Wait_Ack();
			IIC_Send_Byte(data);
			IIC_Wait_Ack();  		    	   
			IIC_Stop();//产生一个停止条件 
			delay_ms(10);
			printf("\n\r24C02 Write Finished!\n\r");//提示传送完成
		}
		if(key==KEY0_PRES)//KEY0按下,读取字符串并显示
		{
 			printf("\n\rStart Read 24C02....\n\r");
			IIC_Start(); 
			IIC_Send_Byte(0XA0);
			IIC_Wait_Ack(); 
			IIC_Send_Byte(eeprom_addr);
			IIC_Wait_Ack();

			IIC_Start();  	 	   
			IIC_Send_Byte(0XA1);           //进入接收模式			   
			IIC_Wait_Ack();	 
			temp=IIC_Read_Byte(0);		   
			IIC_Stop();//产生一个停止条件	   
			if(temp==data)
			{
					printf("\n\rREAD OK\n\r");
			}
			else{printf("\n\rREAD ERROR\n\r");}
			printf("\n\rThe Data Readed Is: %x\n\r",temp);//提示传送完成
		}
		
		for(i=0;i<20;i++)
		{
			delay_ms(2);
			LED0=!LED0;//提示系统正在运行	
		}		   
	}
}




int i2c_self_demo(u8 eeprom_addr,u8 data)
{
	u8 key;
	u16 i=0;
	u8 temp;
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	KEY_Init();				//按键初始化	
	
	i2c_initial();
	printf("\n\rhello world\n\r");
 	while(AT24CXX_Check())//检测不到24c02
	{
		printf("\n\rCHECK ERROR I2C EEPROM\n\r");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	} 	
	printf("\n\rCHECK I2C EEPROM OK\n\r");
	
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY1_PRES)//KEY_1按下,写入24C02
		{
			start_bit();  
			send_byte(0XA0);//divice address
			detect_ack();	
			send_byte(eeprom_addr);
			detect_ack();
			send_byte(data);
			detect_ack();  		    	   
			stop_bit();//产生一个停止条件 
			delay_ms(10);
			printf("\n\r24C02 Write Finished!\n\r");//提示传送完成
		}
		if(key==KEY0_PRES)//KEY0按下,读取字符串并显示
		{
 			printf("\n\rStart Read 24C02....\n\r");
			start_bit(); 
			send_byte(0XA0);
			detect_ack(); 
			send_byte(eeprom_addr);
			detect_ack();

			start_bit();  //change later	 
//      restart();		//both is ok	
			send_byte(0XA1);           //进入接收模式			   
			detect_ack();	 
			temp=receive_byte(0);		   
			stop_bit();//产生一个停止条件	   
			if(temp==data)
			{
					printf("\n\rREAD OK\n\r");
			}
			else{printf("\n\rREAD ERROR\n\r");}
			printf("\n\rThe Data Readed Is: %x\n\r",temp);//提示传送完成
		}
		
		for(i=0;i<20;i++)
		{
			delay_ms(2);
			LED0=!LED0;//提示系统正在运行	
		}		   
	}
}







int i2c_self_demo_2(u8 index)
{
	u8 key;
	u16 i=0;
	u8 temp;
	u8 data[2];
	u8 recv[2];
	data[0]=0x22;
	data[1]=0x33;

	
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	KEY_Init();				//按键初始化	
	
	i2c_initial();
	printf("\n\rhello world\n\r");
	while(AT24CXX_Check())//检测不到24c02
	{
		printf("\n\rCHECK ERROR I2C EEPROM\n\r");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	} 	
	printf("\n\rCHECK I2C EEPROM OK\n\r");
	
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY1_PRES)//KEY_1按下,写入24C02
		{
			i2c_write(0XA0,index,1,data);
			delay_ms(10);
			printf("\n\r24C02 Write Finished!\n\r");//提示传送完成
		}
		if(key==KEY0_PRES)//KEY0按下,读取字符串并显示
		{
 			printf("\n\rStart Read 24C02....\n\r");
/*			start_bit(); 
			send_byte(0XA0);
			detect_ack(); 
			send_byte(index);
			detect_ack();

			start_bit();  //change later	
//			restart();
			send_byte(0XA1);           //进入接收模式			   
			detect_ack();	 
			temp=receive_byte(0);		   
			stop_bit();//产生一个停止条件	   
			if(temp==data[0])
			{
					printf("\n\rREAD OK\n\r");
			}
			else{printf("\n\rREAD ERROR\n\r");}
			printf("\n\rThe Data Readed Is: %x\n\r",temp);//提示传送完成		*/	
			i2c_read(0XA0,index,1,recv);
		}
	}
	return 0;
}