#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "aiwacMaster.h"

//ALIENTEK 探索者STM32F407开发板 实验4
//串口通信实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK


int main(void)
{ 
 
	u8 t;
	u8 len;	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	
	uart_init(115200);	//串口初始化波特率为115200
	uart2_init(115200); // car1
	uart3_init(115200);  //car2
	
	LED_Init();		  		//初始化与LED连接的硬件接口  
	USART2_jsonParseBuF[0] = '-';
	USART3_jsonParseBuF[0] = '-';
	
	
	while(1)
	{
		delay_ms(50); 
		
		// 解析 两小车的数据
		PaserCar2_State();
		PaserCar1_State();
		printf("\r\n Car1_moveState:%d",Car1_moveState);
		Aiwac2CARTeamwork();
	
	}
}

