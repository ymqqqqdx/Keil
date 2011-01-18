#include <reg52.h>
#define CMD 0
#define DATA 1
sbit rtc_clk=P1^6;
sbit rtc_io=P1^7;
sbit rtc_rst=P3^0;

sbit lcd_cs=P0^3;
sbit lcd_clk=P1^2;
sbit lcd_sid=P0^2;

sbit key1=P3^4;
sbit key2=P3^5;
sbit key3=P3^2;

U8 stat=0,key=0;
U8 second_old;

U8 code range[3][2]={{0,59},{0,59},{0,23}};

struct RTC
{
	U8 second;
	U8 minute;
	U8 hour;
} time;

void delay_1ms(U16 z)
{
	U8 i;
	while(z--)
		for(i=0;i<110;i++);
}

void rtc_writeonechar(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat>>=1;
		rtc_clk=0;
		rtc_io=CY;
		rtc_clk=1;
	}
}

U8 rtc_readonechar()
{
	U8 i,temp=0;
	for(i=0;i<8;i++)
	{
		rtc_clk=0;
		temp|=((U8)rtc_io)<<i;
		rtc_clk=1;
	}
	return temp/16*10+temp%16;
}

void rtc_write(U8 add,U8 dat)
{
	rtc_rst=0;
	rtc_clk=0;
	rtc_rst=1;
	rtc_writeonechar(add);
	rtc_writeonechar((dat/10)<<4|dat%10);
	rtc_rst=0;
}

U8 rtc_read(U8 add)
{
	U8 temp;
	rtc_rst=0;
	rtc_clk=0;
	rtc_rst=1;
	rtc_writeonechar(add|1);
	temp=rtc_readonechar();
	rtc_rst=0;
	return temp;
}

void lcd_writeonechar(U8 dat)
{
	U8 i;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		lcd_sid=CY;
		lcd_clk=0;
		lcd_clk=1;
	}
}

void lcd_writecd(bit cd,U8 dat)
{
	lcd_cs=1;
	lcd_writeonechar(cd==0?0xf8:0xfa);
	lcd_writeonechar(dat&0xf0);
	lcd_writeonechar(dat<<4);
	lcd_cs=0;
}

void lcd_string(U8 add,U8 *pt)
{
	lcd_writecd(0,add);
	while(*pt!='\0')
		lcd_writecd(1,*pt++);
}

void lcd_normal_init()
{
	lcd_writecd(0,0x0c);
	lcd_string(0x82,"当前时间：");
	lcd_writecd(0,0x92);
	lcd_writecd(1,':');
	lcd_writecd(0,0x94);
	lcd_writecd(1,':');
}

void lcd_adjust_init()
{
	lcd_writecd(0,0x0f);
	lcd_string(0x82,"调时模式：");	
	lcd_writecd(0,0x92);
	lcd_writecd(1,':');
	lcd_writecd(0,0x94);
	lcd_writecd(1,':');	
	lcd_writecd(0,0x95);	
}

void lcd_displaytwo(U8 add,U8 dat)
{
	lcd_writecd(0,add);
	lcd_writecd(1,dat/10+'0');
	lcd_writecd(1,dat%10+'0');
	lcd_writecd(0,add);
}

void rtc_gettime()
{
	U8 i,*pt=&time.second;
	for(i=0;i<3;i++)
		*pt++=rtc_read(0x80+i*2);
	lcd_displaytwo(0x91,time.hour);
	lcd_displaytwo(0x93,time.minute);
	lcd_displaytwo(0x95,time.second);
}

void lcd_normal()
{
	time.second=rtc_read(0x80);
	if(time.second!=second_old)
	{
		lcd_displaytwo(0x95,time.second);
		if(time.second==0)
			rtc_gettime();
		second_old=time.second;
	}
}

U8 rtc_range(U8 offset,U8 dat)
{
	if(dat<range[offset][0] | dat>range[offset][1])
		return range[offset][0];
	else
		return dat;
}

void keyscan()
{
	U8 *pt,i;
	if(!key1)
	{
		key++;
		if(key==1)
		{
			lcd_adjust_init();
			stat=1;
		}
		lcd_writecd(0,0x95-(key-1)*2);
		if(key==4)
		{
			lcd_normal_init();
			key=0;
			stat=0;
			pt=&time.second;
			for(i=0;i<3;i++)
				rtc_write(0x80+i*2,*(pt+i));
		}
		while(!key1);
	}
	else if(!key2)
	{
		pt=&time.second;
		(*(pt+key-1))++;
		*(pt+key-1)=rtc_range(key-1,*(pt+key-1));
		lcd_displaytwo(0x95-(key-1)*2,*(pt+key-1));
		while(!key2);
	}
	else if(!key3)
	{
		pt=&time.second;
		(*(pt+key-1))--;
		*(pt+key-1)=rtc_range(key-1,*(pt+key-1));
		lcd_displaytwo(0x95-(key-1)*2,*(pt+key-1));
	}while(!key3);
}

void main()
{
	rtc_write(0x8e,0);
	lcd_normal_init();
	rtc_gettime();
	while(1)
	{
		switch(stat)
		{
			case 0 : lcd_normal(); break;
			case 1 : break;
		}
		keyscan();
		delay_1ms(5);
	}
}