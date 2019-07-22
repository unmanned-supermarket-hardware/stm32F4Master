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

	if ((Car1_moveState == STATE_STOP) && (Car1_moveState == STATE_STOP))  // 两车 刚上电 、刚入弯道、刚出弯道
	{
		if (  (Car1_CorrectState  == 0)|| ( Car2_CorrectState == 0) )//姿态未校准
		{
			printf("\r\nwaiting  for correction");
			return ;


		}

		

	}


	
	if (( Car1_moveState > 1 )|| (Car2_moveState > 1) ) //  当前有  小车在  转弯
	{
		printf("\r\nwaiting for turing,  Car1_moveState :%d,  Car2_moveState:%d ",Car1_moveState ,Car2_moveState );
		return; //   等待转完
	}




	if ((Car1_FDistance <0.1 ) || (Car2_FDistance <= 0.1 ))  // 前方值 异常
	{
		printf("\r\n  FD error,Car1_FDistance:%f,  Car2_FDistance:%f",Car1_FDistance,Car2_FDistance);

		//前方距离不对，停止
		AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
		AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;
		return ;
	}

	
	if((Car1_FDistance <= TURING_DISTANCE ) || (Car2_FDistance <= TURING_DISTANCE )) // 有到达转弯点
	{
		
		if ((Car1_FDistance <= TURING_DISTANCE ) && (Car2_FDistance <= TURING_DISTANCE ) && (Car1_FDistance >= TURING_DISTANCE-TURING_DISTANCE_GAP ) && (Car2_FDistance >= TURING_DISTANCE-TURING_DISTANCE_GAP )) // 都到达转弯区间
		{
			if ( (Car1_CorrectState  == 0)|| ( Car2_CorrectState == 0) ) // 姿态不对
				{
					printf("\r\nwaiting  for correction");
					return ;
				}
			else
				{
					//	转弯的  方向 要看 在 超市哪边
					AiwacMasterSendOrderCar1(CAR_STOP , STATE_TURN_RIGHT) ;
					AiwacMasterSendOrderCar2(CAR_STOP , STATE_TURN_RIGHT) ;
					printf("\r\nturing");

				}

		}
		else  //调整到转弯区间
		{

		// 车1 的情况
			if (Car1_FDistance >= TURING_DISTANCE)
				{
				
					AiwacMasterSendOrderCar1(MIN_SPEED , STATE_STRAIGHT) ;
					printf("\r\ncar1  go on");
				}
			else if (Car1_FDistance <= TURING_DISTANCE-TURING_DISTANCE_GAP )
				{
					AiwacMasterSendOrderCar1(-MIN_SPEED , STATE_STRAIGHT) ;
					printf("\r\ncar1  go back");
				}
			
			else{
					AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
					printf("\r\ncar1  wait for turing order");
				}


			
		
		// 车2 的情况
			if (Car2_FDistance >= TURING_DISTANCE)
				{
				
					AiwacMasterSendOrderCar2(MIN_SPEED , STATE_STRAIGHT) ;
					printf("\r\ncar2  go on");
				}
			else if (Car2_FDistance <= TURING_DISTANCE-TURING_DISTANCE_GAP )
				{
					AiwacMasterSendOrderCar2(-MIN_SPEED , STATE_STRAIGHT) ;
					printf("\r\ncar2  go back");
				}
				else{
					AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;
					printf("\r\ncar2  wait for turing order");
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
						AiwacMasterSendOrderCar1(designFSpeed(Car1_FDistance)  +MIN_SPEED*2, STATE_STRAIGHT) ;
						AiwacMasterSendOrderCar2((designFSpeed(Car2_FDistance) ), STATE_STRAIGHT) ;
						printf("\r\n car1 needs to go fast");
					}
				else
					{
						// 发送 1车默认速度，2车 比默认快点
						AiwacMasterSendOrderCar1(designFSpeed(Car1_FDistance) , STATE_STRAIGHT) ;
						AiwacMasterSendOrderCar2(designFSpeed(Car2_FDistance)+ MIN_SPEED*2 , STATE_STRAIGHT) ;
						printf("\r\n car2 needs to go fast");
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
	double FSpeed = 30;		// 低速的速度 mm
	double FD_care = TURING_DISTANCE + 0.06 ;	// 前方警戒距离，需要  低速前进
	double FDSMax = FD_MAX_SPEED;  // 规定的最大  前方速度  mm


	
	if (FD>FD_care)  // 离危险距离较远
	{
		FSpeed = (FD - FD_care)*700 + FSpeed;
	}

	if (FSpeed > FDSMax)
	{
		FSpeed = FDSMax;
	}


	return FSpeed;

}



/**************************************************************************
函数功能：	根据前方距离 定小车前进速度
入口参数：	 前方
返回  值：		前方速度
**************************************************************************/
double  designFSpeed2(double FD, double FD_care,double iniTDistance)
{
	double FSpeed = 30;		// 低速的速度 mm

	double FDSMax = FD_MAX_SPEED;  // 规定的最大  前方速度  mm

	double startSpeed = 0;
	FD_care = FD_care + 0.06 ;	// 前方警戒距离，需要  低速前进



	if ((iniTDistance >=FD) || (iniTDistance -FD)*1000 <100)
		{
			startSpeed = (iniTDistance -FD)*1000*5+FSpeed;
	
		}

	
	if (FD>FD_care)  // 离危险距离较远
	{
		FSpeed = (FD - FD_care)*700 + FSpeed;
	}


	if (startSpeed >0)
		FSpeed = (FSpeed>startSpeed) ? startSpeed:FSpeed;

	
	if (FSpeed > FDSMax)
	{
		FSpeed = FDSMax;
	}


	return FSpeed;

}





/**************************************************************************
函数功能：	指定方向，运动到某方向剩余的距离处
入口参数：	 direction：方向    		needDistance：剩余的距离
返回  值：		无
**************************************************************************/
void goToLocation(int direction,double needDistance)
{

	
	// 保证两车  处于 停止状态
	AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
	AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;

	//起步阶段，需要两车平行起步
	goStartTogether(direction);

	// 运动到某方向的，指定地点
	goGoalPosition( direction,needDistance);

}


/**************************************************************************
函数功能：	两车起步的逻辑，在指定方向让后面的车 移动到前面，然后等矫正
入口参数： int direction  方向 
返回  值：		无
**************************************************************************/
void goStartTogether(int direction)
{
	double goalLocation = 0 ;
	double TogetherGap = 0.01;

	while(( Car1_moveState > 1 )|| (Car2_moveState > 1) ) //  当前有  小车在  转弯
	{
		printf("\r\nwaiting for turing,  Car1_moveState :%d,  Car2_moveState:%d ",Car1_moveState ,Car2_moveState );
		delay_ms(50); //   等待转完
	}


	AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
	AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;

	//等校正
	while (1) 
		{
			delay_ms(50);
			
			if (  (Car1_CorrectState  == 1) && ( Car2_CorrectState == 1) )//姿态校准  ok
			{
				printf("\r\n step1  goStartTogether correction ok ");
				break;
			}

		}

	
	//往前走
	if (direction == FRONT_DIRECTION)
		{
			goalLocation = ((Car1_FDistance >Car2_FDistance)? Car1_FDistance:Car2_FDistance);


				
			if (myabs_double(Car1_FDistance- goalLocation) <= TogetherGap)  // 车1是标准位置
				{

					while (1)
					{
						delay_ms(50);
						AiwacMasterSendOrderCar2(MIN_SPEED * 2, STATE_STRAIGHT) ;  
						
						if (myabs_double(Car2_FDistance- goalLocation) <= TogetherGap) // 车2 ok
							{
								break;
							}
					}
				}
			else
				{
				
					while (1)
					{
						delay_ms(50);
						AiwacMasterSendOrderCar1(MIN_SPEED * 2, STATE_STRAIGHT) ;	
						
						if (myabs_double(Car1_FDistance- goalLocation) <= TogetherGap) // 车1 ok
							{
								break;
							}
					}


				}


		}
	else  // 往后走
		{

		/*
			goalLocation = ((Car1_FDistance >Car2_FDistance)? Car1_FDistance:Car2_FDistance);
				
			if (myabs_double(Car1_FDistance- goalLocation) <= TogetherGap)  // 车1是标准位置
				{

					while (1)
					{
						delay_ms(50);
						AiwacMasterSendOrderCar2(MIN_SPEED , STATE_STRAIGHT) ;  
						
						if (myabs_double(Car2_FDistance- goalLocation) <= TogetherGap) // 车2 ok
							{
								break;
							}
					}
				}
			else
				{
				
					while (1)
					{
						delay_ms(50);
						AiwacMasterSendOrderCar1(MIN_SPEED , STATE_STRAIGHT) ;	
						
						if (myabs_double(Car1_FDistance- goalLocation) <= TogetherGap) // 车1 ok
							{
								break;
							}
					}


				}

		*/
		}



	//等校正
	while (1) 
		{
			delay_ms(50);
			
			if (  (Car1_CorrectState  == 1) && ( Car2_CorrectState == 1) )//姿态校准  ok
			{
				printf("\r\n step3  goStartTogether correction ok ");
				break;
			}

		}


}



/**************************************************************************
函数功能：	两车已经平行，在指定方向运动到目标地点
入口参数： int direction  方向 
返回  值：		无
**************************************************************************/
void goGoalPosition(int direction,double needDistance)
{
	double goalGAP = 0.015;   //m
	double iniTDistance = 0; // 起步距离，用于 渐进起步

	

	
	AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
	AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;


	//等校正
	while (1) 
		{
			delay_ms(50);
			
			if (  (Car1_CorrectState  == 1) && ( Car2_CorrectState == 1) )//姿态校准  ok
			{
				printf("\r\n step1  goGoalPosition correction ok ");
				break;
			}

		}


	
	// 运动到目标地点
	if (direction == FRONT_DIRECTION)
		{

			iniTDistance = (Car2_FDistance + Car1_FDistance)/2;

			while(1)
				{
					delay_ms(50);


					if ( ( myabs_double(Car1_FDistance- needDistance ) <  goalGAP ) || ( ( myabs_double(Car1_FDistance- needDistance ) <  goalGAP )) )
						{

							// 到达目的位置
							if ( ( myabs_double(Car1_FDistance- needDistance ) <  goalGAP ) && ( ( myabs_double(Car1_FDistance- needDistance ) <  goalGAP )) )
								{
									break;

								}

						
							// 车1 的情况
							if (Car1_FDistance >= needDistance + goalGAP)
								{
								
									AiwacMasterSendOrderCar1(MIN_SPEED , STATE_STRAIGHT) ;
									printf("\r\ncar1  go on");
								}
							else if (Car1_FDistance <= needDistance-goalGAP )
								{
									AiwacMasterSendOrderCar1(-MIN_SPEED , STATE_STRAIGHT) ;
									printf("\r\ncar1  go back");
								}
							
							else{
									AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
									printf("\r\ncar1  wait for turing order");
								}


							
						
						// 车2 的情况
							if (Car2_FDistance >= needDistance +goalGAP)
								{
								
									AiwacMasterSendOrderCar2(MIN_SPEED , STATE_STRAIGHT) ;
									printf("\r\ncar2  go on");
								}
							else if (Car2_FDistance <= needDistance-goalGAP )
								{
									AiwacMasterSendOrderCar2(-MIN_SPEED , STATE_STRAIGHT) ;
									printf("\r\ncar2  go back");
								}
								else{
									AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;
									printf("\r\ncar2  wait for turing order");
								}


						}
					else // 未到目标位置
						{
							if( myabs_double(Car1_FDistance - Car2_FDistance ) < goalGAP)  //  两车的	前进 距离ok
							{
								// 下发  继续 默认前进 
								AiwacMasterSendOrderCar1(designFSpeed2(Car1_FDistance, needDistance,iniTDistance) , STATE_STRAIGHT) ;
								AiwacMasterSendOrderCar2(designFSpeed2(Car2_FDistance, needDistance,iniTDistance) , STATE_STRAIGHT) ;
								printf("\r\ngo on straight");
							}
							else // 两车的  前进 距离  no
							{
								if (Car1_FDistance - Car2_FDistance >0)  // 1车在后 
									{
										// 发送 2车默认速度，1车 比默认快点
										AiwacMasterSendOrderCar1(designFSpeed2(Car1_FDistance, needDistance,iniTDistance)  +MIN_SPEED*2, STATE_STRAIGHT) ;
										AiwacMasterSendOrderCar2((designFSpeed2(Car2_FDistance, needDistance,iniTDistance) ), STATE_STRAIGHT) ;
										printf("\r\n car1 needs to go fast");
									}
								else
									{
										// 发送 1车默认速度，2车 比默认快点
										AiwacMasterSendOrderCar1(designFSpeed2(Car1_FDistance, needDistance,iniTDistance) , STATE_STRAIGHT) ;
										AiwacMasterSendOrderCar2(designFSpeed2(Car2_FDistance, needDistance,iniTDistance)+ MIN_SPEED*2 , STATE_STRAIGHT) ;
										printf("\r\n car2 needs to go fast");
									}
								
							}

						}
				}

	}
	else		//后面的值
		{

			;

		}


		//等校正
	while (1) 
		{
			delay_ms(50);
			
			if (  (Car1_CorrectState  == 1) && ( Car2_CorrectState == 1) )//姿态校准  ok
			{
				printf("\r\n step3  goGoalPosition correction ok ");
				break;
			}

		}


}


void sendTuringOrder(int Left_or_Right)
{

	//	转弯的  方向 要看 在 超市哪边
	AiwacMasterSendOrderCar1(CAR_STOP , Left_or_Right) ;
	AiwacMasterSendOrderCar2(CAR_STOP , Left_or_Right) ;
	printf("\r\nturing");
}


