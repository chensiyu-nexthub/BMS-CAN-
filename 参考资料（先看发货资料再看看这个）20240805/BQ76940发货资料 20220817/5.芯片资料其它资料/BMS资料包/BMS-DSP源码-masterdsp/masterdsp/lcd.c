/*该模块包括下面9个子程序
读液晶显示器状态子程序LCDSTA1(E)
对液晶显示器发指令子程序TRANS1(TRANS,E)
对液晶显示器写数据子程序WRITE1(WRITE,CLMADD,E)
开E1显示子程序DISP1(E)
 E1边清屏子程序CLEAR1(int E)
关 E1显示子程序DISOFF1(int E)
LCD的E1边初始化子程序LCD1INIT(int E)
汉字显示子程序DISPLAY1(ARRAY,E,PAGADD,CLMADD)
数字和字母显示子程序DISPLAY2(ARRAY,E,PAGADD,CLMADD)*/
char chari[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,};//.
char char0[16]={0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,};//0		
char char1[16]={0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,};//1
char char2[16]={0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,};//2
char char3[16]={0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,};//3
char char4[16]={0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,};//4
char char5[16]={0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,};//5
char char6[16]={0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,};//6;
char char7[16]={0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,};//7
char char8[16]={0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,};//8
char char9[16]={0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,};//9
// 读液晶显示器状态子程序，形参E代表处理的液晶的边(E1或E2边) 
void LCDSTA1(int E)
{
	int	E1FLAG;
	*PBDATDIR=*PBDATDIR&0X00FF;// IOPB口为输入口 
	while(1)
	{
		if(E==1)
		*PADATDIR=(*PADATDIR|0X0050)&0X0FF5F;
			// E1=1，E2=0，R/W=1，A0=0，以便读晶的状态 
		else	*PADATDIR=(*PADATDIR|0X0060)&0X0FF6F;
			// E1=0，E2=1，R/W=1，A0=0，以便液晶的状态 
		E1FLAG=*PBDATDIR&0X0080;
	if(E1FLAG==0x0000)  break;		// 若液晶空闲，则停止等待 
	}
}

// 对液晶显示器发指令子程序，形参E代表处理的液晶的边(E1或E2边) 
void TRANS1(int TRANS,int E)		// 该函数采用形参和实参的方式传递数值，TRANS为一个形参
{
	LCDSTA1(E);					// 判断液晶是否为忙 
	*PBDATDIR=*PBDATDIR|0XFF00;	// IOPB口为输出口 
	if(E==1)  *PADATDIR=(*PADATDIR|0X0010)&0X0FF1F;
		// E1=1，E2=0，R/W=0，A0=0，以便写入令字 
	else  *PADATDIR=(*PADATDIR|0X0020)&0X0FF2F;
		// E1=0，E2=1，R/W=0，A0=0，以便入命令字 		
	*PBDATDIR=(*PBDATDIR&0X0FF00)|TRANS;
		// 需要写入的命令字送入数据线 
	if(E==1)  *PADATDIR=*PADATDIR&0X0FFEF;	// E1=0，写入指令 
	else  *PADATDIR=*PADATDIR&0X0FFDF;		// E2=0，写入指令 
}

// 对液晶显示器写数据子程序，注意在此子程序里面列地址没有加1，需要用户在外
// 对其进行操作，形参E代表处理的液晶的边(E1或E2边) 	
void WRITE1(int WRITE,int CLMADD,int E)		// WRITE代表需要写入的数据，CLMADD代表要的列地址
{
	TRANS1(CLMADD,E);			// 设置列地址 
	LCDSTA1(E);					// 查询液晶是否为空闲 
	*PBDATDIR=*PBDATDIR|0XFF00;	// IOPB口为输出口 
	if(E==1) 	*PADATDIR=(*PADATDIR|0X0090)&0X0FF9F;
		// E1=1，E2=0，R/W=0，A0=1，以便写入要显示的数据 
	else  *PADATDIR=(*PADATDIR|0X00A0)&0X0FFAF;
		// E1=0，E2=1，R/W=0，A0=1，以便要显示的数据 	
	*PBDATDIR=(*PBDATDIR&0X0FF00)|WRITE;
	// 需要写入的数据送入数据线 
	if(E==1)	*PADATDIR=*PADATDIR&0X0FFEF;// E1=0，写入数据 
	else	*PADATDIR=*PADATDIR&0X0FFDF;		// E2=0，写入数据 
}

// 开E1显示子程序，形参E代表处理的液晶的边(E1或E2边) 
void DISP1(int E)
{
	int	E1FLAG;	
	while(1)
	{
		TRANS1(0X0AF,E);			// 送出控制命令 
		LCDSTA1(E);					// 判断液晶是否为空闲 
		*PBDATDIR=*PBDATDIR&0X00FF;	// IOPB口为输入口 
		if(E==1)	*PADATDIR=(*PADATDIR|0X0050)&0X0FF5F;
			// E1=1，E2=0，R/W=1，A0=0，以便读晶的状态 
		else	*PADATDIR=(*PADATDIR|0X0060)&0X0FF6F;
			// E1=0，E2=1，R/W=1，A0=0，以便液晶的状态 
		E1FLAG=*PBDATDIR&0X0020;
		if(E1FLAG==0)  break;		// 如果液晶没被打开，则继续打开 
	}
}

