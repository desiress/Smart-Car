#include "Key.h"



/* 按键扫描程序 */
uint8_t KEY_Scan(uint8_t mode)
{	 
	static uint8_t key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEYL==1||KEYR==1||KEYU==1||KEYD==1||KEYP==1||KEY0==1||KEY1==1))
	{
		DelayMs(5);//去抖动 
		key_up=0;
		if(KEYL==0)return 1;
		else if(KEYR==0)return 2;
		else if(KEYU==0)return 3;
        else if(KEYD==0)return 4;
        else if(KEYP==0)return 5;
        else if(KEY0==0)return 6;
        else if(KEY1==0)return 7;

	}
    else if(KEYL==1&&KEYR==1&&KEYU==1&&KEYD==1&&KEYP==1&&KEY0==1&&KEY1==1)key_up=1; 	    
 	return 0;// 无按键按下

}


