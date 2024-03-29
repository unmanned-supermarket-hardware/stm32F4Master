#include "sys.h"
#include "usart.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F4探索者开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/6/10
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	




//串口2中断服务程序
//串口发送缓存区 

u16 USART2_RX_STA = 0;


//初始化IO 串口2 
//bound:波特率
void uart2_init(u32 bound){

NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;


//1.串口时钟和和GPIO时钟使能。
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
//2.设置引脚复用器映射 
GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2
GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2  
//3.GPIO端口模式设置
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2和GPIOA3初始化
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化GPIOA2，和GPIOA3
//4.串口参数初始化：设置波特率，字长，奇偶校验等参数
USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
USART_Init(USART2, &USART_InitStructure); //初始化串口2参数
//5.初始化NVIC
NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //子优先级3
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
//6.开启中断
USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断  
//7.使能串口 
USART_Cmd(USART2, ENABLE);                    //使能串口 

	
}

int	USART2_dataLen = -1;		// json字符串的长度
u8 USART2_jsonBuF[500]; 			// 在中断的时候 存储接收的json 字符串
int USART2_jsonDataCount = 0;  //当前接收的  json 字符串数
u8 zone_1_car1_jsonParseBuF[500]; 			//解析的时候用 存储接收的json 字符串，防止跟中断共用一个  字符串 读写 出问题
u8 zone_1_car2_jsonParseBuF[500]; 			//解析的时候用 存储接收的json 字符串，防止跟中断共用一个  字符串 读写 出问题

int uart2ByteNum = 0;  // 串口2 接收符合协议的字节数目
int MSFrom = 0;  // 数据来自哪

void USART2StateTo0(void)
{
	// 恢复初始化
	USART2_dataLen = -1;		// json字符串的长度
	memset(USART2_jsonBuF, 0, sizeof(USART2_jsonBuF));
	USART2_jsonDataCount = 0;	//当前接收的  json 字符串数
	uart2ByteNum = 0;
	MSFrom = 0;
}


/**************************实现函数**********************************************
*功    能:		usart2连zigBee，与小车 和取货单元通信
*********************************************************************************/

void USART2_IRQHandler(void)                	//串口2中断服务程序
{

	u8 temp;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
		
		temp =USART2->DR;

		// 第一个字节
			if (uart2ByteNum == 0)
			{
				if (temp == '#')
				{
					uart2ByteNum++;
					//printf("\r\n get	1!!");
				}
					return ;	
			}
		
			// 第二个字节
			if (uart2ByteNum == 1)
			{
				if (temp == '!')
				{
					uart2ByteNum++;
					//printf("\r\n get	2!!");
					return ;	
				}
				else
				{
					uart2ByteNum = 0;
					//printf("\r\n get	2	FAILED!!");
					return ;
				}
						
			}
		
			// 接收 TO
			// 第三个字节
			if (uart2ByteNum == 2)
				{
					if (temp == MYSELF_ROLE)
					{
						uart2ByteNum++;
						//printf("\r\n get	my data!!");
						return ;
					}
					else
					{
						uart2ByteNum = 0;
						//printf("\r\n get	3	FAILED!!");
						return ;
					}
		
				}
		
			// 接收 FROM
			// 第四个字节
			if (uart2ByteNum == 3)
				{
					if (temp < SYS_MAX_FLAG)
					{
						MSFrom = temp;
						uart2ByteNum++;
						return ;
					}
					else
					{
						uart2ByteNum = 0;
						return ;
					}
		
				}
		
		
			// 接收 json Len	  高字节
			// 第五个字节
			if (uart2ByteNum == 4)
				{
					USART2_dataLen = temp*256;
					uart2ByteNum++;
					return ;
				}
			
			// 接收 json Len	  低字节
			// 第六个字节 
			if (uart2ByteNum == 5)
				{
					USART2_dataLen = USART2_dataLen + temp;
					uart2ByteNum++;
					//printf("\r\n get	6!!");
					return ;
				}
		
			// 开始接收
			if (uart2ByteNum == 6)
			{
				USART2_jsonBuF[USART2_jsonDataCount] = temp;
				USART2_jsonDataCount++;
				
				if (USART2_jsonDataCount == USART2_dataLen)  //  本次接收完毕
				{

					// 针对不同的角色  进行 操作
					if (MSFrom == ZONE_1_CAR_1)
						{
							strcpy(zone_1_car1_jsonParseBuF, USART2_jsonBuF);
						}

					if (MSFrom == ZONE_1_CAR_2)
						{
							strcpy(zone_1_car2_jsonParseBuF, USART2_jsonBuF);
						}
					
					//printf("\r\zone_1_car1_jsonParseBuF:%s",zone_1_car1_jsonParseBuF);
					USART2StateTo0();	
				}
		
				return ;
			}

	}

} 
	
