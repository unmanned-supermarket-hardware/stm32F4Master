#include "usart.h"	
#include "aiwacMaster.h"


int Car1_CorrectState = -1;
double Car1_FDistance = -1;
int Car1_moveState = -1;



int Car2_CorrectState = -1;
double Car2_FDistance = -1;
int Car2_moveState = -1;

u8 mustStop = 0;

/**************************************************************************
�������ܣ�		  ����С��1������  ����
��ڲ�����		��
����  ֵ��		��
**************************************************************************/
void PaserCar1_State(void)
{
	cJSON *root, *orderValue;  // 
	
	if (zone_1_car1_jsonParseBuF[0] == '-' ) //  ��δ�յ��˶�����
	{
		return;
	}
	
	root = cJSON_Parse(zone_1_car1_jsonParseBuF);

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
	
	if (zone_1_car2_jsonParseBuF[0] == '-' ) //  ��δ�յ��˶�����
	{
		return;
	}
	
	root = cJSON_Parse(zone_1_car2_jsonParseBuF);

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
	strSend[2] = ZONE_1_CAR_1;
	strSend[3] = MYSELF_ROLE;


	root=cJSON_CreateObject();

	cJSON_AddNumberToObject(root,"msType", 1);
	cJSON_AddNumberToObject(root,"X_V", X_V);
	cJSON_AddNumberToObject(root,"moveState", moveState);


	strJson=cJSON_Print(root); 
	cJSON_Delete(root); 
	
	jsonSize = strlen(strJson);

	strSend[4] = jsonSize >> 8;
	strSend[5] = jsonSize;

	strncpy(strSend+6,strJson,jsonSize);

	// ��Ҫ��
	zigBee_sendString(strSend,6 + jsonSize);
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
		strSend[2] = ZONE_1_CAR_2;
		strSend[3] = MYSELF_ROLE;
	
		root=cJSON_CreateObject();
	
		cJSON_AddNumberToObject(root,"msType", 1);
		cJSON_AddNumberToObject(root,"X_V", X_V);
		cJSON_AddNumberToObject(root,"moveState", moveState);
	
	
		strJson=cJSON_Print(root); 
		cJSON_Delete(root); 
		
		jsonSize = strlen(strJson);
	
		strSend[4] = jsonSize >> 8;
		strSend[5] = jsonSize;
	
		strncpy(strSend+6,strJson,jsonSize);
	
		// ��Ҫ��
		zigBee_sendString(strSend,6 + jsonSize);
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

	if (mustStop == 1)
	{
		//���ֹ���ֹͣ
		printf("\r\n SomeThing is bad��������");
		AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
		AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;

	}
	if ((Car1_moveState == STATE_STOP) && (Car1_moveState == STATE_STOP)) // ���� ���ϵ� ������������ճ����
	{
		if (  (Car1_CorrectState  <= 0)|| ( Car2_CorrectState <= 0) )//��̬δУ׼
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
		
		if ((Car1_FDistance <= TURING_DISTANCE ) && (Car2_FDistance <= TURING_DISTANCE )) // ������ת���
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
		else
		{
			if (Car1_FDistance <= TURING_DISTANCE)
				{
					//��1  ֹͣ����2 ����
					AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
					AiwacMasterSendOrderCar2(MIN_SPEED , STATE_STRAIGHT) ;
					printf("\r\ncar1 stop, car2 go on");
				}
			else
				{
					//��2	ֹͣ����1 ����
					AiwacMasterSendOrderCar1(MIN_SPEED , STATE_STRAIGHT) ;
					AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;
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

				if(myabs_double(Car1_FDistance - Car2_FDistance) >0.3)  // �������̫��Σ�գ�����ֹͣ
				{
					mustStop = 1;
					printf("\r\n the distance with cars  is too far");
				}
				
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
		FSpeed = (FD - FD_care)*1000 + FSpeed;
	}

	if (FSpeed > FDSMax)
	{
		FSpeed = FDSMax;
	}


	return FSpeed;

}

