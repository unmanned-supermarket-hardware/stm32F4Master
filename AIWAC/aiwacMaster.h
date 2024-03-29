#ifndef __AIWACMASTER_H
#define __AIWACMASTER_H

#include "malloc.h"
#include "cJSON.h"

#define TURING_DISTANCE 0.52  // 前方需要转弯的  时候 m
#define FRONT_DISTANCEGAP  0.02  //  两车  前后 距离差 m
#define DEFUALT_SPEED 100  //  小车的默认速度
#define MIN_SPEED 30  //小车 运动的最小速度   mm/s
#define ACC_SPEED 150  //  小车的加速速度
#define CAR_STOP 0  // 小车速度停止
#define FD_MAX_SPEED 1000  // 小车运动的 最大速度  mm/s

// 小车的运动状态
#define STATE_STOP 0
#define STATE_STRAIGHT 1
#define STATE_TURN_RIGHT 2
#define STATE_TURN_LEFT 3

// ziigbee 通信角色标志
#define	CONTROL_MASTER 			1
#define	ZONE_1_CAR_1 			11	
#define	ZONE_1_CAR_2 			12	
#define	ZONE_1_PICKUP_UNIT		13
#define	ZONE_1_MODULE			14
#define	ZONE_2_CAR_1 			21	
#define	ZONE_2_CAR_2 			22	
#define	ZONE_2_PICKUP_UNIT		23
#define	ZONE_2_MODULE			24
#define	SYS_MAX_FLAG			25
#define	MYSELF_ROLE				CONTROL_MASTER   //  每次都需要改


extern int Car1_CorrectState ;
extern double Car1_FDistance ;
extern int Car1_moveState ;



extern int Car2_CorrectState ;
extern double Car2_FDistance ;
extern int Car2_moveState;

extern u8 mustStop ;

void PaserCar2_State(void);
void PaserCar1_State(void);
double myabs_double(double a);
void AiwacMasterSendOrderCar1(double X_V, int moveState);
void AiwacMasterSendOrderCar2(double X_V, int moveState);
void Aiwac2CARTeamwork(void);
double  designFSpeed(double FD);


#endif

