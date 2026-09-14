#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usmart.h"
#include "DS3231.h"

//ALIENTEK战舰STM32开发板实验22
//IIC 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司  
 				 	
//要写入到24c02的字符串数组
#define SIZE sizeof(TEXT_Buffer)
 int main(void)
 {	 
	u8  t=0;	
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口初始化为9600
	DS3231_Init();
 	LED_Init();			     //LED端口初始化
	usmart_dev.init(72);	//初始化USMART			 		
 			  
	while(1)
	{
		get_show_time();
		delay_ms(500);
		if(t!=calendar.sec)
		{
			t=calendar.sec;
			printf("年月日:");
			printf("%ld,%d,%d\r\n",calendar.w_year+2000,calendar.w_month,calendar.w_date);
			printf("时分秒:");
			printf("%d,%d,%d\r\n",calendar.hour,calendar.min,calendar.sec);
			LED0=!LED0;
		}
		   
	}
}
