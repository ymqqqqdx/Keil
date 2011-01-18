#include <reg52.h>
#include <stdio.h>
unsigned char *p1,*p2,*p3,*p4,*p5;
unsigned char k;
unsigned char high=0,low=0,con=0,total=1,flag=0,total;
unsigned int count1=0;
unsigned char decode[5][16];
void writetwochar(unsigned char,unsigned char,unsigned char,unsigned char,unsigned char);
void delay(int);
unsigned char write_add(unsigned char,unsigned char,unsigned char);
unsigned char read_add(unsigned char,unsigned char);
void init_24c02();
void comm_init();
void main()
{
	unsigned char i,j;
	unsigned int counter=0,add=0;
	init_24c02();
	comm_init();
	total=read_add(7,1);
	for(i=0;i<5;i++)
	{
		for(j=0;j<16;j++)
		{
			decode[i][j]=read_add(counter/256,counter%256);
			counter++;
		}
	}
	p1=&decode[0][0];	p2=&decode[1][0];	p3=&decode[2][0]; p4=&decode[3][0]; p5=&decode[4][0];
	while(1)
	{
		for(k=0;k<8;k++)
		{
			for(j=0;j<2;j++)
			{
				for(i=0;i<16;i++)
				{
					P2=i;
					writetwochar(*(p5+i),*(p4+i),*(p3+i),*(p2+i),*(p1+i));
					delay(10);
				}
			}
		}
		for(i=0;i<16;i++)
		{
			*(p1+i)=*(p2+i);
			*(p2+i)=*(p3+i);
			*(p3+i)=*(p4+i);
			*(p4+i)=*(p5+i);
		}
		if(counter==32*total)
		counter=0;
		for(i=0;i<16;i++)
		{
			decode[4][i]=read_add(counter/256,counter%256);
			counter++;
		}
	}
}
unsigned char convert(unsigned char re)
{
	if(re>='0'&&re<='9')
		re=re-48;
	else if(re>='A'&&re<='F')
		re=re-55;
		return re;
}
void exter0() interrupt 0
{
	ES=1;
	total=1;
	con=0;
	count1=0;
	flag=0;
	while(count1!=32*total);
	ES=0;
}
void ser() interrupt 4
{
	if(RI==0) return;
	if(flag==0)
	{
		if(con==0)
		{
			high=SBUF;
			high=convert(high);
			high<<=4;
			con=1;
			SBUF='K';
			while(!TI);
			TI=0;
		}
		else if(con==1)
		{
			low=SBUF;
			low=convert(low);
			con=0;
			total=high+low;
			flag=1;
			SBUF='K';
			while(!TI);
			TI=0;
			total=high+low;
			write_add(7,1,high+low);
		}
	}
	else if(flag==1)
	{
		if(con==0)
		{
			high=SBUF;
			high=convert(high);
			high<<=4;
			con=1;
			SBUF='O';
			while(!TI);
			TI=0;
		}
		else if(con==1)
		{
			low=SBUF;
			low=convert(low);
			con=0;
			if(write_add(count1/256,count1%256,(high+low))==1)
			{
				SBUF='O';
				while(!TI);
				TI=0;
				count1++;
			}
		}
	}
	RI=0;
}
