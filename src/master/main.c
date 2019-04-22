#include "sys.h"
#include "24cxx.h"
#include "led.h"
#include "myiic.h"
#include "usart.h"
#include "key.h"
#include "delay.h"



int main()
{	
	
//--------------------------------------------------------------
//----------------------FOR DEBUG I2C MASTER--------------------
//--------------------------------------------------------------
//	i2c_eeprom_demo();
//	i2c_byte_demo(0x0,0x5a);
//	i2c_self_demo(0x1,0xa5);
//	i2c_self_demo_2(0x1);
	
//--------------------------------------------------------------
//-----------------------FOR DEBUG I2C SLAVE--------------------
//--------------------------------------------------------------
	u8 device_addr=0x55;//7'h55 define in the I2C slave verilog
	u8 slave_addr=(device_addr<<1)&0xff;
	u8 index=1;
	u8 data[2];
	u8 key;
	u8 recv[2];
	u16 i=0;
	data[0]=0x5a;
	data[1]=0xa5;
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	KEY_Init();				//按键初始化	
	
	printf("\n\rslave addr=%x\n\r",slave_addr);
	i2c_initial();
	printf("\n\rhello world\n\r");
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY1_PRES)//KEY_1按下,写入24C02
		{
			printf("\n\r----------------------------------------------\n\r");
			printf("\n\SLAVE REG WRITE\n\r");//提示传送完成
			i2c_write(slave_addr,index,2,data);
			delay_ms(10);
			printf("\n\r----------------------------------------------\n\r");
		}
		if(key==KEY0_PRES)//KEY0按下,读取字符串并显示
		{
			printf("\n\r----------------------------------------------\n\r");
			printf("\n\rSLAVE REG READ\n\r");	
			i2c_read(slave_addr,index,2,recv);
			delay_ms(10);
			printf("\n\r----------------------------------------------\n\r");
		}
	}
	return 0;
}		
	