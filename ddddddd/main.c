#include <reg52.h>

void main()
{
	U8 i=0,j=0;
	j= (++i)+(++i);
	i=j;
	while(1);
}
