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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ�� 
	
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	uart2_init(115200); // car1
	uart3_init(115200);  //car2
	
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�  
	zone_1_car1_jsonParseBuF[0] = '-';
	zone_1_car2_jsonParseBuF[0] = '-';
	
	
	while(1)
	{
		delay_ms(50); 
		
		// ���� ��С��������
		PaserCar1_State();
		//delay_ms(10);// ��һ�� 
		PaserCar2_State();
		
		printf("\r\n Car1:Car1_CorrectState :%d,  Car1_FDistance:%f,   Car1_moveState:%d",Car1_CorrectState ,Car1_FDistance, Car1_moveState);
		printf("\r\n Car2:Car2_CorrectState :%d,  Car2_FDistance:%f,   Car2_moveState:%d",Car2_CorrectState ,Car2_FDistance, Car2_moveState);

		Aiwac2CARTeamwork();
	
	}
}

