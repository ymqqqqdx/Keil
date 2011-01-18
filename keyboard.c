#include <reg52.h>
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
unsigned char k,temp;
void delay(int z)
{
	int i,j;
        for(i=z;i>0;i--)
         for(j=110;j>0;j--);
}
void display(unsigned char dd)
{
	P2=0x08;
	P0=table[dd/10];
	delay(3);
	P2=0x04;
	P0=table[dd%10];
	delay(3);
}
void keyscan()
{
	P1=0xfe;
	temp=P1;
	temp=temp&0xf0;
	while(temp!=0xf0)
	{
		delay(5);
		temp=P1;
		temp=temp&0xf0;
		while(temp!=0xf0)
		{
			switch(temp)
			{
				case 0x70:k=0;
				     break;
				case 0xb0:k=1;
				     break;
				case 0xd0:k=2;
				     break;
				case 0xe0:k=3;
				     break;
			}	       
			P0=table[k];
			temp=P1;
			temp=temp&0xf0;
		}
	}
   P1=0xfd;
	temp=P1;
	temp=temp&0xf0;
	while(temp!=0xf0)
	{
		delay(5);
		temp=P3;
		temp=temp&0xf0;
		while(temp!=0xf0)
		{
			switch(temp)
			{
				case 0x70:k=4;
				     break;
				case 0xb0:k=5;
				     break;
				case 0xd0:k=6;
				     break;
				case 0xe0:k=7;
				     break;
			}
			P0=table[k];
			temp=P1;
			temp=temp&0xf0;
		}
	}
   P1=0xf7;
	temp=P1;
	temp=temp&0xf0;
	delay(5);
	while(temp!=0xf0)
	{
		delay(5);
		temp=P1;
		temp=temp&0xf0;
		while(temp!=0xf0)
		{
			switch(temp)
			{
				case 0x70:k=12;
				     break;
				case 0xb0:k=13;
				     break;
				case 0xd0:k=14;
				     break;

				case 0xe0:k=15;
				     break;
			}
		
			P0=table[k];
		
			temp=P1;
			temp=temp&0xf0;
		}
	}
	P1=0xfb;
	temp=P1;
	temp=temp&0xf0;
	while(temp!=0xf0)
	{
		delay(5);
		temp=P1;
		temp=temp&0xf0;
		while(temp!=0xf0)
		{
			switch(temp)
			{
				case 0x70:k=8;
                     break;
				case 0xb0:k=9;
				     break;
				case 0xd0:k=10;
				     break;

				case 0xe0:k=11;
				     break;

			}
		
          
			P0=table[k];
	
			temp=P1;
			temp=temp&0xf0;
		}
	}
}
void main()
{
	while(1)
	{
		keyscan();
//		display(key);
	}
}
