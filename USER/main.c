#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "aiwacMaster.h"



int main(void)
{ 
 int num1 =0;
	u8 t;
	u8 len;	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	
	uart1_init(115200);	//串口初始化波特率为115200
	uart2_init(115200); // car1
	uart3_init(115200);  //car2
	

	USART2_jsonParseBuF[0] = '-';
	USART3_jsonParseBuF[0] = '-';
	
	


		while (1)
		{

			printf("\r\n waiting for data from cars");
			
			delay_ms(100); 
			
			if( (Car1_CorrectState > -1) && (Car2_CorrectState > -1)) // 还未完全收到两小车的  数据
			{
				
				break;
			}

		}


	
	
	//goStartTogether(FRONT_DIRECTION);
	goToLocation(BACK_DIRECTION, TURING_DISTANCE);
	sendTuringOrder(STATE_TURN_LEFT);
	
	goToLocation(BACK_DIRECTION, 0.3);


	
	while (1) 
		{
			delay_ms(1000);
			
			printf("\r\n ok ");
		}
	
}

