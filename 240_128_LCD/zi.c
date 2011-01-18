#include<reg52.h>
#include "code.h"
#include "code1.h"
#define uchar unsigned char
#define uint unsigned int 
#define AddressSend {}
sbit ce=P2^7;
sbit rw=P3^6;
sbit rd=P3^7;
sbit cd=P2^6;
sbit rst=P2^5;
uint address;
uchar data1,data2,cmd;
void clear();
void delay(uint z)
{
	uint i,j;
	for(i=z;i>0;i--)
		for(j=110;j>0;j--);
}
void delayus(uint z)
{
	while(z--);
}
void WriteData(uchar dat)           //写数据    
{    ce=1;
     cd=0;
    rw=0;
     P0=dat; 
   delayus(12);

	ce=0;
     rw=1;
}   
void WriteCommand(uchar cmd)           //写命令    
{   ce=1;
     cd=1;
     rw=0;
     P0=cmd;
delayus(12);
	ce=0;
     rw=1;
} 
void WriteDC(uchar dat,uchar cmd)      //写一个数据和一个指令 
{   
     WriteData(dat);   
     WriteCommand(cmd);   
} 
void WriteDDC(uchar data1, uchar data2, uchar cmd)   //写两个数据和一个指令 
{   
     WriteData(data1);         
     WriteData(data2);   
     WriteCommand(cmd);     
}   
void LCD_Init(void)         //液晶初始化 
{   
     uchar data1,data2,cmd;    
   		rw=1;
        
     
     data1 = 0x00;   
     data2 = 0x10;              //图形显示占用32*128=0xfff，故文本从0x1000开始
     cmd = 0x40;                //设置文本显示区首地址    
     WriteDDC(data1,data2,cmd);   
     data1 = 0x20;              //可以整屏显示
     data2 = 0x00;   
     cmd = 0x41;                //设置文本显示区宽度    
     WriteDDC(data1,data2,cmd);   
     data1 = 0x00;   
     data2 = 0x00;   
     cmd = 0x42;                //设置图形显示区首地址    
     WriteDDC(data1,data2,cmd);   
     data1 = 0x1e;   
     data2 = 0x00;   
     cmd = 0x43;                //设置图形显示区宽度    
     WriteDDC(data1,data2,cmd); 
   cmd =0xa2;                 //设置光标形状 
   WriteCommand(cmd); 
     cmd = 0x80;                //显示方式设置
                              //启用外部字符发生器,显示方式是文本与图形与 
     WriteCommand(cmd); 
     cmd = 0x9e;                //启用光标闪烁,光标不显示,文本显示与图形显示    
     WriteCommand(cmd);   
}



void WriteOneChinese(uchar *codes)
{
	uchar i=0,j=0;
	for(i=0;i<32;i++)
	{
		WriteData(codes[i]);
		j++;
		if(j==2)
		{
			WriteCommand(0xb2);           //退出自动写模式
		    address=address+30;
		    data1 = address;                 //取地址低八位    
		    data2 = address>>8;                //取地址高八位    
		    cmd = 0x24; 
		    WriteDDC(data1,data2,cmd);
		    j=0;
		    WriteCommand(0xb0);         //进入自动写模式		
		}
	}
}
void writezi(uchar row,uchar rol)
{
	uint k,h=0,total;
	uchar t;
	total=sizeof(CG_code)/32/15-6;
	for(t=0;t<total;t++)
	{
		address = row*30+rol;
		data1 = address;                 //取地址低八位    
		data2 = address>>8;                //取地址高八位    
		cmd = 0x24; 
		WriteDDC(data1,data2,cmd);
	//	WriteCommand(0xb0);//自动写模式
		for(k=1;k<121;k++)
		{   
			h++;
			WriteCommand(0xb2); 
		    address = row*30+rol+2*(h-1);
			data1 = address;                 //取地址低八位    
			data2 = address>>8;                //取地址高八位    
			cmd = 0x24; 
			
			WriteDDC(data1,data2,cmd);
		    WriteCommand(0xb0);
			WriteOneChinese(&CG_code[((k-1)+15*t)*32]);
			if((t==total-1)&&(k==(sizeof(CG_code)/32%15)+105)) break;
			if(((t>0)&&(k>107))||(t==0))
		   		 delay(1000);
			if(k%15==0)
		    {row=row+16;h=0;}
		}
		row=0;rol=0;
		if(t<total-1)
		clear();
	}
}
void WriteImage(uchar row,uchar rol,uchar hight,uchar width)
{
	uint i;
	address=row*30+rol;
	data1=address;
	data2=address>>8;
	WriteDDC(data1,data2,0x24);
	WriteCommand(0xb0);
	for(i=0;i<(hight*width)/8;i++)
	{
		WriteData(Image_code[i]);
		delay(10);
	}
	WriteCommand(0xb2);
}
void clear()
{
	uint i;
	WriteCommand(0xb2);
	data1 = 0x00;                 //取地址低八位    
	data2 = 0x00;                //取地址高八位    
	cmd = 0x24; 
	WriteDDC(data1,data2,cmd);
	WriteCommand(0xb0);//自动写模式
	for(i=0;i<5000;i++)
	{
		WriteData(0x00);
	}
		WriteCommand(0xb2);
}
void main()   
{   
	rst=0;
	delay(5);
	rst=1;
	rd=1;  
	LCD_Init();
	clear();
	WriteImage(0,0,240,128);
	delay(10000);
	clear();
	writezi(0,0);
	while(1);
}

