#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int 
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
//void delayus(uint z)
//{
//	while(z--);
//}
void writedata(uchar dat)           //写数据    
{    ce=1;
     cd=0;
    rw=0;
     P0=dat; 
 //  delayus(10);

	ce=0;
     rw=1;
}   
void writecommand(uchar cmd)           //写命令    
{   ce=1;
     cd=1;
     rw=0;
     P0=cmd;
//delayus(10);
	ce=0;
     rw=1;
} 
/*void writedc(uchar dat,uchar cmd)      //写一个数据和一个指令 
{   
     writedata(dat);   
     writecommand(cmd);   
} */
void writeddc(uchar data1, uchar data2, uchar cmd)   //写两个数据和一个指令 
{   
     writedata(data1);         
     writedata(data2);   
     writecommand(cmd);     
}   
void LCD_Init(void)         //液晶初始化 
{   
     uchar data1,data2,cmd;    
   		rw=1;
		rst=0;
	delay(5);
	rst=1;
	rd=1;  
        
     
     data1 = 0x00;   
     data2 = 0x10;              //图形显示占用32*128=0xfff，故文本从0x1000开始
     cmd = 0x40;                //设置文本显示区首地址    
     writeddc(data1,data2,cmd);   
     data1 = 0x20;              //可以整屏显示
     data2 = 0x00;   
     cmd = 0x41;                //设置文本显示区宽度    
     writeddc(data1,data2,cmd);   
     data1 = 0x00;   
     data2 = 0x00;   
     cmd = 0x42;                //设置图形显示区首地址    
     writeddc(data1,data2,cmd);   
     data1 = 0x1e;   
     data2 = 0x00;   
     cmd = 0x43;                //设置图形显示区宽度    
     writeddc(data1,data2,cmd); 
   cmd =0xa2;                 //设置光标形状 
   writecommand(cmd); 
     cmd = 0x80;                //显示方式设置
                              //启用外部字符发生器,显示方式是文本与图形与 
     writecommand(cmd); 
     cmd = 0x9e;                //启用光标闪烁,光标不显示,文本显示与图形显示    
     writecommand(cmd);   
}



/*void writeonechinese(uchar *codes)
{
	uchar i=0,j=0;
	for(i=0;i<32;i++)
	{
		writedata(codes[i]);
		j++;
		if(j==2)
		{
			writecommand(0xb2);           //退出自动写模式
		    address=address+30;
		    data1 = address;                 //取地址低八位    
		    data2 = address>>8;                //取地址高八位    
		    cmd = 0x24; 
		    writeddc(data1,data2,cmd);
		    j=0;
		    writecommand(0xb0);         //进入自动写模式		
		}
	}
}*/
void clearlcd()
{
	uint i;
	writecommand(0xb2);
	data1 = 0x00;                 //取地址低八位    
	data2 = 0x00;                //取地址高八位    
	cmd = 0x24; 
	writeddc(data1,data2,cmd);
	writecommand(0xb0);//自动写模式
	for(i=0;i<5000;i++)
	{
		writedata(0x00);
	}
		writecommand(0xb2);
}
