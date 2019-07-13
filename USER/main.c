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
	
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	uart2_init(115200); // car1
	uart3_init(115200);  //car2
	
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�  
	USART2_jsonParseBuF[0] = '-';
	USART3_jsonParseBuF[0] = '-';
	
	
	while(1)
	{
		delay_ms(50); 
		
		// ���� ��С��������
		PaserCar2_State();
		PaserCar1_State();
		printf("\r\n Car1_moveState:%d",Car1_moveState);
		Aiwac2CARTeamwork();
	
	}
}

