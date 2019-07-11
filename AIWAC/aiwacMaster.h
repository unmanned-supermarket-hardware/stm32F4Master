#ifndef __AIWACMASTER_H
#define __AIWACMASTER_H

#include "malloc.h"
#include "cJSON.h"

#define TURING_DISTANCE 500  // ǰ����Ҫת���  ʱ�� mm
#define FRONT_DISTANCEGAP  20  //  ����  ǰ�� ����� mm
#define DEFUALT_SPEED 100  //  С����Ĭ���ٶ�
#define ACC_SPEED 150  //  С���ļ����ٶ�
#define CAR_STOP 0  // С���ٶ�ֹͣ


// С�����˶�״̬
#define STATE_STOP 0
#define STATE_STRAIGHT 1
#define STATE_TURN_RIGHT 2
#define STATE_TURN_LEFT 3


extern int Car1_CorrectState ;
extern int Car1_FDistance ;
extern int Car1_moveState ;



extern int Car2_CorrectState ;
extern int Car2_FDistance ;
extern int Car2_moveState;


void PaserCar2_State(void);
void PaserCar1_State(void);
double myabs_double(double a);
void AiwacMasterSendOrderCar1(double X_V, int moveState);
void AiwacMasterSendOrderCar2(double X_V, int moveState);
void Aiwac2CARTeamwork(void);

#endif

