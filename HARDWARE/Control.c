#include "Control.h"
#include "Date.h"

void PID_realize()
{ 
/**************** 环岛pid***************/	
    if(loop_flag==1)
	{
		loop_err = (L_vertical-R_vertical)/(L_vertical+R_vertical)*250; 
		
		if(fabs(loop_direction)==1)
		{
			if(loop_direction>0)
			{
				if(loop_err<70) loop_err=70;
			}
			if(loop_direction<0)
			{
				if(loop_err>-70) loop_err=-70;
			}
		}
		else
		{
			if(loop_direction>0)
			{
				if(loop_err<30) loop_err=30;
			}
			if(loop_direction<0)
			{
				if(loop_err>-30) loop_err=-30;
			}
		}
		
		
		if(loop_err>250) loop_err=250;
		if(loop_err<-250) loop_err=-250;
			
		loop_errc_temp[4] = loop_errc_temp[3];
		loop_errc_temp[3] = loop_errc_temp[2];
		loop_errc_temp[2] = loop_errc_temp[1];
		loop_errc_temp[1] = loop_errc_temp[0];
		loop_errc_temp[0] = loop_err; 
		
		loop_errc = 4*(loop_errc_temp[0]-loop_errc_temp[4]);
		
		if(loop_errc>180) loop_errc=180;
		if(loop_errc<-180) loop_errc=-180;
		
		adjust = loop_kp*0.01*loop_err+loop_kd*0.01*loop_errc;
	}

/*************** 正常pid****************/
	else
	{
		if(outloop_flag==1) 
		{
			if(L_vertical+R_vertical<220)//270
			{
				buchang=(L_vertical-R_vertical)/220*250;
			}
			else	
			{
				buchang=(L_vertical-R_vertical)/(L_vertical+R_vertical)*220;
			}
			if(buchang>280) buchang=280;
			if(buchang<-280) buchang=-280;
			
			if(loop_direction>0)
			{
				if(buchang>0)
				{
					err = ((L_inside-R_inside-buchang)/(L_inside+R_inside)*1450);
				}
				else
				{
					err = ((L_inside-R_inside+buchang)/(L_inside+R_inside)*1450);
				}
				
			}
			else
			{
				if(buchang<0)
				{
					err = ((L_inside-R_inside-buchang)/(L_inside+R_inside)*1450);
				}
				else
				{
					err = ((L_inside-R_inside+buchang)/(L_inside+R_inside)*1450);
				}
			}		
		}
		else
		{
			err = ((L_outside-R_outside)/(L_inside+R_inside)*k_sum); 
			if(LZflag3==1)
			{
				if(err>0)
				{
					FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH0, 0); /* 0-10000 对应 0-100% */
					FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH1, 0);	
				}
			}
			
		}
		
		
		if(err>550) err=550;
		if(err<-550) err=-550;
		 

		errc_temp[4] = errc_temp[3];
		errc_temp[3] = errc_temp[2];
		errc_temp[2] = errc_temp[1];
		errc_temp[1] = errc_temp[0];
		errc_temp[0] = err;                       //水平电感 递推
			
		errc = 4*(errc_temp[0]-errc_temp[4]);     //水平电感的偏差微分

		
		if(errc>220) errc=220;
		if(errc<-220) errc=-220;
		 	
		adjust = kp*0.01*err+kd*0.01*errc;
		 
	}
	
	
	if(adjust>150) adjust=150;
	if(adjust<-150) adjust=-150;
	

	
	if(yisiloop==1||loop_flag==1||outloop_flag==1&&loop_shield_flag!=1)
	{
		speed=inloop_speed;
	}
	else 
	{
		speed=set_speed;
		if(loop_shield_flag==1)
		{
			if(fabs(loop_direction)==2)	speed=Lloop_speed;
			else	speed=Sloop_speed;
		}		
		
	}
	
    L_speed=speed*10-adjust;
    R_speed=speed*10+adjust;
	
	L_speed*=850/V_battery;
	R_speed*=850/V_battery;

/*************** PWM限幅****************/
    
    if(L_speed>1000) L_speed=1000;
    if(L_speed<0) L_speed=0;
    if(R_speed>1000) R_speed=1000;
    if(R_speed<0) R_speed=0;             

    if(outflag==1)
    {
        FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH0, 0); /* 0-10000 对应 0-100% */
		FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH1, 0);	
    }
    else
    {
		FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH0, L_speed*10); /* 0-10000 对应 0-100% */
		FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH1, R_speed*10);	                                                    
    }             

}
	
