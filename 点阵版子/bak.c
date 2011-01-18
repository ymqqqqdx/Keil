void main()
{
	U8 i,j,k;
	U16 total;
	total=sizeof(table);
	P0=0x00;
	while(1)
	{
		for(j=0;j<total/3-160;j++)
		{
			for(k=0;k<10;k++)
			{
				for(i=0;i<160;i++)
				{
					if(i)
						dat=0;
					else
						dat=1;
					clk=0;
					clk=1;
					P0=table[3*(i+j)];
					P1=table[3*(i+j)+1];
					P2=table[3*(i+j)+2];
					delay(1);
					P0=P1=P2=0x00;
				}
			}
		}
	}
}


void Ser_Handle() interrupt 4
{
	RI=0;
	BUF[count]=SBUF;
	count++;
	if(count==503)
		count=0;
/*	if((count+1)%72==0)
	{
		DataSize++;
		ISP_sectorerase(0x00);
		ISP_write(0x00,DataSize);
		if(count==503)
		{
			Write2Flash(0x200);
		}
	}*/
	SBUF='O';
	while(!TI);
	TI=0;
}

	while(1)
	{
//		for(j=0;j<Total*16;j++)
//		{
//			for(k=0;k<10;k++)
//			{
				for(i=0;i<160;i++)
				{
//					temp=0x200+3*(i+j);//+((3*(i+j))/504)*512+(3*(i+j))%504
//					DBUF[0]=ISP_read(temp);
//					DBUF[1]=ISP_read(temp+1);
//					DBUF[2]=ISP_read(temp+2);
					if(i)
						dat=0;
					else
						dat=1;
					clk=0;
					clk=1;
					P0=BUF[3*i];
					P1=BUF[3*i+1];
					P2=BUF[3*i+2];
					delay(3);
					P0=P1=P2=0x00;
				}
//			}
//		}
	}
	
	
	
	
	
	
	
	ISP_sectorerase(0x0200);
	for(i=0;i<504;i++)
		ISP_write(0x0200+i,table[i]);
	for(i=0;i<504;i++)
		BUF[i]=ISP_read(0x0200+i);
	while(1)
	{
		for(i=0;i<160;i++)
		{
			if(i)
				dat=0;
			else
				dat=1;
			clk=0;
			clk=1;
			P0=BUF[3*i];
			P1=BUF[3*i+1];
			P2=BUF[3*i+2];
			delay(3);
			P0=P1=P2=0x00;
		}
	}