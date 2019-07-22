#include "sys.h"
#include "usart.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F4̽���߿�����
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/6/10
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart1_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}


void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
#endif	




//����2�жϷ������
//���ڷ��ͻ����� 
u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; //���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; //���ջ���,���USART2_MAX_RECV_LEN���ֽ�.

u16 USART2_RX_STA = 0;


//��ʼ��IO ����2 
//bound:������
void uart2_init(u32 bound){

NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;


//1.����ʱ�Ӻͺ�GPIOʱ��ʹ�ܡ�
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
//2.�������Ÿ�����ӳ�� 
GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2
GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2  
//3.GPIO�˿�ģʽ����
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2��GPIOA3��ʼ��
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��GPIOA2����GPIOA3
//4.���ڲ�����ʼ�������ò����ʣ��ֳ�����żУ��Ȳ���
USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
USART_Init(USART2, &USART_InitStructure); //��ʼ������2����
//5.��ʼ��NVIC
NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�3
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
//6.�����ж�
USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�  
//7.ʹ�ܴ��� 
USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

	
}

u8 UASRT2 = 0;
char USART2_startMS = '+';	//����Э��ǰ���ֽ�			#��
u8 USART2_startGetMS = 0;		// 0�������ܿ�ʼ��1������  ���ݳ���λ 2����ʼ����json��
int	USART2_dataLen = -1;		// json�ַ����ĳ���
u8 USART2_jsonBuF[1000]; 			// ���жϵ�ʱ�� �洢���յ�json �ַ���
int USART2_jsonDataCount = 0;  //��ǰ���յ�  json �ַ�����
u8 USART2_jsonParseBuF[1000]; 			//������ʱ���� �洢���յ�json �ַ�������ֹ���жϹ���һ��  �ַ��� ��д ������
int uart2GetLen = 0;  //  ����Э����յ��� �ֽ���Ŀ

void USART2StateTo0(void)
{
	// �ָ���ʼ��
	USART2_startMS = '+';	//����Э��ǰ���ֽ�			#��
	USART2_startGetMS = 0; 	// 0�������ܿ�ʼ��1������  ���ݳ���λ 2����ʼ����json��
	USART2_dataLen = -1;		// json�ַ����ĳ���
	memset(USART2_jsonBuF, 0, sizeof(USART2_jsonBuF));
	USART2_jsonDataCount = 0;	//��ǰ���յ�  json �ַ�����
	uart2GetLen = 0;
}

void USART2_IRQHandler(void)                	//����2�жϷ������
{

	u8 temp;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
		
		temp =USART2->DR;


		// �ж�Э�����ݵĿ�ͷ
	
		if (USART2_startGetMS == 0)
		{
			if (temp == '#')
			{
				USART2_startMS = '#';
				uart2GetLen++;
			}
			else if ((temp == '!') && (USART2_startMS == '#') && (uart2GetLen == 1)) 
			{
				USART2_startGetMS = 1;// Э���־ ǰ���ֽ� ����ok	
				uart2GetLen++;
			}
			else if ((temp != '!')  && (USART2_startMS == '#')  && (uart2GetLen == 1)) // ������  Э�飬����  ����
			{
				USART2StateTo0();

			}
		}
		else if (USART2_startGetMS == 1)// ���� Э������  �� json �ַ����ĳ���
		{
			if (USART2_dataLen == -1)
			{
				USART2_dataLen = temp*256;
			}else if(USART2_dataLen != -1)
			{
				USART2_dataLen = USART2_dataLen + temp;
				USART2_startGetMS =2;				
			}		
		}else if (USART2_startGetMS == 2)	// // ��ʼ����	Json ��
		{
			
			USART2_jsonBuF[USART2_jsonDataCount] = temp;
			USART2_jsonDataCount++;
			
			if (USART2_jsonDataCount == USART2_dataLen)  //  ���ν������
			{
				strcpy(USART2_jsonParseBuF,USART2_jsonBuF);
				USART2StateTo0();	
				PaserCar1_State();
				
			}
		}


	}

} 
	
