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




	
	if (( Car1_moveState > 1 )|| (Car2_moveState > 1) ) //  ��ǰ��  С����  ת��
	{
		printf("\r\nwaiting for turing,  Car1_moveState :%d,  Car2_moveState:%d ",Car1_moveState ,Car2_moveState );
		return; //   �ȴ�ת��
	}

	if(  (Car1_CorrectState  == 0)|| ( Car2_CorrectState == 0) )// ��̬δ����
	{

		AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
		AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;
		printf("\r\nwaiting  for correction");
		return;  //�ȴ���̬����

	}
	else  // ��̬ ����ok
	{


		if ((Car1_FDistance <0.08 ) || (Car2_FDistance <= 0.08 ))  // ǰ��ֵ �쳣
		{
			printf("\r\n  FD error,Car1_FDistance:%f,  Car2_FDistance:%f",Car1_FDistance,Car2_FDistance);
			return ;
		}

		
		if((Car1_FDistance <= TURING_DISTANCE ) || (Car2_FDistance <= TURING_DISTANCE )) // �е���ת���
		{
			
			if ((Car1_FDistance <= TURING_DISTANCE ) && (Car2_FDistance <= TURING_DISTANCE )) // ������ת���
			{

				//  ת���  ���� Ҫ�� �� �����ı�
				AiwacMasterSendOrderCar1(CAR_STOP , STATE_TURN_RIGHT) ;
				AiwacMasterSendOrderCar2(CAR_STOP , STATE_TURN_RIGHT) ;
				printf("\r\nturing");
			}
			else
			{
				if (Car1_FDistance <= TURING_DISTANCE)
					{
						//��1  ֹͣ����2 ����
						AiwacMasterSendOrderCar1(CAR_STOP , STATE_STRAIGHT) ;
						AiwacMasterSendOrderCar2(MIN_SPEED , STATE_STRAIGHT) ;
						printf("\r\ncar1 stop, car2 go on");
					}
				else
					{
						//��2	ֹͣ����1 ����
						AiwacMasterSendOrderCar1(MIN_SPEED , STATE_STRAIGHT) ;
						AiwacMasterSendOrderCar2(CAR_STOP , STATE_STRAIGHT) ;
						printf("\r\ncar2 stop ,car1 go on");

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
							AiwacMasterSendOrderCar1(designFSpeed(Car1_FDistance)  +MIN_SPEED, STATE_STRAIGHT) ;
							AiwacMasterSendOrderCar2((designFSpeed(Car2_FDistance) ), STATE_STRAIGHT) ;
							printf("\r\n car1 needs to go fast");
						}
					else
						{
							// ���� 1��Ĭ���ٶȣ�2�� ��Ĭ�Ͽ��
							AiwacMasterSendOrderCar1(designFSpeed(Car1_FDistance) , STATE_STRAIGHT) ;
							AiwacMasterSendOrderCar2(designFSpeed(Car2_FDistance)+ MIN_SPEED , STATE_STRAIGHT) ;
							printf("\r\n car2 needs to go fast");
						}
					
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
	double FSpeed = 30;		// ���ٵ��ٶ�
	double FD_care = 10;	// ǰ��������룬��Ҫ  ����ǰ��
	double FDSMax = 40;  // �涨�����  ǰ���ٶ�
	
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

