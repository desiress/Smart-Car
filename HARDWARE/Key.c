#include "Key.h"



/* ����ɨ����� */
uint8_t KEY_Scan(uint8_t mode)
{	 
	static uint8_t key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEYL==1||KEYR==1||KEYU==1||KEYD==1||KEYP==1||KEY0==1||KEY1==1))
	{
		DelayMs(5);//ȥ���� 
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
 	return 0;// �ް�������

}


