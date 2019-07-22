#include "usart.h"	
#include "aiwacMaster.h"


int Car1_CorrectState = -1;
double Car1_FDistance = -1;
int Car1_moveState = -1;



int Car2_CorrectState = -1;
double Car2_FDistance = -1;
int Car2_moveState = -1;



/**************************************************************************
�������ܣ�		  ����С��1������  ����
��ڲ�����		��
����  ֵ��		��
**************************************************************************/
void PaserCar1_State(void)
{
	cJSON *root, *orderValue;  // 
	
	if (USART2_jsonParseBuF[0] == '-' ) //  ��δ�յ��˶�����
	{
		return;
	}
	
	root = cJSON_Parse(USART2_jsonParseBuF);

	orderValue = cJSON_GetObjectItem(root, "CorrectState");  //  ��У׼���
	if (!orderValue) {
	    //printf("get name faild !\n");
	    //printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	    goto end;
	}
	Car1_CorrectState = orderValue->valueint;  //��У׼��� 


	orderValue = cJSON_GetObjectItem(root, "FDistance");  //  ǰ������
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	   goto end;
	}
	Car1_FDistance = orderValue->valuedouble;  // ǰ������


	orderValue = cJSON_GetObjectItem(root, "moveState");  //  С�����˶�״ָ̬��
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	   goto end;
	}
	Car1_moveState = orderValue->valueint;  // С�����˶�״ָ̬��

end :
	cJSON_Delete(root);


}


/**************************************************************************
�������ܣ�		  ����С��2������  ����
��ڲ�����		��
����  ֵ��		��
**************************************************************************/
void PaserCar2_State(void)
{
	cJSON *root, *orderValue;  // 
	
	if (USART3_jsonParseBuF[0] == '-' ) //  ��δ�յ��˶�����
	{
		return;
	}
	
	root = cJSON_Parse(USART3_jsonParseBuF);

	orderValue = cJSON_GetObjectItem(root, "CorrectState");  //  ��У׼���
	if (!orderValue) {
	    //printf("get name faild !\n");
	    //printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	    goto end;
	}
	Car2_CorrectState = orderValue->valueint;  //��У׼��� 


	orderValue = cJSON_GetObjectItem(root, "FDistance");  //  ǰ������
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	   goto end;
	}
	Car2_FDistance = orderValue->valuedouble;  // ǰ������


	orderValue = cJSON_GetObjectItem(root, "moveState");  //  С�����˶�״ָ̬��
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	   goto end;
	}
	Car2_moveState = orderValue->valueint;  // С�����˶�״ָ̬��


end:
	cJSON_Delete(root);


}


/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����double
����  ֵ��unsigned int
**************************************************************************/
double myabs_double(double a)
{ 		   
	  double temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}


