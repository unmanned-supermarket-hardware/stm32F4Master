#include "usart.h"	
#include "aiwacMaster.h"


int Car1_CorrectState = -1;
double Car1_FDistance = -1;
int Car1_moveState = -1;



int Car2_CorrectState = -1;
double Car2_FDistance = -1;
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
	    goto end;
	}
	Car1_CorrectState = orderValue->valueint;  //自校准情况 


	orderValue = cJSON_GetObjectItem(root, "FDistance");  //  前方距离
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	   goto end;
	}
	Car1_FDistance = orderValue->valuedouble;  // 前方距离


	orderValue = cJSON_GetObjectItem(root, "moveState");  //  小车的运动状态指令
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	   goto end;
	}
	Car1_moveState = orderValue->valueint;  // 小车的运动状态指令

end :
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
	    goto end;
	}
	Car2_CorrectState = orderValue->valueint;  //自校准情况 


	orderValue = cJSON_GetObjectItem(root, "FDistance");  //  前方距离
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	   goto end;
	}
	Car2_FDistance = orderValue->valuedouble;  // 前方距离


	orderValue = cJSON_GetObjectItem(root, "moveState");  //  小车的运动状态指令
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	   goto end;
	}
	Car2_moveState = orderValue->valueint;  // 小车的运动状态指令


end:
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
	u16 jsonSize;
	cJSON *root;
	char *strJson;
	char strSend[300];
	
	strSend[0] = '#';
	strSend[1] = '!';


	root=cJSON_CreateObject();


	cJSON_AddNumberToObject(root,"from", 3);
	cJSON_AddNumberToObject(root,"to", 2);
	cJSON_AddNumberToObject(root,"msType", 1);
	cJSON_AddNumberToObject(root,"X_V", X_V);
	cJSON_AddNumberToObject(root,"moveState", moveState);


	strJson=cJSON_Print(root); 
	cJSON_Delete(root); 
	
	jsonSize = strlen(strJson);

	strSend[2] = jsonSize >> 8;
	strSend[3] = jsonSize;

	strncpy(strSend+4,strJson,jsonSize);

	// 需要打开
	usart2_sendString(strSend,4 + jsonSize);
	myfree(strJson);


}



