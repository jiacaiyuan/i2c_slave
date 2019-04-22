#include "myiic.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"

void i2c_initial()
{
	IIC_Init();
	printf("\n\rI2C INIT\n\r");
}



void start_bit()
{
	IIC_Start();
	printf("\n\rI2C START\n\r");
}

void stop_bit()
{
	IIC_Stop();
	printf("\n\rI2C STOP\n\r");
}


void send_byte(u8 txd)
{
	IIC_Send_Byte(txd);
	printf("\n\rSEND BYTE %x\n\r",txd);
}

int detect_ack()
{
	int ack;
	ack=IIC_Wait_Ack();
	if(ack==0){printf("\n\rDETECT ACK\n\r");}
	else      {printf("\n\rERROR ACK\n\r");}
	return ack;
}




void restart()
{
	SDA_OUT();
	IIC_SCL=0;
	delay_us(2);
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_Start();
	printf("\n\rI2C RESTART\n\r");
}



u8  receive_byte(unsigned char ack)//ack=1 has ACK;
{
	u8 rxd;
	rxd=IIC_Read_Byte(ack);
	printf("\n\rRECEIVE BYTE %x\n\r",rxd);
	return rxd;
}

void gen_scl(int cycles)
{
	int i=0;
	for(i=0;i<cycles;i++)
	{
		delay_us(2);
		IIC_SCL=1;
		delay_us(2);
		IIC_SCL=0;
	}
	delay_us(2);
	IIC_SCL=1;
	printf("\n\rMASTER GEN %d SCL CYCLES\n\r",cycles);
}




void gen_ack()
{
	IIC_Ack();
	printf("\n\rMASTER GEN ACK\n\r");
}


void gen_noack()
{
	IIC_NAck();
	printf("\n\rMASTER NO ACK\n\r");
}


void i2c_write(u8 addr,u8 index,int N, u8* data)
{
	int i=0;
	u8 cmd=(addr|0x0)&0xff;
	start_bit();
	send_byte(cmd);
	detect_ack();
	send_byte(index);
	detect_ack();
	for(i=0;i<N;i++)
	{
		send_byte(*(data+i));
		detect_ack();
	}
	stop_bit();
	gen_scl(3);
	printf("\n\rwrite Bytes=%d to Addr=%x\n\r",N,addr);
	for(i=0;i<N;i++)
	{
		printf("\n\rindex=%d,data=%x\n\r",index+i,*(data+i));
	}
}

void i2c_read(u8 addr,u8 index,int N,u8* data)
{
	int i=0;
	u8 cmd=(addr|0x0)&0xff;
	start_bit();
	send_byte(cmd);
	detect_ack();
	send_byte(index);
	detect_ack();
	
	restart();
	cmd=(addr|0x1)&0xff;
	send_byte(cmd);
	detect_ack();
	for(i=0;i<N;i++)
	{
		if(i==N-1)
		{
			*(data+i)=receive_byte(0);//don't gen ack
			gen_scl(3);
			stop_bit();
		}
		else
		{
			*(data+i)=receive_byte(1);// gen ack
		}
	}
	printf("\n\rread Bytes=%d to Addr=%x\n\r",N,addr);
	for(i=0;i<N;i++)
	{
		printf("\n\rindex=%d,data=%x\n\r",index+i,*(data+i));
	}

}


