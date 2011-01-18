#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
uchar time1,time2,time3,t1bak,t2bak,t3bak;
uchar count=0;
uchar buff=0;
void init7219();
void write7219(uchar,uchar);
void showtime(uchar,uchar);
void init()
{
	t1bak=t2bak=15;t3bak=2;//默认双向通行时间均为15S;黄灯警示时间为2S;
	time1=t1bak; time2=t2bak; time3=t3bak;
	TMOD=0x01;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	EA=1;
	ET0=1;
	TR0=1;
	init7219();
}
void main()
{
//	init();
	init7219();
	showtime(1,234);
	while(1);
}
void showtime(uchar dat1,uchar dat2)
{
	write7219(1,dat1>99?dat1/100:0x0f);
	write7219(2,dat1%100/10);
	write7219(3,dat1%10);
	write7219(5,dat2>99?dat2/100:0x0f);
	write7219(6,dat2%100/10);
	write7219(7,dat2%10);
}
void timer0() interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	count++;
	if(count==20)
	{
		count=0;
		switch(buff)
		{
			case 0:
			{
				P1=0x14;
				showtime(time1,time1+time3);
				time1--;
				if(!time1)
				{
					time1=t1bak;
					buff++;
				}
			}	break;
			case 1:
			{
				P1=0x12;
				showtime(time3,time3);
				time3--;
				if(!time3)
				{
					time3=t3bak;
					buff++;
				}
			}	break;
			case 2:
			{
				P1=0x41;
				showtime(time2+time3,time2);
				time2--;
				if(!time2)
				{
					time2=t2bak;
					buff++;
				}
			}	break;
			case 3:
			{
				P1=0x21;
				showtime(time3,time3);
				time3--;
				if(!time3)
				{
					time3=t3bak;
					buff=0;
				}
			}	break;
		}		
	}
}