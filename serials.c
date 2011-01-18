#include <reg52.h>
//#include <AT89X52.H>
#include <mscomm.h>
sbit k1=P1^4;
sbit k2=P1^5;
sbit k3=P1^6;
sbit k4=P1^7;
void main()
{
	init_comm();
	while(1)
	{
	if(flag_comm==1)
//		{
			ES=0;
		
			if(k1==0)
			{ 
			   P2=0x02;
				P0=0x01;
				SBUF='1';while(!TI);TI=0;
				while(k1==0);
			}
		/*	if(k2==0)
			{
				SBUF='2';while(!TI);TI=0;
			}
			if(k3==0)
			{
				SBUF='3';while(!TI);TI=0;
			}
			if(k4==0)
			{
				SBUF='4';while(!TI);TI=0;
			}
//			SBUF=buffer;*/
	
			
			ES=1;
		}
//	}
}


