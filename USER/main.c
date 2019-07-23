#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "aiwacMaster.h"

//ALIENTEK ̽����STM32F407������ ʵ��4
//����ͨ��ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK


int main(void)
{ 
 
	u8 t;
	u8 len;	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ�� 
	
	uart1_init(115200);	//���ڳ�ʼ��������Ϊ115200
	uart2_init(115200); // car1
	uart3_init(115200);  //car2
	
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�  
	USART2_jsonParseBuF[0] = '-';
	USART3_jsonParseBuF[0] = '-';
	


	/*
	while(1)
	{
		delay_ms(50); 
		
		// ���� ��С��������
		
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
			
			if( (Car1_CorrectState > -1) && (Car2_CorrectState > -1)) // ��δ��ȫ�յ���С����  ����
			{
				
				break;
			}

		}

	delay_ms(2000);// �ȴ�����ȫ׼����

	
	
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

