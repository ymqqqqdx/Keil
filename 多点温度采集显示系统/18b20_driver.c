#include <reg52.h>
sbit DQ0=P3^0;//数据线0
sbit DQ1=P3^1;
sbit DQ2=P3^2;
sbit DQ3=P3^3;
sbit DQ4=P3^4;
sbit DQ5=P3^5;
sbit DQ6=P3^6;
sbit DQ7=P3^7;
extern bit symbol;//main.c中的symbol
//微秒延时
void delayus(unsigned int i)
{
	 while(i--);
}
//对每个数据线分开操作
void DQoperate(unsigned char num,bit buff)
{
	switch(num)
	{
		case 0: if(buff==0) DQ0=0; else DQ0=1; break;
		case 1: if(buff==0) DQ1=0; else DQ1=1; break;
		case 2: if(buff==0) DQ2=0; else DQ2=1; break;
		case 3: if(buff==0) DQ3=0; else DQ3=1; break;
		case 4: if(buff==0) DQ4=0; else DQ4=1; break;
		case 5: if(buff==0) DQ5=0; else DQ5=1; break;
		case 6: if(buff==0) DQ6=0; else DQ6=1; break;
		case 7: if(buff==0) DQ7=0; else DQ7=1; break;
	}
}
//读数据线
unsigned char readDQ(unsigned char num)
{
	bit buff;
	switch(num)
	{
		case 0: buff=DQ0; break;
		case 1: buff=DQ1; break;
		case 2: buff=DQ2; break;
		case 3: buff=DQ3; break;
		case 4: buff=DQ4; break;
		case 5: buff=DQ5; break;
		case 6: buff=DQ6; break;
		case 7: buff=DQ7; break;
	}
	return buff;
}
//18b20初始化函数
init_ds18b20(unsigned char num)
{
	 unsigned char x=0;
	 DQoperate(num,1);
	 delayus(8);  //稍做延时
	 DQoperate(num,0);
	 delayus(80); //精确延时 大于 480us
	 DQoperate(num,1);
	 delayus(14);
	 x=readDQ(num);      //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
	 delayus(20);
}
//读一个字节
unsigned char readonechar(unsigned char num)
{
	unsigned char i=0;
	unsigned char dat=0;
	for (i=8;i>0;i--)
	{
		DQoperate(num,0);
		dat>>=1;
		DQoperate(num,1);
		if(readDQ(num))
		dat|=0x80;
		delayus(4);
	}
return(dat);
}
//写一个字节
void writeonechar(unsigned char num,unsigned char dat)
{	
	unsigned char i=0;
	for (i=8; i>0; i--)
	{
		DQoperate(num,0);
		DQoperate(num,dat&0x01);
		delayus(5);
		DQoperate(num,1);
		dat>>=1;
	}
delayus(4);
}
//读取温度
unsigned int readtemp(unsigned char num)
{
	unsigned char a=0;
	unsigned char b=0;
	unsigned int t=0;
	float tt=0;
	init_ds18b20(num);
	writeonechar(num,0xcc); // 跳过读序号列号的操作
	writeonechar(num,0x44); // 启动温度转换
	init_ds18b20(num);
	writeonechar(num,0xcc); //跳过读序号列号的操作
	writeonechar(num,0xbe); //读取温度寄存器等两个就是温度
	a=readonechar(num);
	b=readonechar(num);
	t=b;
	if(t<8)	//若最高位为0，则为正数
	{
		symbol=0;
		t<<=8;
		t|=a;
	}
	else
	{
		symbol=1;
		t<<=8;
		t|=a;
		t=~t;//取反+1取相反数
		t+=1;
	}	
	tt=t*0.0625;
	t= tt*10+0.5;
	return(t);
}