//////////////////////////////////////////////////////////////////
/**************************实现函数**********************************************
*功    能:		usart2发送一个字节
*********************************************************************************/
void usart2_send(u8 data)
{
	USART2->DR = data;
	while((USART2->SR&0x40)==0);	
}



/**************************实现函数**********************************************
*功    能:		用Zigbee发数据
*********************************************************************************/
void zigBee_sendString(char *data,u8 len)
{
	usart2_sendString(data, len);
}

/**************************实现函数**********************************************
*功    能:		usart2发送一个字符串
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
		usart3   函数
*********************************************************************************/

//串口3中断服务程序
//串口发送缓存区 
    
u16 USART3_RX_STA = 0;


//初始化IO 串口3 
//bound:波特率
void uart3_init(u32 bound){

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;


	//1.串口时钟和和GPIO时钟使能。
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
	//2.设置引脚复用器映射 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10复用为USART3  
	//3.GPIO端口模式设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOB11和GPIOB10初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化GPIOB11，和GPIOB10
	//4.串口参数初始化：设置波特率，字长，奇偶校验等参数
	USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口2参数
	//5.初始化NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
	//6.开启中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断  
	//7.使能串口 
	USART_Cmd(USART3, ENABLE);                    //使能串口 

	
}

/*
u8 UASRT3 = 0;
char USART3_startMS = '+';	//保存协议前两字节			#！
u8 USART3_startGetMS = 0;		// 0：还不能开始，1：接收  数据长度位 2：开始接收json串
int	USART3_dataLen = -1;		// json字符串的长度
u8 USART3_jsonBuF[1000]; 			// 在中断的时候 存储接收的json 字符串
int USART3_jsonDataCount = 0;  //当前接收的  json 字符串数

int uart3GetLen = 0; 


void  USART3StateTo0(void )
{
	// 恢复初始化
	USART3_startMS = '+';	//保存协议前两字节			#！
	USART3_startGetMS = 0;	// 0：还不能开始，1：接收  数据长度位 2：开始接收json串
	USART3_dataLen = -1;		// json字符串的长度
	memset(USART3_jsonBuF, 0, sizeof(USART3_jsonBuF));
	USART3_jsonDataCount = 0;	//当前接收的  json 字符串数
	uart3GetLen = 0;


}
*/
void USART3_IRQHandler(void)                	//串口3中断服务程序
{

	u8 temp;


	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
	{
		
		temp =USART3->DR;

			// 判断协议数据的开头
	/*
		if (USART3_startGetMS == 0)
		{
			if (temp == '#')
			{
				USART3_startMS = '#';	
				uart3GetLen++;
			}
			else if ((temp == '!') && (USART3_startMS == '#') && (uart3GetLen == 1)) 
			{
				USART3_startGetMS = 1;// 协议标志 前两字节 接收ok	
			}else if (USART3_startMS == '#')
			{
				USART3StateTo0();
			}
		}
		else if (USART3_startGetMS == 1)// 接收 协议数据  内 json 字符串的长度
		{
			if (USART3_dataLen == -1)
			{
				USART3_dataLen = temp*256;
			}else if(USART3_dataLen != -1)
			{
				USART3_dataLen = USART3_dataLen + temp;
				USART3_startGetMS =2;				
			}		
		}else if (USART3_startGetMS == 2)	// // 开始接收	Json 串
		{
			
			USART3_jsonBuF[USART3_jsonDataCount] = temp;
			USART3_jsonDataCount++;
			
			if (USART3_jsonDataCount == USART3_dataLen)  //  本次接收完毕
			{
				strcpy(USART3_jsonParseBuF, USART3_jsonBuF);
				USART3StateTo0();	
			}
		}
*/

	}

} 
	
//////////////////////////////////////////////////////////////////
/**************************实现函数**********************************************
*功    能:		usart3发送一个字节
*********************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);	
}


/**************************实现函数**********************************************
*功    能:		usart3发送一个字符串
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