/************************************** 元素判断 *****************************************/
void Judge()
{	
/************* 出线停车**************/
	
//	if(L_outside+R_outside<90)
//    {
//        outflag=1;    
//	}
	if(outflag!=1)
	{
		LZ_number_last=LZ_number;
		if(GDG==0&&LZ_shield_flag==0)
		{
			LZ_shield_flag=1;
			LZ_number++;
		}		
	}

	if(LZ_number>0&&LZ_number>LZ_number_last)
	{
		if((int)LZ_number%2==1) 
		{
			if(LZ_PD==1) LZflag=1;
		}
		else
		{
			if(LZ_PD==0) LZflag=1;
		}
	}
	
	if(LZflag2==1&&(L_outside+R_outside>400||L_vertical+R_vertical>600))
	{
		LZflag3=1;
		LZflag=0;
	}
	
	if(LZflag3==1&&L_outside-R_outside<-50)
	{
		LZflag=0;
		LZflag1=0;
		LZflag2=0;
		LZflag3=0;
	}
/************* 环岛处理**************/ 
	loop_direction=loop_dir[(int)loop_num];
	
	if(fabs(loop_direction)==1)
	{
		if(L_outside+R_outside+L_inside+R_inside>inloop_sum&&loop_shield_flag==0) 
		{
			yisiloop=1;
			if(fabs(L_vertical-R_vertical)<outloop_err&&(L_vertical+R_vertical)<outloop_err)
			{
				if(L_vertical+R_vertical>L_vertical_last+R_vertical_last+3) count--;
				else count=3;
				if(count==0)
				{
					count=3;
					loop_flag=1;
				}
			}
			
		}
	}
	else
	{
		if(L_outside+R_outside+L_inside+R_inside>inbloop_sum&&loop_shield_flag==0) 
		{
			yisiloop=1;
			if(fabs(L_vertical-R_vertical)<outloop_err)
			{
				if(L_vertical+R_vertical>L_vertical_last+R_vertical_last+3) count--;
				else count=3;
				if(count==0)
				{
					count=3;
					loop_flag=1;			
				}
			}
		}
	}

	if(L_outside+R_outside+L_inside+R_inside<outloop_sum/*&&fabs(L_vertical-R_vertical)>40*/&&loop_flag==1)
	{
		loop_shield_flag=1;	
		loop_flag=0;
		yisiloop=0;
	}

	if(L_outside+R_outside+L_inside+R_inside>2200&&loop_shield_flag==1) 
	{
		outloop_flag=1;
	}
	
	if(L_outside+R_outside+L_inside+R_inside<outloop_sum&&(L_vertical+R_vertical<150||fabs(L_outside-R_outside)<110)&&outloop_flag==1)
	{
		outloop_flag=0;
		loop_shield_flag=0;
		loop_num++;
		if(loop_num==loop_num_max) loop_num=0;
	}

	if(/*outloop_flag==1||*/loop_flag==1)
	{
		Beep=1;
	}
	else
	{
		Beep=0;
	}
	
	if(loop_shield_flag==1)
	{
		loop_shield_count--;
		if(loop_shield_count==0)
		{
			loop_shield_count=bigloop_shild;
			loop_shield_flag=0;
		}
	}
	else
	{
		loop_shield_count=bigloop_shild;
	}
	
	if(LZ_shield_flag==1)
	{
		LZ_shield_count--;
		if(LZ_shield_count==0)
		{
			LZ_shield_count=700;
			LZ_shield_flag=0;
		}
	}
	else
	{
		LZ_shield_count=700;
	}
	
}