// E1边清屏子程序，形参E代表处理的液晶的边(E1或E2边) 
void CLEAR1(int E)
{	
	int	PAGEADD,PAGENUM,CLMADD,CLMSUM;// 定义所需要的局部变量 
	for(PAGENUM=0X04,PAGEADD=0x0B8;PAGENUM>0;PAGENUM--,PAGEADD++)
	{
		TRANS1(PAGEADD,E);					// 设置相应的页地址 
		for(CLMSUM=0X00;CLMSUM<0X3E;CLMSUM++)
		{
			LCDSTA1(E);						// 判断液晶是否为空闲 
			WRITE1(0X00,CLMSUM,E);			// 向相应的列送0以清屏 
		}
	}
}

// 关 E1显示子程序，形参E代表处理的液晶的边(E1或E2边) 
void DISOFF1(int E)
{
	int	E1FLAG;
	while(1)  
	{
		TRANS1(0X0AE,E);				// 发出控制命令 
		LCDSTA1(E);						// 判断液晶是否为空闲 
		*PBDATDIR=*PBDATDIR&0X00FF;		// IOPB口为输入口 
		if(E==1)	*PADATDIR=(*PADATDIR|0X0050)&0X0FF5F;
		// E1=1，E2=0，R/W=1，A0=0，以便读晶的状态
		else	*PADATDIR=(*PADATDIR|0X0060)&0X0FF6F; // E1=0，E2=1，R/W=1，A0=0，以便
		// 液晶的状态 
		E1FLAG=*PBDATDIR&0X0020;
		if(E1FLAG==1)		break;		// 如果液晶没有被关闭，则继续关 
	}
}

// LCD的E1边初始化子程序，形参E代表处理的液晶的边(E1或E2边) 
void LCD1INIT(int E)
{
	DISOFF1(E);					// 关显示E1 
	TRANS1(0X0A4,E);				// 静态显示驱动 
	TRANS1(0X0A9,E);				// 占空比为1/32 
	TRANS1(0X0E2,E);				// 复位 
	TRANS1(0X0A0,E);				// ADC选择正常输出 
	CLEAR1(E);					// 清屏 
	LCDSTA1(E);					// 判断液晶是否为空闲 
	DISP1(E);						// 开显示 
}

// 汉字显示子程序，入口为存储汉字库的数组的首地址，页地址和起始列地址
// 和选中的E1或E2的边数 
/*void DISPLAY1(int* ARRAY,int E,int PAGADD,int CLMADD) 
{
	int K,TEMP;
	TRANS1(0X0C0,E);				// 从第0行开始显示 
	TRANS1(PAGADD,E);				// 送需要显示的页地址 
	for(K=0;K<8;K++,CLMADD++,ARRAY++)
	{
		TEMP=*ARRAY&0X00FF;
		WRITE1(TEMP,CLMADD,E);	// 送需要显示的数据字节 
		CLMADD++;
		TEMP=(*ARRAY&0X0FF00)>>8;
		WRITE1(TEMP,CLMADD,E);	// 送需要显示的数据字节 
	}	
	CLMADD=CLMADD-16;				// 恢复列地址 
	PAGADD=PAGADD+1;				// 页地址加1 
	TRANS1(PAGADD,E);
	for(;K<16;K++,CLMADD++,ARRAY++)
	{
		TEMP=*ARRAY&0X00FF;
		WRITE1(TEMP,CLMADD,E);	// 送需要显示的数据字节 
		CLMADD++;
		TEMP=(*ARRAY&0X0FF00)>>8;
		WRITE1(TEMP,CLMADD,E);	// 送需要显示的数据字节 
	}
}*/
void DISPLAY1(char* ARRAY,int E,int PAGADD,int CLMADD)
{
	int K;
	char TEMP;
	TRANS1(0X0C0,E);				// 从第0行开始显示 
	TRANS1(PAGADD,E);				// 送需要显示的页地址 
	for(K=0;K<16;K++,CLMADD++,ARRAY++)
	{
		TEMP=*ARRAY&0X00FF;
		WRITE1(TEMP,CLMADD,E);	// 送需要显示的数据字节 
	}	
	CLMADD=CLMADD-16;				// 恢复列地址 
	PAGADD=PAGADD+1;				// 页地址加1 
	TRANS1(PAGADD,E);
	for(;K<32;K++,CLMADD++,ARRAY++)
	{
		TEMP=*ARRAY&0X00FF;
		WRITE1(TEMP,CLMADD,E);	// 送需要显示的数据字节 
	}
}

