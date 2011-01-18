#include <reg52.h>
sbit si=P1^0;
sbit sclk=P1^1;
sbit OE=P1^2;
extern unsigned char k;
writetwochar(unsigned char temp5,unsigned char temp4,unsigned char temp3,unsigned char temp2,unsigned char temp1)
{
    int i,t;
    sclk=0;
	 OE=1;
	
    for(i=0;i<k;i++)
    {
 		t=0x80;
		t=t>>k;
		if(temp5&t)
		si=1;
		else
		si=0;
		sclk=1;
		sclk=0;
		temp5=temp5>>1;
    }
    for(i=0;i<8;i++)
    {
        si=temp4&0x01;
        sclk=1;
        sclk=0;
        temp4=temp4>>1;
    }
    for(i=0;i<8;i++)
    {
        si=temp3&0x01;
        sclk=1;
        sclk=0;
        temp3=temp3>>1;
    }
    for(i=0;i<8;i++)
    {
        si=temp2&0x01;
        sclk=1;
        sclk=0;
        temp2=temp2>>1;
    }
    for(i=24;i<32-k;i++)
    {
        si=temp1&0x01;
        sclk=1;
        sclk=0;
        temp1=temp1>>1;
    }
	sclk=1;
	OE=0;
}

