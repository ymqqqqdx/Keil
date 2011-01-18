#include <reg52.h>
#include <absacc.h>
void main()
{
	unsigned char i;
	for(i=0;i<40;i++)
	{
		XBYTE[256-i]=i+1;
	}
	while(1);
}