//////////////////////////////////////////////////////////////////
/**************************ʵ�ֺ���**********************************************
*��    ��:		usart2����һ���ֽ�
*********************************************************************************/
void usart2_send(u8 data)
{
	USART2->DR = data;
	while((USART2->SR&0x40)==0);	
}


/**************************ʵ�ֺ���**********************************************
*��    ��:		usart2����һ���ַ���
*********************************************************************************/
void usart2_sendString(char *data,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)
	{
		USART2->DR = data[i];
		while((USART2->SR&0x40)==0);	
	}
	
}

 


//////////////////////////////////////////////////////////////////
/************************************************************************
		usart3   ����
*********************************************************************************/

//����3�жϷ������
//���ڷ��ͻ����� 
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; //���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; //���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
    
u16 USART3_RX_STA = 0;


//��ʼ��IO ����3 
//bound:������
void uart3_init(u32 bound){

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;


	//1.����ʱ�Ӻͺ�GPIOʱ��ʹ�ܡ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
	//2.�������Ÿ�����ӳ�� 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART3  
	//3.GPIO�˿�ģʽ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOB11��GPIOB10��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��GPIOB11����GPIOB10
	//4.���ڲ�����ʼ�������ò����ʣ��ֳ�����żУ��Ȳ���
	USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������2����
	//5.��ʼ��NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	//6.�����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�  
	//7.ʹ�ܴ��� 
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

	
}

u8 UASRT3 = 0;
char USART3_startMS = '+';	//����Э��ǰ���ֽ�			#��
u8 USART3_startGetMS = 0;		// 0�������ܿ�ʼ��1������  ���ݳ���λ 2����ʼ����json��
int	USART3_dataLen = -1;		// json�ַ����ĳ���
u8 USART3_jsonBuF[1000]; 			// ���жϵ�ʱ�� �洢���յ�json �ַ���
int USART3_jsonDataCount = 0;  //��ǰ���յ�  json �ַ�����
u8 USART3_jsonParseBuF[1000]; 			//������ʱ���� �洢���յ�json �ַ�������ֹ���жϹ���һ��  �ַ��� ��д ������
int uart3GetLen = 0; 


void  USART3StateTo0(void )
{
	// �ָ���ʼ��
	USART3_startMS = '+';	//����Э��ǰ���ֽ�			#��
	USART3_startGetMS = 0;	// 0�������ܿ�ʼ��1������  ���ݳ���λ 2����ʼ����json��
	USART3_dataLen = -1;		// json�ַ����ĳ���
	memset(USART3_jsonBuF, 0, sizeof(USART3_jsonBuF));
	USART3_jsonDataCount = 0;	//��ǰ���յ�  json �ַ�����
	uart3GetLen = 0;


}

void USART3_IRQHandler(void)                	//����3�жϷ������
{

	u8 temp;


	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
	{
		
		temp =USART3->DR;

			// �ж�Э�����ݵĿ�ͷ
	
		if (USART3_startGetMS == 0)
		{
			if (temp == '#')
			{
				USART3_startMS = '#';	
				uart3GetLen++;
			}
			else if ((temp == '!') && (USART3_startMS == '#') && (uart3GetLen == 1)) 
			{
				USART3_startGetMS = 1;// Э���־ ǰ���ֽ� ����ok	
			}else if (USART3_startMS == '#')
			{
				USART3StateTo0();
			}
		}
		else if (USART3_startGetMS == 1)// ���� Э������  �� json �ַ����ĳ���
		{
			if (USART3_dataLen == -1)
			{
				USART3_dataLen = temp*256;
			}else if(USART3_dataLen != -1)
			{
				USART3_dataLen = USART3_dataLen + temp;
				USART3_startGetMS =2;				
			}		
		}else if (USART3_startGetMS == 2)	// // ��ʼ����	Json ��
		{
			
			USART3_jsonBuF[USART3_jsonDataCount] = temp;
			USART3_jsonDataCount++;
			
			if (USART3_jsonDataCount == USART3_dataLen)  //  ���ν������
			{
				strcpy(USART3_jsonParseBuF, USART3_jsonBuF);
				USART3StateTo0();	
				PaserCar2_State();
			}
		}


	}

} 
	
