#ifndef __CONTROL_H
#define __CONTROL_H

#include "headfile.h"


#define 	Beep   	PDout(15)
#define 	GDG   	PEin(2)

/********************πÈ“ªªØ***********************/
#define   normal_outside   400
#define   normal_inside    400
#define   normal_vertical  100
/*************************************************/
extern float loop_err,err,loop_errc,errc,loop_errc_temp[5],errc_temp[5],charge_err,charge_errc;
extern float adjust,L_speed,R_speed;
extern float L_outside,R_outside,L_inside,R_inside,L_vertical,R_vertical;
extern float Data_sum[6],Data_reg[6][5];
extern float outflag,loop_flag,compensation_flag,loop_shield_flag;
extern float V_battery,V_true,I_true,charge_pwm,charge_adjust,charge_err,power,V_start;
extern float loop_shield_count;
extern float LZflag,LZflag1,LZflag2,LZ_number,LZ_PD;
/*************************************************/
extern float charge_kp,charge_kd,charge_k,aim;
extern float speed,set_speed,inloop_speed;
extern float k;
extern float kp,loop_kp;
extern float kd,loop_kd;
extern float compensation;
extern float inloop_sum,outloop_sum,outloop_err;
/*************************************************/

void PID_realize(void);
void Judge(void);
void GetData(void);
void charge(void);	
void LZ(void);
		

#endif
