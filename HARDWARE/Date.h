#ifndef __DATE_H
#define __DATE_H

#include "headfile.h"

#define loop_num_max 2
#define bigloop_shild 1400
#define smallloop_shild 800
uint8_t i=0,j=0;
/*************************************************/
float err,errc,errc_temp[5],adjust,L_speed,R_speed;
float loop_err,err,loop_errc,errc,loop_errc_temp[5];
float charge_err,charge_errc,charge_k,charge_adjust;
float L_outside,R_outside,L_inside,R_inside,L_vertical,R_vertical,L_vertical_last,R_vertical_last;
float Data_sum[6],Data_reg[6][5];
float outflag=1,outloop_flag=0,loop_shield_flag=0;
float loop_flag=0,yisiloop=0,buchang;
float V_battery,I_true,charge_pwm,power;
float I[132],V[132];
float loop_direction=0,LZ_count;
float LZflag=0,LZflag1=0,LZflag2=0,LZflag3=0,LZ_number=0,LZ_number_last=0;
float LZ_shield_flag=0;
/*************************************************/
float k=130,k_sum=800;
float loop_shield_count=bigloop_shild,count=3,LZ_shield_count=1200;
float V_start=850;
float charge_kp=13,charge_kd=0,aim=280;
float speed,set_speed=31;
float inloop_speed=22,Sloop_speed=27,Lloop_speed=31;
float kp=40,/*45*/loop_kp=200;
float kd=280,/*280*/loop_kd=450;
float inloop_sum=2150,inbloop_sum=2350,outloop_sum=1350,outloop_err=160;
float loop_num=0,loop_dir[3]={-1,-2,0}/*loop_dir[3]={2,-1,1}*/,LZ_PD=0;
/*************************************************/


#endif