/**************************************************************************
函数功能：	给小车2发 速度 和 小车的运动状态指令 
入口参数：	 X_V  : X轴的速度,前进速度			     moveState：小车的运动状态指令
返回  值：		无
**************************************************************************/
void AiwacMasterSendOrderCar2(double X_V, int moveState)
{
	u16 jsonSize;
		cJSON *root;
		char *strJson;
		char strSend[300];
		
		strSend[0] = '#';
		strSend[1] = '!';
	
	
		root=cJSON_CreateObject();
	
	
		cJSON_AddNumberToObject(root,"from", 3);
		cJSON_AddNumberToObject(root,"to", 2);
		cJSON_AddNumberToObject(root,"msType", 1);
		cJSON_AddNumberToObject(root,"X_V", X_V);
		cJSON_AddNumberToObject(root,"moveState", moveState);
	
	
		strJson=cJSON_Print(root); 
		cJSON_Delete(root); 
		
		jsonSize = strlen(strJson);
	
		strSend[2] = jsonSize >> 8;
		strSend[3] = jsonSize;
	
		strncpy(strSend+4,strJson,jsonSize);
	
		// 需要打开
		usart3_sendString(strSend,4 + jsonSize);
		myfree(strJson);


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
		printf("\r\n waiting for data from cars");
		return;
	}




	
	if (( Car1_moveState > 1 )|| (Car2_moveState > 1) ) //  当前有  小车在  转弯
	{
		printf("\r\nwaiting for turing,  Car1_moveState :%d,  Car2_moveState:%d ",Car1_moveState ,Car2_moveState );
		return; //   等待转完
	}

	if(  (Car1_CorrectState  == 0)|| ( Car2_CorrectState == 0) )// 姿态未矫正
	{

		AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
		AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;
		printf("\r\nwaiting  for correction");
		return;  //等待姿态矫正

	}
	else  // 姿态 矫正ok
	{


		if ((Car1_FDistance <0.08 ) || (Car2_FDistance <= 0.08 ))  // 前方值 异常
		{
			printf("\r\n  FD error,Car1_FDistance:%f,  Car2_FDistance:%f",Car1_FDistance,Car2_FDistance);
			return ;
		}

		
		if((Car1_FDistance <= TURING_DISTANCE ) || (Car2_FDistance <= TURING_DISTANCE )) // 有到达转弯点
		{
			
			if ((Car1_FDistance <= TURING_DISTANCE ) && (Car2_FDistance <= TURING_DISTANCE )) // 都到达转弯点
			{

				//  转弯的  方向 要看 在 超市哪边
				AiwacMasterSendOrderCar1(CAR_STOP , STATE_TURN_RIGHT) ;
				AiwacMasterSendOrderCar2(CAR_STOP , STATE_TURN_RIGHT) ;
				printf("\r\nturing");
			}
			else
			{
				if (Car1_FDistance <= TURING_DISTANCE)
					{
						//车1  停止，车2 继续
						AiwacMasterSendOrderCar1(CAR_STOP , STATE_STRAIGHT) ;
						AiwacMasterSendOrderCar2(MIN_SPEED , STATE_STRAIGHT) ;
						printf("\r\ncar1 stop, car2 go on");
					}
				else
					{
						//车2	停止，车1 继续
						AiwacMasterSendOrderCar1(MIN_SPEED , STATE_STRAIGHT) ;
						AiwacMasterSendOrderCar2(CAR_STOP , STATE_STRAIGHT) ;
						printf("\r\ncar2 stop ,car1 go on");

					}

			}
		}
		else  //  没有车到达 转弯点
		{
			if( myabs_double(Car1_FDistance - Car2_FDistance ) < FRONT_DISTANCEGAP)  //  两车的  前进 距离ok
				{
					// 下发  继续 默认前进 
					AiwacMasterSendOrderCar1(designFSpeed(Car1_FDistance) , STATE_STRAIGHT) ;
					AiwacMasterSendOrderCar2(designFSpeed(Car2_FDistance) , STATE_STRAIGHT) ;
					printf("\r\ngo on straight");
				}
			else // 两车的  前进 距离  no
				{
					if (Car1_FDistance - Car2_FDistance >0)  // 1车在后 
						{
							// 发送 2车默认速度，1车 比默认快点
							AiwacMasterSendOrderCar1(designFSpeed(Car1_FDistance)  +MIN_SPEED, STATE_STRAIGHT) ;
							AiwacMasterSendOrderCar2((designFSpeed(Car2_FDistance) ), STATE_STRAIGHT) ;
							printf("\r\n car1 needs to go fast");
						}
					else
						{
							// 发送 1车默认速度，2车 比默认快点
							AiwacMasterSendOrderCar1(designFSpeed(Car1_FDistance) , STATE_STRAIGHT) ;
							AiwacMasterSendOrderCar2(designFSpeed(Car2_FDistance)+ MIN_SPEED , STATE_STRAIGHT) ;
							printf("\r\n car2 needs to go fast");
						}
					
				}
		}

	}


}



/**************************************************************************
函数功能：	根据前方距离 定小车前进速度
入口参数：	 前方
返回  值：		前方速度
**************************************************************************/
double  designFSpeed(double FD)
{
	double FSpeed = 30;		// 低速的速度
	double FD_care = 10;	// 前方警戒距离，需要  低速前进
	double FDSMax = 40;  // 规定的最大  前方速度
	
	if (FD>10)
	{
		FSpeed = (FD - FD_care)*2 + FSpeed;
	}

	if (FSpeed > FDSMax)
	{
		FSpeed = FDSMax;
	}


	return FSpeed;

}