//////////////////////////////////////////////////////////////////
/**************************ʵ�ֺ���**********************************************
*��    ��:		usart3����һ���ֽ�
*********************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);	
}


/**************************ʵ�ֺ���**********************************************
*��    ��:		usart3����һ���ַ���
*********************************************************************************/
void usart3_sendString(char *data,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)
	{
		USART3->DR = data[i];
		while((USART3->SR&0x40)==0);	
	}
	
}



//��ʼ��IO ����4 
//bound:������
void uart4_init(u32 bound){

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;


	//1.����ʱ�Ӻͺ�GPIOʱ��ʹ�ܡ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ��UART4ʱ��
	//2.�������Ÿ�����ӳ�� 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOC11����ΪUART4
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOC10����ΪUART4  
	//3.GPIO�˿�ģʽ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOC11��GPIOC10��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��GPIOC11����GPIOC10
	//4.���ڲ�����ʼ�������ò����ʣ��ֳ�����żУ��Ȳ���
	USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(UART4, &USART_InitStructure); //��ʼ������2����
	//5.��ʼ��NVIC
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	//6.�����ж�
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�����ж�  
	//7.ʹ�ܴ��� 
	USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ��� 

	
}




void UART4_IRQHandler(void)                	//����4�жϷ������
{

	u8 temp;

	
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) 
	{
		
		temp =UART4->DR;
		printf("\r\n33");
		
	
		

	}
} 
	
//////////////////////////////////////////////////////////////////
/**************************ʵ�ֺ���**********************************************
*��    ��:		uart4����һ���ֽ�
*********************************************************************************/
void uart4_send(u8 data)
{
	UART4->DR = data;
	while((UART4->SR&0x40)==0);	
}


/**************************ʵ�ֺ���**********************************************
*��    ��:		uart4����һ���ַ���
*********************************************************************************/
void uart4_sendString(char *data,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)
	{
		UART4->DR = data[i];
		while((UART4->SR&0x40)==0);	
	}
	
}




//��ʼ��IO ����5 
//bound:������
void uart5_init(u32 bound){

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;


	//1.����ʱ�Ӻͺ�GPIOʱ��ʹ�ܡ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOCʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIODʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//ʹ��UART5ʱ��
	//2.�������Ÿ�����ӳ�� 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //GPIOC12����ΪUART5
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //GPIOD2����ΪUART4  
	//3.GPIO�˿�ģʽ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //GPIOC12��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��GPIOC12

	//3.GPIO�˿�ģʽ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOD2��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��GPIOD2

	
	//4.���ڲ�����ʼ�������ò����ʣ��ֳ�����żУ��Ȳ���
	USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(UART5, &USART_InitStructure); //��ʼ������2����
	//5.��ʼ��NVIC
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	//6.�����ж�
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�����ж�  
	//7.ʹ�ܴ��� 
	USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ��� 

	
}




void UART5_IRQHandler(void)                	//����5�жϷ������
{

	u8 temp;

	
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) 
	{
		
		temp =UART5->DR;
		printf("\r\n55");
		
	
		

	}
} 
	
//////////////////////////////////////////////////////////////////
/**************************ʵ�ֺ���**********************************************
*��    ��:		uart4����һ���ֽ�
*********************************************************************************/
void uart5_send(u8 data)
{
	UART5->DR = data;
	while((UART5->SR&0x40)==0);	
}


/**************************ʵ�ֺ���**********************************************
*��    ��:		uart4����һ���ַ���
*********************************************************************************/
void uart5_sendString(char *data,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)
	{
		UART5->DR = data[i];
		while((UART5->SR&0x40)==0);	
	}
	
}


