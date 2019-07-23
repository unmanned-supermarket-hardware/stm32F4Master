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
	
	uart1_init(115200);	//串口初始化波特率为115200
	uart2_init(115200); // car1
	uart3_init(115200);  //car2
	
	LED_Init();		  		//初始化与LED连接的硬件接口  
	USART2_jsonParseBuF[0] = '-';
	USART3_jsonParseBuF[0] = '-';
	


	/*
	while(1)
	{
		delay_ms(50); 
		
		// 解析 两小车的数据
		
		printf("\r\n Car1:Car1_CorrectState :%d,  Car1_FDistance:%f,   Car1_moveState:%d",Car1_CorrectState ,Car1_FDistance, Car1_moveState);
		printf("\r\n Car2:Car2_CorrectState :%d,  Car2_FDistance:%f,   Car2_moveState:%d",Car2_CorrectState ,Car2_FDistance, Car2_moveState);

		Aiwac2CARTeamwork();
	
	}

	

	*/

	while (1)
		{
			delay_ms(100); 
			printf("\r\n waiting for data from cars");
			printf("\r\n Car1:Car1_CorrectState :%d,  Car1_FDistance:%f,   Car1_moveState:%d",Car1_CorrectState ,Car1_FDistance, Car1_moveState);
			printf("\r\n Car2:Car2_CorrectState :%d,  Car2_FDistance:%f,   Car2_moveState:%d",Car2_CorrectState ,Car2_FDistance, Car2_moveState);
			
			if( (Car1_CorrectState > -1) && (Car2_CorrectState > -1)) // 还未完全收到两小车的  数据
			{
				
				break;
			}

		}

	delay_ms(2000);// 等待车完全准备好

	
	
	//goStartTogether(FRONT_DIRECTION);
	goToLocation(FRONT_DIRECTION, TURING_DISTANCE);
	sendTuringOrder(STATE_TURN_RIGHT);
	
	goToLocation(FRONT_DIRECTION, 0.30);


	
	while (1) 
		{
			delay_ms(50);
			
			printf("\r\n ok ");
		}
	
}

