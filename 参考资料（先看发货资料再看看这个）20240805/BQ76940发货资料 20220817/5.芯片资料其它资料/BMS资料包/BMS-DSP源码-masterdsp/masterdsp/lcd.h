#ifndef _lcd_h
#define _lcd_h
void LCDSTA1(int E);
// 对液晶显示器发指令子程序，形参E代表处理的液晶的边(E1或E2边) 
void TRANS1(int TRANS,int E);		// 该函数采用形参和实参的方式传递数值，TRANS为一个形参
// 对液晶显示器写数据子程序，注意在此子程序里面列地址没有加1，需要用户在外
// 对其进行操作，形参E代表处理的液晶的边(E1或E2边) 	
void WRITE1(int WRITE,int CLMADD,int E);		// WRITE代表需要写入的数据，CLMADD代表要的列地址
// 开E1显示子程序，形参E代表处理的液晶的边(E1或E2边) 
void DISP1(int E);
// E1边清屏子程序，形参E代表处理的液晶的边(E1或E2边) 
void CLEAR1( int E);
// 关 E1显示子程序，形参E代表处理的液晶的边(E1或E2边) 
void DISOFF1(int E);
// LCD的E1边初始化子程序，形参E代表处理的液晶的边(E1或E2边) 
void LCD1INIT(int E);
// 汉字显示子程序，入口为存储汉字库的数组的首地址，页地址和起始列地址
// 和选中的E1或E2的边数 
void DISPLAY1(char* ARRAY,int E,int PAGADD,int CLMADD);
// 数字和字母显示子程序，入口为存储字母或数字库的数组的首地址，页地址和
// 始列地址和选中的E1或E2的边数 
void DISPLAY2(char* ARRAY,int E,int PAGADD,int CLMADD);
void displaysoc(float soc);
#endif