/************* 加权平均递推滤波**************/ 
void GetData()
{
    for(j=0;j<6;j++)
    {
        for(i=0;i<4;i++)  
        {
            Data_reg[j][i]=Data_reg[j][i+1];
        }    
    }
 
    Data_reg[0][4] = ADC_QuickReadValue(ADC0_SE8_PB0);    //左外
    Data_reg[1][4] = ADC_QuickReadValue(ADC0_SE13_PB3);   //左竖
    Data_reg[2][4] = ADC_QuickReadValue(ADC0_SE12_PB2);   //左内
    Data_reg[3][4] = ADC_QuickReadValue(ADC1_SE11_PB05);  //右内
    Data_reg[4][4] = ADC_QuickReadValue(ADC1_SE10_PB04);  //右竖
    Data_reg[5][4] = ADC_QuickReadValue(ADC1_SE13_PB07);  //右外
     

    for(j=0;j<6;j++)
    {
        for(i=0;i<4;i++)  
        {
            Data_sum[j] += (i+1)*Data_reg[j][i];
        }    
    }

    for(j=0;j<6;j++)
    {
        Data_sum[j] += 8*Data_reg[j][4];
    }

	L_vertical_last=L_vertical;
	R_vertical_last=R_vertical;
    L_outside=Data_sum[0]/18;
    L_vertical=Data_sum[1]/18;
    L_inside=Data_sum[2]/18;
    R_inside=Data_sum[3]/18;
    R_vertical=Data_sum[4]/18;
    R_outside=Data_sum[5]/18;

    for(j=0;j<6;j++)
    {
        Data_sum[j] = 0;
    } 

//    L_outside = (L_outside<4?4:L_outside);//电感最小值为4
//    R_outside = (R_outside<4?4:R_outside);//电感最小值为4
        
}



void charge()
{
	for(i=0;i<132;i++)
	{
		I[i]=ADC_QuickReadValue(ADC0_SE15_PC1);
		V[i]=ADC_QuickReadValue(ADC0_SE17_E24);
	}
	for(i=0;i<132;i++)
	{
		I_true+=I[i];
		V_battery+=V[i];
	}
	
	I_true=I_true/132.0;	
	I_true=3.3*I_true*0.48*3.5;
//	I_true=3.3*I_true/1024/20*100*100*3.5;

	V_battery=V_battery/132.0;
	V_battery=3.3*V_battery*0.4;
//	V_battery=3.3*V_battery/1024/10*43*100;
		
	power=V_battery*I_true/100;

	charge_k=V_battery/58;
	if(V_battery>860) charge_k=V_battery/74;
	if(V_battery<430) charge_k=V_battery/60;
	if(V_battery<250) charge_k=V_battery/70;
	if(charge_k<1) charge_k=1;
	
	charge_errc=charge_err;
	charge_err=(aim*charge_k-power)/10;
	charge_errc=charge_err-charge_errc;
	
	charge_adjust=charge_err*charge_kp*0.01*charge_k*0.1+charge_errc*charge_kd*0.01;
	charge_pwm+=charge_adjust;
	

	if(charge_pwm>1000) charge_pwm=1000;
	if(charge_pwm<0) charge_pwm=0;	
	if(V_battery>V_start)
	{
		outflag=0;
		charge_pwm=0;
	}	
	if(V_battery<80)	
	{
		FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, 2100);
	}
	else	
	{
		FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, charge_pwm*10);		
	}
}


void LZ()
{
	if(LZflag2==0&&LZflag3==0&&GDG==0)
	{
		LZflag1=1;
	}
	if(LZflag==1&&GDG==1)
	{
		LZflag2=1;
	}

	
	if(LZflag1==1&&LZflag2==0&&LZflag3==0)
	{
		FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH0, 2900*820/V_battery);
		FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH1, 700*820/V_battery);
	}
	if(LZflag2==1&&LZflag3==0)
	{
		FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH0, 1600*820/V_battery);
		FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH1, 2700*820/V_battery);
	}		
}
	
//void LZ()
//{
//	if(LZflag2==0&&LZflag3==0&&GDG==0)
//	{
//		LZflag1=1;
//	}
//	if(LZflag==1&&GDG==1)
//	{
//		LZflag2=1;
//	}

//	
//	if(LZflag1==1&&LZflag2==0&&LZflag3==0)
//	{
//		FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH0, 2900*820/V_battery);
//		FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH1, 700*820/V_battery); 
//	}
//	if(LZflag2==1&&LZflag3==0)
//	{
//		LZ_count++;
//		FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH0, 1600*820/V_battery);
//		if(LZ_count<200)
//		{
//			FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH0, 1700*820/V_battery);
//			FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH1, 2800*820/V_battery);
//		}
//		if(LZ_count>200&&LZ_count<300)
//		{
//			FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH0, ((170+LZ_count/6)*820/V_battery)*10);
//			FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH1, ((270-LZ_count/7)*820/V_battery)*10);
//		}	
//		if(LZ_count>300)
//		{
//			FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH0, 1700*820/V_battery);
//			FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH1, 2500*820/V_battery);
//		}
//	}		
//}


