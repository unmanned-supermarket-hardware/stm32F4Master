#ifndef __AIWACMASTER_H
#define __AIWACMASTER_H

#include "malloc.h"
#include "cJSON.h"

#define TURING_DISTANCE 0.52  // ǰ����Ҫת���  ʱ�� m
#define TURING_DISTANCE_GAP 0.015  // ת������ m

#define FRONT_DISTANCEGAP  0.02  //  ����  ǰ�� ����� m
#define DEFUALT_SPEED 100  //  С����Ĭ���ٶ�
#define MIN_SPEED 30  //С�� �˶�����С�ٶ�   mm/s
#define ACC_SPEED 150  //  С���ļ����ٶ�
#define CAR_STOP 0  // С���ٶ�ֹͣ
#define FD_MAX_SPEED 600  // С���˶��� ����ٶ�  mm/s

// С�����˶�״̬
#define STATE_STOP 0
#define STATE_STRAIGHT 1
#define STATE_TURN_RIGHT 2
#define STATE_TURN_LEFT 3



// С��ǰ��������
#define FRONT_DIRECTION  1

#define BACK_DIRECTION  2



extern int Car1_CorrectState ;
extern double Car1_FDistance ;
extern double Car1_BDistance ;
extern int Car1_moveState ;



extern int Car2_CorrectState ;
extern double Car2_FDistance ;
extern double Car2_BDistance ;
extern int Car2_moveState;


void PaserCar2_State(void);
void PaserCar1_State(void);
double myabs_double(double a);
void AiwacMasterSendOrderCar1(double X_V, int moveState);
void AiwacMasterSendOrderCar2(double X_V, int moveState);
void Aiwac2CARTeamwork(void);
double  designFSpeed(double FD);
void goGoalPosition(int direction,double needDistance);
void goStartTogether(int direction);
void goToLocation(int direction,double needDistance);
double  designFSpeed2(double FD, double FD_care,double iniTDistance);
void sendTuringOrder(int Left_or_Right);



#endif

