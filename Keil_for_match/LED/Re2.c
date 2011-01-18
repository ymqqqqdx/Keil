#include <reg52.h>
#include <stdio.h>
#include <string.h>
#define U8 unsigned char
#define U16 unsigned int

int add(int a,int b)
{
	return a+b;
}

int sub(int a,int b)
{
	return a-b;
}

int mul(int a,int b)
{
	return a*b;
}

int div(int a,int b)
{
	return a/b;
}

void result(int(*p)(),int a,int b)
{
	int value;
	value=(*p)(a,b);
	return value;
}

void main()
{
	int i=10,j=5,val;
	int (*pt)();
	pt=add;
	val=result(pt,i,j);
  	pt=sub;
	val=result(pt,i,j);
  	pt=mul;
	val=result(pt,i,j);
  	pt=div;
	val=result(pt,i,j);
	while(1);
}
