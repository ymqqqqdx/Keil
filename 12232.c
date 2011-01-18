#include <reg52.h>
#include <string.h>
unsigned char code table[]="河北科技师范学院";
unsigned char code table1[]="0123456789-";
sbit ln=P2^7;
sbit ls=P2^5;
sbit rw=P2^6;
void delay(int z)
{
	int i,j;
	for(i=0;i<z;i++)
		for(j=0;j<110;j++);
}
void write_com(unsigned char com)
{
	rw=0;
	ls=0;
    P0=com;
    ln=1;
    ln=0;
	rw=1;
}
void write_data(unsigned char date)
{
	rw=0;
	ls=1;
    P0=date;
    ln=1;
    ln=0;
	rw=1;
}
void init_12232()
{
	 rw=0;
    ln=0;
    write_com(0x30);
    write_com(0x02);
    write_com(0x0f);
	 write_com(0x01);	
	 write_com(0x0c);
	 delay(50);
}
void main()
{
	unsigned char i;
	init_12232();
	write_com(0x80);
	for(i=0;i<16;i++)
		write_data(table[i]);
	write_com(0x80+0x40);
	write_data(table[2]);
	write_data(table[0]);
	write_data(table[1]);
	write_data(table[0]);
	write_data(table[10]);
	write_data(table[0]);
	write_data(table[7]);
	write_data(table[10]);
	write_data(table[0]);
	write_data(table[5]);
	while(1);
}