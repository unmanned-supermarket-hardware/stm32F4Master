#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "aiwacMaster.h"



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
	zone_1_car1_jsonParseBuF[0] = '-';
	zone_1_car2_jsonParseBuF[0] = '-';
	
	
	while(1)
	{
		delay_ms(50); 
		
		// 解析 两小车的数据
		PaserCar1_State();
		//delay_ms(10);// 用一个 
		PaserCar2_State();
		
		printf("\r\n Car1:Car1_CorrectState :%d,  Car1_FDistance:%f,   Car1_moveState:%d",Car1_CorrectState ,Car1_FDistance, Car1_moveState);
		printf("\r\n Car2:Car2_CorrectState :%d,  Car2_FDistance:%f,   Car2_moveState:%d",Car2_CorrectState ,Car2_FDistance, Car2_moveState);

		Aiwac2CARTeamwork();
	
	}
}

