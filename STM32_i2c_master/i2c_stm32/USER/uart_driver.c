#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"

int uart_baud_setting(int baud)
{
	uart_init(baud);
//	uart_init(115200);	 //串口初始化为115200
	return 0;
}


int uart_print_receive()
{
	u16 t;  
	u16 len;	
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
			break;
		}
		else
		{
			continue;
		}
	}
	return 0;
}



