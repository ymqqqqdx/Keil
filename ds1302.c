#include<reg52.h>
#include<intrins.h>
#include<1602.h>
#include<ds_1302.h>
#include<24c02.h>
#include<ds18b20p.h>
sbit k1=P1^0;
sbit k2=P1^1;
sbit k3=P1^2;
sbit k4=P1^3;
sbit beep=P3^0;
unsigned char year,month,day,week,hour,minute,second,alarmflag,flag=0,flag2=0,min2=0,sec2=0,secp=0,num=0,max,i,bb,ahour=0,amin=0,asec=0;
unsigned char mins[20]=0,secs[20]=0,secps[20]=0;
void display1();
void display2();
void init()
{
	write_com(0x01);
	year=readyear+2000;
	month=readmonth;
	day=readday;
	hour=readhour;
	minute=readminute;
	second=readsecond;
	week=readweek;
	display1();
	TMOD=0x01;
	TH0=(65536-5000)/256; 
	TL0=(65536-5000)%256;
	EA=1; //开总中断
	ahour=read_add(1);
	amin=read_add(2);
}
void init2()
{
	write_com(0x01);
	write_com(0x80+0x40+0x03); //显示number
	for(i=0;i<6;i++)
	write_data(table2[i]);
	write_com(0x80+0x06);
	write_data(table1[0]);
	write_com(0x80+0x09);
	write_data(table1[1]);
}
void init3()
{
	write_com(0x01);
	write_com(0x80+0x42);
	for(i=0;i<13;i++)
		write_data(table5[i]);
	displaytwo(6,ahour);
	write_data(table1[0]);
	displaytwo(9,amin);
	write_com(0x80+0x06);
}
void keyscan()
{
	if(k1==0&&flag2!=0)
	{
		delay(5);
		if(k1==0&&flag2!=0)
		{
			flag=0;  flag2=0; write_com(0x0c); init(); if(num!=0&&TR0==0) ET0=0;
		}
		while(k1==0);
	} 
	if(k1==0)
	{
		delay(5);
		if(k1==0)
		{
			flag++;
			if(flag==1)
			{
				init2();
			}
			switch(flag)
			{
				case 6 : write_com(0x80+0x40+0x04); break;
				case 7 : write_com(0x80+0x40+0x07); break;
				case 8 : write_com(0x80+0x40+0x0a); break;
				case 2 : init(); write_com(0x80+0x01); write_com(0x0f); break;
				case 3 : write_com(0x80+0x06); break;
				case 4 : write_com(0x80+0x09); break;
				case 5 : write_com(0x80+0x0d); break;
				case 9 : init3(); break;
				case 10: write_com(0x80+0x09); break;
				case 11: flag=0; write_com(0x01); write_com(0x0c); break;
			}
		}
		while(k1==0);
	}
	if(k2==0)
	{
		delay(5);
		if(k2==0)
		{
			if(flag==1)
			{
				if(ET0==0)
				{
					min2=0;sec2=0;num=0;secp=0;
					for(i=0;i<max;i++)
					{
						mins[i]=0;secs[i]=0;secps[i]=0;
					}
					max=0;ET0=1;flag2++;
					while(k2==0);
				}
				if(TR0==1)
				{
					num++;
					mins[num]=min2;
					secs[num]=sec2;
					secps[num]=secp;
					TR0=0;
					ET0=0;
					max=num;
					flag2++;
					while(k2==0);
				}
			}
			switch(flag)
			{
				case 6 : if(hour<23) hour++; else hour=0; writehour; delay(5); displayhour; write_com(0x80+0x40+0x04); break;
				case 7 : if(minute<59) minute++; else minute=0;writeminute; delay(5); displayminute;  write_com(0x80+0x40+0x07); break;
				case 8 : if(second<59) second++; else second=0;writesecond; delay(5); displaysecond;  write_com(0x80+0x40+0x0a); break;
				case 2 : year++;  writeyear; delay(5); displayyear;write_com(0x80+0x01);  break;
				case 3 : if(month<12) month++; else month=1;writemonth;delay(5); displaymonth; write_com(0x80+0x06);  break;
				case 4 : if(day<31) day++; else day=1; writeday; delay(5); displayday; write_com(0x80+0x09); break;
				case 5 : if(week<8) week++; else week=2; writeweek;delay(5); displayweek; write_com(0x80+0x0d); break;
				case 9 : if(ahour<23) ahour++; else ahour=0; displaytwo(6,ahour); write_add(1,ahour); write_com(0x80+0x06); break;
				case 10: if(amin<59) amin++; else amin=0; displaytwo(9,amin); write_add(2,amin); write_com(0x80+0x09); break;
			}
		}
		while(k2==0);
	}
	if(k3==0&flag!=0)
	{
		delay(5);
		if(k3==0&flag!=0)
		{
			if(flag==1)
			{
				if(ET0==1&&TR0==1)
				{
					num++;
					flag2++;
					mins[num]=min2;
					secs[num]=sec2;
					secps[num]=secp;
					while(k3==0)
					{
						display2();
					}
				}
				if(ET0==1&&TR0==0)
					{
						TR0=1;
						flag2++;
					}
				if(ET0==0)
				{
					if(num>1)
					num--;
					min2=mins[num];
					sec2=secs[num];
					secp=secps[num];
					flag2++;
				}
			}
			switch(flag)
			{
				case 6 : if(hour>0) hour--; else hour=23; writehour;delay(5); displayhour; write_com(0x80+0x40+0x04); break;
				case 7 : if(minute>0) minute--; else minute=59; writeminute; delay(5); displayminute; write_com(0x80+0x40+0x07); break;
				case 8 : if(second>0) second--; else second=59; writesecond; delay(5); displaysecond; write_com(0x80+0x40+0x0a); break;
				case 2 : year--; writeyear; delay(5);  displayyear; write_com(0x80+0x01); break;
				case 3 : if(month>1) month--; else month=12; writemonth; delay(5); displaymonth;  write_com(0x80+0x06); break;
				case 4 : if(day>1) day--; else day=31;writeday; delay(5); displayday;  write_com(0x80+0x09); break;
				case 5 : if(week>1) week--; else week=7;writeweek; delay(5); displayweek;  write_com(0x80+0x0d); break;
				case 9 : if(ahour>0) ahour--; else ahour=23; displaytwo(6,ahour); write_add(1,ahour); write_com(0x80+0x06); break;
				case 10: if(amin>0) amin--; else amin=59; displaytwo(9,amin); write_add(2,amin); write_com(0x80+0x09); break;
			}
		}
		while(k3==0);		
	}
	if(k4==0)
	{
		delay(5);
		if(k4==0&&ET1==0&&flag!=0)
		{
			if(num<max)
			num++;
			min2=mins[num];
			sec2=secs[num];
			secp=secps[num];
			flag2++;
		}
		if(k4==0&&flag==0)
		{
			write_com(0x01);
			displaytemp(readtemp());		  
		}
	}
	while(k4==0);
}
void display1()
{
	write_com(0x80+5);
	write_data(table1[2]);
	write_com(0x80+8);
	write_data(table1[2]);
	write_com(0x80+0x40+6);
	write_data(table1[0]);
	write_com(0x80+0x40+9);
	write_data(table1[0]);
	second=readsecond;
	minute=readminute;
	second=readsecond;
	displayyear;
	displaymonth;
	displayday;
	displayweek;
	displayhour;
	displayminute;
	displaysecond;
}
void display2()
{
	displaytwo(10,secp);
	displaytwo(0x40+0x0a,num);
	displaytwo(7,sec2);
	displaytwo(4,min2);
}
void display3()
{
	write_com(0x01);
	displaytwo(6,ahour);
	write_data(table1[0]);
	displaytwo(9,amin);
	write_com(0x80+0x43);
	for(i=0;i<11;i++)
		write_data(table4[i]);
}
void display4()
{
		write_com(0x01);
		write_com(0x80+0x46);
		for(i=0;i<5;i++)
			write_data(table5[i]);
		displaytwo(6,ahour);
		write_data(table1[0]);
		displaytwo(9,amin);
		delay(3000);
		write_com(0x01);
}
void main()
{
	init_1602();
	init_24c02();
	init();
	readtemp();
	EA=1;
	ET0=1;
	TR0=0;
	while(1)
	{
		keyscan();
		if(second==0)
		{
			if(ahour==hour&&amin==minute)
			{
				display3();
				alarmflag=1;
				beep=0;
				while(alarmflag==1)
				{
					if(k1==0||k2==0||k3==0||k4==0)
					{
						alarmflag=0;
						write_com(0x01);
						beep=1;
						while(k1==0||k2==0||k3==0||k4==0);
					}
				}
			}
		}
		if(flag==0&&k3==0)
		{
			display4();
		}
		if(flag==0&&alarmflag==0)
		{
			display1();
		}
		if(flag==1&&alarmflag==0)
		{
			display2();
		}
	}
} 
void timer1() interrupt 1
{
	TH0=(65536-5000)/256;
   TL0=(65536-5000)%256;
	bb++;
	if(bb==2)
	{   
		bb=0;
		secp++;
		if(secp==100)
		{
			sec2++;
			secp=0;
			if(sec2==60)
			{
				min2++;
				sec2=0;
			}
		}
	}		
}