// 数字和字母显示子程序，入口为存储字母或数字库的数组的首地址，页地址和
// 始列地址和选中的E1或E2的边数 
/*void DISPLAY2(int* ARRAY,int E,int PAGADD,int CLMADD)
{
	int K,TEMP;
	TRANS1(0X0C0,E);			// 从第0行开始显示 
	TRANS1(PAGADD,E);		// 送需要显示的页地址 
	for(K=0;K<4;K++,CLMADD++,ARRAY++)
	{
		TEMP=*ARRAY&0X00FF;
		WRITE1(TEMP,CLMADD,E);	// 送需要显示的数据字节 
		CLMADD++;
		TEMP=(*ARRAY&0X0FF00)>>8;
		WRITE1(TEMP,CLMADD,E);	// 送需要显示的数据字节 
	}
	CLMADD=CLMADD-8;			// 恢复列地址 
	PAGADD=PAGADD+1;			// 页地址加1 
	TRANS1(PAGADD,E);
	for(;K<8;K++,CLMADD++,ARRAY++)
	{
		TEMP=*ARRAY&0X00FF;
		WRITE1(TEMP,CLMADD,E);	// 送需要显示的数据字节 
		CLMADD++;
		TEMP=(*ARRAY&0X0FF00)>>8;
		WRITE1(TEMP,CLMADD,E);	// 送需要显示的数据字节 
	}
}*/
void DISPLAY2(char* ARRAY,int E,int PAGADD,int CLMADD)
{
	int K;
	char TEMP;
	TRANS1(0X0C0,E);			// 从第0行开始显示 
	TRANS1(PAGADD,E);		// 送需要显示的页地址 
	for(K=0;K<8;K++,CLMADD++,ARRAY++)
	{
		TEMP=*ARRAY&0X00FF;
		WRITE1(TEMP,CLMADD,E);	// 送需要显示的数据字节 
	}
	CLMADD=CLMADD-8;			// 恢复列地址 
	PAGADD=PAGADD+1;			// 页地址加1 
	TRANS1(PAGADD,E);
	for(;K<16;K++,CLMADD++,ARRAY++)
	{
		TEMP=*ARRAY&0X00FF;
		WRITE1(TEMP,CLMADD,E);	// 送需要显示的数据字节 
	}
}
 
/* 主程序 
main()
{
	initial();						// 系统初始化 
	// 显示第一行"DSP  实验板" 
	LCD1INIT(1);					// LCD的E1边初始化 
	LCD1INIT(2);					// LCD的E2边初始化 
	DISPLAY2(char1,1,0x0BA,00);	// 调用字母和数字显示子程序   
	DISPLAY2(char2,1,0x0BA,8);	// 调用字母和数字显示子程序 
	DISPLAY2(char3,1,0x0BA,17);	// 调用字母和数字显示子程序 
	DISPLAY1(char4,2,0x0BA,0);	// 调用汉字显示子程序 
	DISPLAY1(char5,2,0x0BA,15);	// 调用汉字显示子程序 
	DISPLAY1(char6,2,0x0BA,30);	// 调用汉字显示子程序 
	// 显示第行行"DSP  实验板" 
	DISPLAY2(char1,1,0x0B8,00);	// 调用字母和数字显示子程序   
	DISPLAY2(char2,1,0x0B8,8);	// 调用字母和数字显示子程序 
	DISPLAY2(char3,1,0x0B8,17);	// 调用字母和数字显示子程序 
	DISPLAY1(char4,2,0x0B8,0);	// 调用汉字显示子程序 
	DISPLAY1(char5,2,0x0B8,15);	// 调用汉字显示子程序 
	DISPLAY1(char6,2,0x0B8,30);	// 调用汉字显示子程序 
}*/
/*显示SOC值*/
void displaysoc(float soc)
{
	int p,i;
	if(10000*soc>=5)soc+=0.001;//对soc四舍五入保留3位有效数字
	DISPLAY2(chari,1,0x0BA,00);	// 调用字母和数字显示子程序
	for(i=0;i<3;i++)
	{
		p=(int)soc/0.1;
		switch(p)
		{
		case 0:	
			DISPLAY2(char0,1,0x0BA,8+i*8);
			break;
		case 1:
			DISPLAY2(char1,1,0x0BA,8+i*8);
			break;
		case 2:
			DISPLAY2(char2,1,0x0BA,8+i*8);
			break;			
		case 3:
			DISPLAY2(char3,1,0x0BA,8+i*8);
			break;
		case 4:
			DISPLAY2(char4,1,0x0BA,8+i*8);
			break;
		case 5:
			DISPLAY2(char5,1,0x0BA,8+i*8);
			break;
		case 6:
			DISPLAY2(char6,1,0x0BA,8+i*8);
			break;
		case 7:
			DISPLAY2(char7,1,0x0BA,8+i*8);
			break;
		case 8:
			DISPLAY2(char8,1,0x0BA,8+i*8);
			break;
		case 9:
			DISPLAY2(char9,1,0x0BA,8+i*8);
			break;
		}
		soc=soc-p*0.1;
	}
}