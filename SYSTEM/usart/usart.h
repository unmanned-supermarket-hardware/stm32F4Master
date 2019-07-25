#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2011/6/14
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
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
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart1_init(u32 bound);




extern u8 UASRT2;



extern u16 USART2_RX_STA;   //��������״̬
extern u8 USART2_jsonParseBuF[300]; 


extern unsigned char const crc8_tab[256];
unsigned	char crc8_calculate(unsigned char * ucPtr, unsigned char ucLen) ;


void uart2_init(u32 bound); //����2��ʼ�� 
void usart2_send(u8 data);
void usart2_sendString(char *data,u8 len);





extern u8 USART3_jsonParseBuF[300];


void uart3_init(u32 bound); //����3��ʼ�� 
void usart3_send(u8 data);
void usart3_sendString(char *data,u8 len);


//����4
//TX:PC10   RX:PC11
void uart4_init(u32 bound); //����4��ʼ�� 
void uart4_send(u8 data);
void uart4_sendString(char *data,u8 len);

//����5
//RX: PD2  TX:  PC10
void uart5_init(u32 bound); //����4��ʼ�� 
void uart5_send(u8 data);
void uart5_sendString(char *data,u8 len);



#endif