/**************************************************************************
�������ܣ�	��С��1�� �ٶ� �� С�����˶�״ָ̬�� 
��ڲ�����	 X_V  : X����ٶ�,ǰ���ٶ�			     moveState��С�����˶�״ָ̬��
����  ֵ��		��
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

	// ��Ҫ��
	usart2_sendString(strSend,4 + jsonSize);
	myfree(strJson);


}



/**************************************************************************
�������ܣ�	��С��2�� �ٶ� �� С�����˶�״ָ̬�� 
��ڲ�����	 X_V  : X����ٶ�,ǰ���ٶ�			     moveState��С�����˶�״ָ̬��
����  ֵ��		��
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
	
		// ��Ҫ��
		usart3_sendString(strSend,4 + jsonSize);
		myfree(strJson);


}


/**************************************************************************
�������ܣ�	����С��  ����
��ڲ�����	 ��
����  ֵ��		��
**************************************************************************/
void Aiwac2CARTeamwork(void)
{
	if( (Car1_CorrectState == -1) || (Car2_CorrectState == -1)) // ��δ��ȫ�յ���С����  ����
	{
		printf("\r\n waiting for data from cars");
		return;
	}

	if ((Car1_moveState == STATE_STOP) && (Car1_moveState == STATE_STOP))  // ���� ���ϵ� ������������ճ����
	{
		if (  (Car1_CorrectState  == 0)|| ( Car2_CorrectState == 0) )//��̬δУ׼
		{
			printf("\r\nwaiting  for correction");
			return ;


		}

		

	}


	
	if (( Car1_moveState > 1 )|| (Car2_moveState > 1) ) //  ��ǰ��  С����  ת��
	{
		printf("\r\nwaiting for turing,  Car1_moveState :%d,  Car2_moveState:%d ",Car1_moveState ,Car2_moveState );
		return; //   �ȴ�ת��
	}




	if ((Car1_FDistance <0.1 ) || (Car2_FDistance <= 0.1 ))  // ǰ��ֵ �쳣
	{
		printf("\r\n  FD error,Car1_FDistance:%f,  Car2_FDistance:%f",Car1_FDistance,Car2_FDistance);

		//ǰ�����벻�ԣ�ֹͣ
		AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
		AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;
		return ;
	}

	
	if((Car1_FDistance <= TURING_DISTANCE ) || (Car2_FDistance <= TURING_DISTANCE )) // �е���ת���
	{
		
		if ((Car1_FDistance <= TURING_DISTANCE ) && (Car2_FDistance <= TURING_DISTANCE ) && (Car1_FDistance >= TURING_DISTANCE-TURING_DISTANCE_GAP ) && (Car2_FDistance >= TURING_DISTANCE-TURING_DISTANCE_GAP )) // ������ת������
		{
			if ( (Car1_CorrectState  == 0)|| ( Car2_CorrectState == 0) ) // ��̬����
				{
					printf("\r\nwaiting  for correction");
					return ;
				}
			else
				{
					//	ת���  ���� Ҫ�� �� �����ı�
					AiwacMasterSendOrderCar1(CAR_STOP , STATE_TURN_RIGHT) ;
					AiwacMasterSendOrderCar2(CAR_STOP , STATE_TURN_RIGHT) ;
					printf("\r\nturing");

				}

		}
		else  //������ת������
		{

		// ��1 �����
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


			
		
		// ��2 �����
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
	else  //  û�г����� ת���
	{
		if( myabs_double(Car1_FDistance - Car2_FDistance ) < FRONT_DISTANCEGAP)  //  ������  ǰ�� ����ok
			{
				// �·�  ���� Ĭ��ǰ�� 
				AiwacMasterSendOrderCar1(designFSpeed(Car1_FDistance) , STATE_STRAIGHT) ;
				AiwacMasterSendOrderCar2(designFSpeed(Car2_FDistance) , STATE_STRAIGHT) ;
				printf("\r\ngo on straight");
			}
		else // ������  ǰ�� ����  no
			{
				if (Car1_FDistance - Car2_FDistance >0)  // 1���ں� 
					{
						// ���� 2��Ĭ���ٶȣ�1�� ��Ĭ�Ͽ��
						AiwacMasterSendOrderCar1(designFSpeed(Car1_FDistance)  +MIN_SPEED*2, STATE_STRAIGHT) ;
						AiwacMasterSendOrderCar2((designFSpeed(Car2_FDistance) ), STATE_STRAIGHT) ;
						printf("\r\n car1 needs to go fast");
					}
				else
					{
						// ���� 1��Ĭ���ٶȣ�2�� ��Ĭ�Ͽ��
						AiwacMasterSendOrderCar1(designFSpeed(Car1_FDistance) , STATE_STRAIGHT) ;
						AiwacMasterSendOrderCar2(designFSpeed(Car2_FDistance)+ MIN_SPEED*2 , STATE_STRAIGHT) ;
						printf("\r\n car2 needs to go fast");
					}
				
			}
	}



}



/**************************************************************************
�������ܣ�	����ǰ������ ��С��ǰ���ٶ�
��ڲ�����	 ǰ��
����  ֵ��		ǰ���ٶ�
**************************************************************************/
double  designFSpeed(double FD)
{
	double FSpeed = 30;		// ���ٵ��ٶ� mm
	double FD_care = TURING_DISTANCE + 0.06 ;	// ǰ��������룬��Ҫ  ����ǰ��
	double FDSMax = FD_MAX_SPEED;  // �涨�����  ǰ���ٶ�  mm


	
	if (FD>FD_care)  // ��Σ�վ����Զ
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
�������ܣ�	����ǰ������ ��С��ǰ���ٶ�
��ڲ�����	 ǰ��
����  ֵ��		ǰ���ٶ�
**************************************************************************/
double  designFSpeed2(double FD, double FD_care,double iniTDistance)
{
	double FSpeed = 30;		// ���ٵ��ٶ� mm

	double FDSMax = FD_MAX_SPEED;  // �涨�����  ǰ���ٶ�  mm

	double startSpeed = 0;
	FD_care = FD_care + 0.06 ;	// ǰ��������룬��Ҫ  ����ǰ��



	if ((iniTDistance >=FD) || (iniTDistance -FD)*1000 <100)
		{
			startSpeed = (iniTDistance -FD)*1000*5+FSpeed;
	
		}

	
	if (FD>FD_care)  // ��Σ�վ����Զ
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
�������ܣ�	ָ�������˶���ĳ����ʣ��ľ��봦
��ڲ�����	 direction������    		needDistance��ʣ��ľ���
����  ֵ��		��
**************************************************************************/
void goToLocation(int direction,double needDistance)
{

	
	// ��֤����  ���� ֹͣ״̬
	AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
	AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;

	//�𲽽׶Σ���Ҫ����ƽ����
	goStartTogether(direction);

	// �˶���ĳ����ģ�ָ���ص�
	goGoalPosition( direction,needDistance);

}


/**************************************************************************
�������ܣ�	�����𲽵��߼�����ָ�������ú���ĳ� �ƶ���ǰ�棬Ȼ��Ƚ���
��ڲ����� int direction  ���� 
����  ֵ��		��
**************************************************************************/
void goStartTogether(int direction)
{
	double goalLocation = 0 ;
	double TogetherGap = 0.01;

	while(( Car1_moveState > 1 )|| (Car2_moveState > 1) ) //  ��ǰ��  С����  ת��
	{
		printf("\r\nwaiting for turing,  Car1_moveState :%d,  Car2_moveState:%d ",Car1_moveState ,Car2_moveState );
		delay_ms(50); //   �ȴ�ת��
	}


	AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
	AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;

	//��У��
	while (1) 
		{
			delay_ms(50);
			
			if (  (Car1_CorrectState  == 1) && ( Car2_CorrectState == 1) )//��̬У׼  ok
			{
				printf("\r\n step1  goStartTogether correction ok ");
				break;
			}

		}

	
	//��ǰ��
	if (direction == FRONT_DIRECTION)
		{
			goalLocation = ((Car1_FDistance >Car2_FDistance)? Car1_FDistance:Car2_FDistance);


				
			if (myabs_double(Car1_FDistance- goalLocation) <= TogetherGap)  // ��1�Ǳ�׼λ��
				{

					while (1)
					{
						delay_ms(50);
						AiwacMasterSendOrderCar2(MIN_SPEED * 2, STATE_STRAIGHT) ;  
						
						if (myabs_double(Car2_FDistance- goalLocation) <= TogetherGap) // ��2 ok
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
						
						if (myabs_double(Car1_FDistance- goalLocation) <= TogetherGap) // ��1 ok
							{
								break;
							}
					}


				}


		}
	else  // ������
		{

		/*
			goalLocation = ((Car1_FDistance >Car2_FDistance)? Car1_FDistance:Car2_FDistance);
				
			if (myabs_double(Car1_FDistance- goalLocation) <= TogetherGap)  // ��1�Ǳ�׼λ��
				{

					while (1)
					{
						delay_ms(50);
						AiwacMasterSendOrderCar2(MIN_SPEED , STATE_STRAIGHT) ;  
						
						if (myabs_double(Car2_FDistance- goalLocation) <= TogetherGap) // ��2 ok
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
						
						if (myabs_double(Car1_FDistance- goalLocation) <= TogetherGap) // ��1 ok
							{
								break;
							}
					}


				}

		*/
		}



	//��У��
	while (1) 
		{
			delay_ms(50);
			
			if (  (Car1_CorrectState  == 1) && ( Car2_CorrectState == 1) )//��̬У׼  ok
			{
				printf("\r\n step3  goStartTogether correction ok ");
				break;
			}

		}


}



/**************************************************************************
�������ܣ�	�����Ѿ�ƽ�У���ָ�������˶���Ŀ��ص�
��ڲ����� int direction  ���� 
����  ֵ��		��
**************************************************************************/
void goGoalPosition(int direction,double needDistance)
{
	double goalGAP = 0.015;   //m
	double iniTDistance = 0; // �𲽾��룬���� ������

	

	
	AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
	AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;


	//��У��
	while (1) 
		{
			delay_ms(50);
			
			if (  (Car1_CorrectState  == 1) && ( Car2_CorrectState == 1) )//��̬У׼  ok
			{
				printf("\r\n step1  goGoalPosition correction ok ");
				break;
			}

		}


	
	// �˶���Ŀ��ص�
	if (direction == FRONT_DIRECTION)
		{

			iniTDistance = (Car2_FDistance + Car1_FDistance)/2;

			while(1)
				{
					delay_ms(50);


					if ( ( myabs_double(Car1_FDistance- needDistance ) <  goalGAP ) || ( ( myabs_double(Car1_FDistance- needDistance ) <  goalGAP )) )
						{

							// ����Ŀ��λ��
							if ( ( myabs_double(Car1_FDistance- needDistance ) <  goalGAP ) && ( ( myabs_double(Car1_FDistance- needDistance ) <  goalGAP )) )
								{
									break;

								}

						
							// ��1 �����
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


							
						
						// ��2 �����
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
					else // δ��Ŀ��λ��
						{
							if( myabs_double(Car1_FDistance - Car2_FDistance ) < goalGAP)  //  ������	ǰ�� ����ok
							{
								// �·�  ���� Ĭ��ǰ�� 
								AiwacMasterSendOrderCar1(designFSpeed2(Car1_FDistance, needDistance,iniTDistance) , STATE_STRAIGHT) ;
								AiwacMasterSendOrderCar2(designFSpeed2(Car2_FDistance, needDistance,iniTDistance) , STATE_STRAIGHT) ;
								printf("\r\ngo on straight");
							}
							else // ������  ǰ�� ����  no
							{
								if (Car1_FDistance - Car2_FDistance >0)  // 1���ں� 
									{
										// ���� 2��Ĭ���ٶȣ�1�� ��Ĭ�Ͽ��
										AiwacMasterSendOrderCar1(designFSpeed2(Car1_FDistance, needDistance,iniTDistance)  +MIN_SPEED*2, STATE_STRAIGHT) ;
										AiwacMasterSendOrderCar2((designFSpeed2(Car2_FDistance, needDistance,iniTDistance) ), STATE_STRAIGHT) ;
										printf("\r\n car1 needs to go fast");
									}
								else
									{
										// ���� 1��Ĭ���ٶȣ�2�� ��Ĭ�Ͽ��
										AiwacMasterSendOrderCar1(designFSpeed2(Car1_FDistance, needDistance,iniTDistance) , STATE_STRAIGHT) ;
										AiwacMasterSendOrderCar2(designFSpeed2(Car2_FDistance, needDistance,iniTDistance)+ MIN_SPEED*2 , STATE_STRAIGHT) ;
										printf("\r\n car2 needs to go fast");
									}
								
							}

						}
				}

	}
	else		//�����ֵ
		{

			;

		}


		//��У��
	while (1) 
		{
			delay_ms(50);
			
			if (  (Car1_CorrectState  == 1) && ( Car2_CorrectState == 1) )//��̬У׼  ok
			{
				printf("\r\n step3  goGoalPosition correction ok ");
				break;
			}

		}


}


void sendTuringOrder(int Left_or_Right)
{

	//	ת���  ���� Ҫ�� �� �����ı�
	AiwacMasterSendOrderCar1(CAR_STOP , Left_or_Right) ;
	AiwacMasterSendOrderCar2(CAR_STOP , Left_or_Right) ;
	printf("\r\nturing");
}


