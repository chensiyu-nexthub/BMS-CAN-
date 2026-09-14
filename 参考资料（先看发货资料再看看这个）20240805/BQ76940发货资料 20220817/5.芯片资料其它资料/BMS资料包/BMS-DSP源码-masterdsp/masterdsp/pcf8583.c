// 软件延时子程序 
void delay()
{
	nop();nop();nop();//int  i;
	//for(i=0X07D;i--;)	{	i=i;}
}
// 启动I2C总线子程序 
void START()
{
	*PFDATDIR=*PFDATDIR|0X03;		// SDA=SCL=1 
	delay( );							// 软件延时，以使时序匹配 
	*PFDATDIR=*PFDATDIR&0X0FFFD;	// SDA=0 
	delay();							// 软件延时，以使时序匹配 
	*PFDATDIR=*PFDATDIR&0X0FFFE;		// SCL=0 
	delay();							// 软件延时，以使时序匹配 
}
// 发送字节子程序 
void TRANSMIT(int TRAN)
{
	int flag,sz;						// 定义需要的局部变量 
	*PFDATDIR=*PFDATDIR&0X0FFFE;		// SCL=0    
	delay();							// 软件延时，以使时序匹配 
	for(flag=0x0080;flag!=0x00;flag=flag/2)	
	{
		sz=TRAN&flag;				// 屏蔽掉不需要的位 
		if(sz==0)	*PFDATDIR=*PFDATDIR&0X0FFFD;// 如果相应的位为0，则SDA=0 
		else	*PFDATDIR=*PFDATDIR|0X02;// 如果相应的位为1，则SDA=1 
		delay();						// 软件延时，以使时序匹配 
		*PFDATDIR=*PFDATDIR|0X01;	// SCL=1 
		delay();						// 软件延时，以使时序匹配 
		*PFDATDIR=*PFDATDIR&0X0FFFE;	// SCL=0 
		delay();						// 软件延时，以使时序匹配 
	}
}
// 从机(即PCF8583芯片)应答子程序。返回值为0时，代表操作成功;返回值为1时，代表操作失败 
int	SLAVE_ACK()
{
	int sz,k=0;					// 定义所需要的局部变量 
	*PFDATDIR=*PFDATDIR|0X02;		// SDA=1 
	delay();							// 软件延时，以使时序匹配 
	*PFDATDIR=*PFDATDIR&0X0FDFF;	// 设置IOPC5(SDA)为输入 
	delay();							// 软件延时，以使时序匹配 
	*PFDATDIR=*PFDATDIR|0X01;		// SCL=1 
	delay();							// 软件延时，以使时序匹配 
	sz=*PFDATDIR&0X0002;				// 检测数据位 
	if(sz==0X0020)  k=1;				// 如果数据位为1，则证明失败，则令k=1 
	else  k=0;  	 					// 如果数据位为0，则证明成功，则保持k=0不变 
	*PFDATDIR=*PFDATDIR|0X0200;		// 设置IOPC5(SDA)为输出 
	*PFDATDIR=*PFDATDIR&0X0FFFC;	// SCL=SDA=0 
	return(k);
}
// I2C停止子程序 
void    STOP()
{ 
	*PFDATDIR=*PFDATDIR&0X0FFFD;	// SDA=0 
	delay();
	*PFDATDIR=*PFDATDIR|0X0001;		// SCL=1 
	delay( );							// 软件延时，以使时序匹配 
	*PFDATDIR=*PFDATDIR|0X0002;		// SDA=1 
}
// 字节写子程序，即向I2C器件写1个字节的数据，入口为地址BYTE_ADDR和需要写入的字节
// 内容T_DATA。返回值为0时，代表操作成功;返回值为1时，代表操作失败 
int	BYTE_WR(int BYTE_ADDR,int T_DATA)
{
	int  k;
	START();							// 启动I2C总线 
	TRANSMIT(0X0A0);					// 送写控制字 
	k=SLAVE_ACK();					// 从机(即PCF8583芯片)应答 
	if(k==0)	 TRANSMIT(BYTE_ADDR);	// 送出地址 
	if(k==0)	 k=SLAVE_ACK();			// 从机(即PCF8583芯片)应答 
	if(k==0)	TRANSMIT(T_DATA);		// 送出需要保存的数据 
	if(k==0)	k=SLAVE_ACK();			// 从机(即PCF8583芯片)应答 
	if(k==0)	STOP();					// 设置停止状态 
	return(k);
}
// 接收一个字节子程序，出口为接收到的数据R_DATA 
int RECEIVE()	
{
	int  R_DATA=0,sz,i;
	*PFDATDIR=*PFDATDIR&0X0FDFF;	// 设置IOPC5(SDA)为输入 
	*PFDATDIR=*PFDATDIR&0X0FFFE;		// SCL=0 
	delay();							// 软件延时，以使时序匹配  
	for(i=0;i<8;i++)	
	{
		R_DATA=R_DATA<<1;			// R_DATA左移一位 
		*PFDATDIR=*PFDATDIR|0X0001;	// SCL=1 
		delay();						// 软件延时，以使时序匹配   
		sz=*PFDATDIR&0X0002;			// 取得相应的数据位 
		if(sz==0)	 R_DATA=R_DATA&0XFFFE;	// 如果数据位为0，则R_DATA最低位清0 
		else  R_DATA=R_DATA|0x0001;		// 如果数据位为1，则R_DATA最低位置1 
		*PFDATDIR=*PFDATDIR&0X0FFFE;		// SCL=0 
		delay();							// 软件延时，以使时序匹配  
	}
	*PFDATDIR=*PFDATDIR|0X0200;		// 设置IOPC5(SDA)为输出 
	return(R_DATA);					// 返回接收的字节 
}
// 主机无应答信号子程序 
void NO_ACK()
{
	*PFDATDIR=*PFDATDIR|0X0001;		// SCL=1 
	delay();							// 软件延时，以使时序匹配 
	*PFDATDIR=*PFDATDIR&0X0FFFE;		// SCL=0 
}
// 主机应答子程序 
void MASTER_ACK()
{
	*PFDATDIR=*PFDATDIR&0X0FFFE;		// SCL=0 
	*PFDATDIR=*PFDATDIR&0X0FFFD;	// SDA=0 
	*PFDATDIR=*PFDATDIR|0X0001;		// SCL=1 
	delay();							// 软件延时，以使时序匹配 
	*PFDATDIR=*PFDATDIR&0X0FFFE;		// SCL=0 
}
// 字节读子程序，即从I2C器件读出1个字节的数据，入口为需要读出的地址BYTE_ADDR，出口
// 为读出的数据R_DATA，通过C语言的参数传递功能实现。返回值为0X0FFFF时，表示操作失败;
// 否则操作成功 
int	BYTE_RD(int  BYTE_ADDR)
{
	int	k,R_DATA;
	START();						// 启动I2C总线 
	TRANSMIT(0XA0);				// 送出写控制字，以写入地址字节 
	k=SLAVE_ACK( );				// 从机(即PCF8583芯片)应答 
	if(k==0)	TRANSMIT(BYTE_ADDR);// 送出需要读出数据的地址 
	k=SLAVE_ACK();				// 从机(即PCF8583芯片)应答 
	if(k==0)  
	{
		START( );					// 启动I2C总线 
	TRANSMIT(0XA1);
	}							// 送出读控制字 
	k=SLAVE_ACK();				// 从机(即PCF8583芯片)应答 
	if(k==0)	
	{
		R_DATA=RECEIVE();		// 接收PCF8583发出的数据 
		NO_ACK();				// 主机不作应答 
		STOP();					// 设置停止状态 
	}
	if(k==0)  return(R_DATA);			// 返回一个读出的数据 
	else   return(0X0FFFF);			// 如果整个读过程有误，则返回0X0FFFF 
}
// 连续写子程序，入口为需要写的起始地址ADDR，存储需要写入数据的数组的首地址ARRY，需要
// 写入的数据的个数N 
int	CON_WR(int ADDR,int* ARRAY,int N)
{
	int	k;
	START();						// 启动I2C总线 
	TRANSMIT(0X0A0);				// 送写控制字 
	k=SLAVE_ACK();				// 从机(即PCF8583芯片)应答 
	if(k==0) TRANSMIT(ADDR);	// 送出要写数据的起始地址 
	if(k==0) k=SLAVE_ACK();		// 从机(即PCF8583芯片)应答 
	if(k==0)	 
	{
	for(;N>0;N--,ARRAY++)	
	{ 
		TRANSMIT(*ARRAY);	// 送出需要保存的数据，保存在数组ARRAY中 
		k=SLAVE_ACK();		// 从机(即PCF8583芯片)应答，地址自动加1 
		if(k==1)	break;
	}
	}
	STOP();						// 设置停止状态 
	return(k);
}
// 连续读子程序，入口为需要读的起始地址ADDR，存储读出数据的数组首地址ARRY，需要读出的
// 数据的个数N 
int	CON_RD(int ADDR,int* ARRAY,int N)
{
	int	k,R_DATA;
	START();						// 启动I2C总线 
	TRANSMIT(0XA0);				// 送出写控制字，以写入地址字节 
	k=SLAVE_ACK();				// 从机(即PCF8583芯片)应答 
	if(k==0)	TRANSMIT(ADDR);		// 送出需要读出数据的地址 
	k=SLAVE_ACK();				// 从机(即PCF8583芯片)应答 
	if(k==0)	
	{
		START();					// 启动I2C总线 
		TRANSMIT(0XA1);
	}							// 送出读控制字 
	k=SLAVE_ACK();				// 从机(即PCF8583芯片)应答 
	if(k==0)	
	{
		for(;N>1;N--,ARRAY++)	
		{
			R_DATA=RECEIVE();	// 接收PCF8583发出的数据 
			*ARRAY=R_DATA;		// 读出的数据存入数组 
			MASTER_ACK();		// 主机应答，地址自动加1   
		}
		R_DATA=RECEIVE();		// 接收PCF8583发出的数据        
		*ARRAY=R_DATA;			// 读出的数据存入数组 
		NO_ACK();				// 主机不作应答 
		STOP();					// 设置停止状态 
	}
	return(k);						// k=0时表示操作成功，k=1时表示操作失败 
}
//连续写入N个浮点数
int	CON_WRF(int ADDR,float* ARRAY,int N)
{
	int i,ch[4];
	for(i=0;i<N;i++,ARRAY++)
	{
		int* p=(int*)ARRAY;
		ch[0]=*p&0x00FF;
		ch[1]=(*p&0xFF00)>>8;
		p++;
		ch[2]=*p&0x00FF;
		ch[3]=(*p&0xFF00)>>8;
		CON_WR(ADDR+4*i,ch,4);
	}
}
//连续读出N个浮点数
int	CON_RDF(int ADDR,float* ARRAY,int N)
{
	int *p,i,ch[4];
	for(i=0;i<N;i++,ARRAY++)
	{
		CON_RD(ADDR+4*i,ch,4);
		*p=ch[0];
		*p=*p|(ch[1]<<8);
		p++;
		*p=ch[2];
		*p=*p|(ch[3]<<8);
		p--;
		ARRAY=(float*)p;
	}
}
//计算时间差ARRAY1-ARRAY2精确到分
int CalPlaceTime(int *ARRAY1,int *ARRAY2)

{
/*	int Ratio[7]=[0, 0, 1, 60, 24*60, 30*24*60, 12*30*24*60];//毫秒、秒、分、...
	int k=0, i;
	for(i=0; i<7;i++, ARRAY1++,ARRAY2++)
	{
		if(*ARRAY1-*ARRAY2)
			k+=(*ARRAY1-*ARRAY2)*Ratio[i];
	}*/
	if(*(ARRAY1+6)-*(ARRAY2+6))	return 0x7FFF;
	else if(*(ARRAY1+5)-*(ARRAY2+5))	return 0x7FFF;
	else if(*(ARRAY1+4)-*(ARRAY2+4))	return 0x7FFF;
	else if(*(ARRAY1+3)-*(ARRAY2+3))	return (*(ARRAY1+3)-*(ARRAY2+3))*60+(*(ARRAY1+2)-*(ARRAY2+2));
	else return (*(ARRAY1+2)-*(ARRAY2+2));
}

