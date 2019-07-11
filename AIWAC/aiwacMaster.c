#include "usart.h"	
#include "aiwacMaster.h"


int Car1_CorrectState = -1;
int Car1_FDistance = -1;
int Car1_moveState = -1;



int Car2_CorrectState = -1;
int Car2_FDistance = -1;
int Car2_moveState = -1;



/**************************************************************************
函数功能：		  解析小车1发来的  数据
入口参数：		无
返回  值：		无
**************************************************************************/
void PaserCar1_State(void)
{
	cJSON *root, *orderValue;  // 
	
	if (USART2_jsonParseBuF[0] == '-' ) //  还未收到运动命令
	{
		return;
	}
	
	root = cJSON_Parse(USART2_jsonParseBuF);

	orderValue = cJSON_GetObjectItem(root, "CorrectState");  //  自校准情况
	if (!orderValue) {
	    //printf("get name faild !\n");
	    //printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	Car1_CorrectState = orderValue->valueint;  //自校准情况 


	orderValue = cJSON_GetObjectItem(root, "FDistance");  //  前方距离
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	Car1_FDistance = orderValue->valuedouble;  // 前方距离


	orderValue = cJSON_GetObjectItem(root, "moveState");  //  小车的运动状态指令
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	Car1_moveState = orderValue->valueint;  // 小车的运动状态指令

	cJSON_Delete(root);


}


/**************************************************************************
函数功能：		  解析小车2发来的  数据
入口参数：		无
返回  值：		无
**************************************************************************/
void PaserCar2_State(void)
{
	cJSON *root, *orderValue;  // 
	
	if (USART3_jsonParseBuF[0] == '-' ) //  还未收到运动命令
	{
		return;
	}
	
	root = cJSON_Parse(USART3_jsonParseBuF);

	orderValue = cJSON_GetObjectItem(root, "CorrectState");  //  自校准情况
	if (!orderValue) {
	    //printf("get name faild !\n");
	    //printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	Car2_CorrectState = orderValue->valueint;  //自校准情况 


	orderValue = cJSON_GetObjectItem(root, "FDistance");  //  前方距离
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	Car2_FDistance = orderValue->valuedouble;  // 前方距离


	orderValue = cJSON_GetObjectItem(root, "moveState");  //  小车的运动状态指令
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	Car2_moveState = orderValue->valueint;  // 小车的运动状态指令

	cJSON_Delete(root);


}


/**************************************************************************
函数功能：绝对值函数
入口参数：double
返回  值：unsigned int
**************************************************************************/
double myabs_double(double a)
{ 		   
	  double temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}


/**************************************************************************
函数功能：	给小车1发 速度 和 小车的运动状态指令 
入口参数：	 X_V  : X轴的速度,前进速度			     moveState：小车的运动状态指令
返回  值：		无
**************************************************************************/
void AiwacMasterSendOrderCar1(double X_V, int moveState)
{
	cJSON *root;
	char *out;
	int len = 0;
	char str[300];
	
	char usartData[300];
	memset(usartData, 0, sizeof(usartData));

	usartData[0] = '#';
	usartData[1] = '!';

	root=cJSON_CreateObject();

	cJSON_AddNumberToObject(root,"from", 3);
	cJSON_AddNumberToObject(root,"to", 2);
	cJSON_AddNumberToObject(root,"msType", 1);
	cJSON_AddNumberToObject(root,"X_V", X_V);
	cJSON_AddNumberToObject(root,"moveState", moveState);


	out=cJSON_Print(root); 
	cJSON_Delete(root); 

	strcpy(str,out);
	//printf("%s\n",out); 
	len = strlen(str);
	
	usartData[2] = (u8)(len/256);

	//printf("\r\nusartData[2]:%d",usartData[2]);
	
	usartData[3] = (u8)(len%256);
	//printf("\r\nusartData[3]:%d",usartData[3]);
	strcat(usartData, str);
	//printf("\r\n  2Master  jsonLen:%d,usatLen:%d,usartData:%s,json:%s",strlen(str),usartData[2]*256+usartData[3],usartData,out);

	
// 需要打开
	usart2_sendString(usartData, strlen(usartData));
	myfree(out);

}



/**************************************************************************
函数功能：	给小车2发 速度 和 小车的运动状态指令 
入口参数：	 X_V  : X轴的速度,前进速度			     moveState：小车的运动状态指令
返回  值：		无
**************************************************************************/
void AiwacMasterSendOrderCar2(double X_V, int moveState)
{
	cJSON *root;
	char *out;
	int len = 0;
	char str[300];
	
	char usartData[300];
	memset(usartData, 0, sizeof(usartData));

	usartData[0] = '#';
	usartData[1] = '!';

	root=cJSON_CreateObject();

	cJSON_AddNumberToObject(root,"from", 3);
	cJSON_AddNumberToObject(root,"to", 2);
	cJSON_AddNumberToObject(root,"msType", 1);
	cJSON_AddNumberToObject(root,"X_V", X_V);
	cJSON_AddNumberToObject(root,"moveState", moveState);


	out=cJSON_Print(root); 
	cJSON_Delete(root); 

	strcpy(str,out);
	//printf("%s\n",out); 
	len = strlen(str);
	
	usartData[2] = (u8)(len/256);

	//printf("\r\nusartData[2]:%d",usartData[2]);
	
	usartData[3] = (u8)(len%256);
	//printf("\r\nusartData[3]:%d",usartData[3]);
	strcat(usartData, str);
	//printf("\r\n	2Master  jsonLen:%d,usatLen:%d,usartData:%s,json:%s",strlen(str),usartData[2]*256+usartData[3],usartData,out);

	
// 需要打开
	usart3_sendString(usartData, strlen(usartData));
	myfree(out);


}


/**************************************************************************
函数功能：	让两小车  联动
入口参数：	 无
返回  值：		无
**************************************************************************/
void Aiwac2CARTeamwork(void)
{
	if( (Car1_CorrectState == -1) || (Car2_CorrectState == -1)) // 还未完全收到两小车的  数据
	{
		return;
	}

	if (( Car1_moveState > 1 )|| (Car2_moveState > 1) ) //  当前有  小车在  转弯
	{
		return; //   等待转完
	}

	if(  (Car1_CorrectState  == 0)|| ( Car2_CorrectState == 0) )// 姿态未矫正
	{

		AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
		AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;
		return;  //等待姿态矫正

	}
	else  // 姿态 矫正ok
	{
		if((Car1_FDistance <= TURING_DISTANCE ) || (Car1_FDistance <= TURING_DISTANCE )) // 有到达转弯点
		{
			
			if ((Car1_FDistance <= TURING_DISTANCE ) && (Car1_FDistance <= TURING_DISTANCE )) // 都到达转弯点
			{

				//  转弯的  方向 要看 在 超市哪边
				AiwacMasterSendOrderCar1(CAR_STOP , STATE_TURN_RIGHT) ;
				AiwacMasterSendOrderCar2(CAR_STOP , STATE_TURN_RIGHT) ;
			}
			else
			{
				if (Car1_FDistance <= TURING_DISTANCE)
					{
						//车1  停止，车2 继续
						AiwacMasterSendOrderCar1(CAR_STOP , STATE_STRAIGHT) ;
						AiwacMasterSendOrderCar2(DEFUALT_SPEED , STATE_STRAIGHT) ;

					}
				else
					{
						//车2	停止，车1 继续
						AiwacMasterSendOrderCar1(DEFUALT_SPEED , STATE_STRAIGHT) ;
						AiwacMasterSendOrderCar2(CAR_STOP , STATE_STRAIGHT) ;

					}

			}
		}
		else  //  没有车到达 转弯点
		{
			if( myabs_double(Car1_FDistance - Car2_FDistance ) < FRONT_DISTANCEGAP)  //  两车的  前进 距离ok
				{
					// 下发  继续 默认前进 
					AiwacMasterSendOrderCar1(DEFUALT_SPEED , STATE_STRAIGHT) ;
					AiwacMasterSendOrderCar2(DEFUALT_SPEED , STATE_STRAIGHT) ;

				}
			else // 两车的  前进 距离  no
				{
					if (Car1_FDistance - Car2_FDistance >0)  // 1车在前 
						{
							// 发送 1车默认速度，2车 比默认快点
							AiwacMasterSendOrderCar1(DEFUALT_SPEED , STATE_STRAIGHT) ;
							AiwacMasterSendOrderCar2(ACC_SPEED , STATE_STRAIGHT) ;
						}
					else
						{
							// 发送 2车默认速度，1车 比默认快点
							AiwacMasterSendOrderCar1(ACC_SPEED, STATE_STRAIGHT) ;
							AiwacMasterSendOrderCar2(DEFUALT_SPEED , STATE_STRAIGHT) ;
						}
					
				}
		}

	}


}

