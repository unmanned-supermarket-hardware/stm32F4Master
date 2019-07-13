#include "usart.h"	
#include "aiwacMaster.h"


int Car1_CorrectState = -1;
int Car1_FDistance = -1;
int Car1_moveState = -1;



int Car2_CorrectState = -1;
int Car2_FDistance = -1;
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
	}
	Car1_CorrectState = orderValue->valueint;  //��У׼��� 


	orderValue = cJSON_GetObjectItem(root, "FDistance");  //  ǰ������
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	Car1_FDistance = orderValue->valuedouble;  // ǰ������


	orderValue = cJSON_GetObjectItem(root, "moveState");  //  С�����˶�״ָ̬��
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	Car1_moveState = orderValue->valueint;  // С�����˶�״ָ̬��

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
	}
	Car2_CorrectState = orderValue->valueint;  //��У׼��� 


	orderValue = cJSON_GetObjectItem(root, "FDistance");  //  ǰ������
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	Car2_FDistance = orderValue->valuedouble;  // ǰ������


	orderValue = cJSON_GetObjectItem(root, "moveState");  //  С�����˶�״ָ̬��
	if (!orderValue) {
	   // printf("get name faild !\n");
	   // printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	Car2_moveState = orderValue->valueint;  // С�����˶�״ָ̬��

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
	char strSend[1000];
	
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
		char strSend[1000];
		
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
		return;
	}

	if (( Car1_moveState > 1 )|| (Car2_moveState > 1) ) //  ��ǰ��  С����  ת��
	{
		return; //   �ȴ�ת��
	}

	if(  (Car1_CorrectState  == 0)|| ( Car2_CorrectState == 0) )// ��̬δ����
	{

		AiwacMasterSendOrderCar1(CAR_STOP , STATE_STOP) ;
		AiwacMasterSendOrderCar2(CAR_STOP , STATE_STOP) ;
		return;  //�ȴ���̬����

	}
	else  // ��̬ ����ok
	{
		if((Car1_FDistance <= TURING_DISTANCE ) || (Car1_FDistance <= TURING_DISTANCE )) // �е���ת���
		{
			
			if ((Car1_FDistance <= TURING_DISTANCE ) && (Car1_FDistance <= TURING_DISTANCE )) // ������ת���
			{

				//  ת���  ���� Ҫ�� �� �����ı�
				AiwacMasterSendOrderCar1(CAR_STOP , STATE_TURN_RIGHT) ;
				AiwacMasterSendOrderCar2(CAR_STOP , STATE_TURN_RIGHT) ;
			}
			else
			{
				if (Car1_FDistance <= TURING_DISTANCE)
					{
						//��1  ֹͣ����2 ����
						AiwacMasterSendOrderCar1(CAR_STOP , STATE_STRAIGHT) ;
						AiwacMasterSendOrderCar2(DEFUALT_SPEED , STATE_STRAIGHT) ;

					}
				else
					{
						//��2	ֹͣ����1 ����
						AiwacMasterSendOrderCar1(DEFUALT_SPEED , STATE_STRAIGHT) ;
						AiwacMasterSendOrderCar2(CAR_STOP , STATE_STRAIGHT) ;

					}

			}
		}
		else  //  û�г����� ת���
		{
			if( myabs_double(Car1_FDistance - Car2_FDistance ) < FRONT_DISTANCEGAP)  //  ������  ǰ�� ����ok
				{
					// �·�  ���� Ĭ��ǰ�� 
					AiwacMasterSendOrderCar1(DEFUALT_SPEED , STATE_STRAIGHT) ;
					AiwacMasterSendOrderCar2(DEFUALT_SPEED , STATE_STRAIGHT) ;

				}
			else // ������  ǰ�� ����  no
				{
					if (Car1_FDistance - Car2_FDistance >0)  // 1����ǰ 
						{
							// ���� 1��Ĭ���ٶȣ�2�� ��Ĭ�Ͽ��
							AiwacMasterSendOrderCar1(DEFUALT_SPEED , STATE_STRAIGHT) ;
							AiwacMasterSendOrderCar2(ACC_SPEED , STATE_STRAIGHT) ;
						}
					else
						{
							// ���� 2��Ĭ���ٶȣ�1�� ��Ĭ�Ͽ��
							AiwacMasterSendOrderCar1(ACC_SPEED, STATE_STRAIGHT) ;
							AiwacMasterSendOrderCar2(DEFUALT_SPEED , STATE_STRAIGHT) ;
						}
					
				}
		}

	}


}

