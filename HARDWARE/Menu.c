#include "Menu.h"


/***********************************************/
int page,keyflag=1;
float A=23,KEY;
/***********************************************/


struct MenuItem        
{
    uint8_t MenuCount;
    uint8_t *DisplayString;
	float *p;
    void (*Subs)();	
};
struct MenuItem MainMenu[10];

void NullSubs(void){}

void MainMenuInit(void)
{
     MainMenu[0].MenuCount = 10;
     MainMenu[0].DisplayString ="set_speed";
	 MainMenu[0].p = &set_speed;
     MainMenu[0].Subs = NullSubs;
	
     MainMenu[1].MenuCount = 10;
     MainMenu[1].DisplayString = "outflag";
	 MainMenu[1].p = &outflag;
     MainMenu[1].Subs = NullSubs;


     MainMenu[2].MenuCount = 10;
     MainMenu[2].DisplayString = "I_true";
	 MainMenu[2].p = &I_true;
     MainMenu[2].Subs = NullSubs;
   
     MainMenu[3].MenuCount = 10;
     MainMenu[3].DisplayString = "charge_pwm";
	 MainMenu[3].p = &charge_pwm;
     MainMenu[3].Subs = NullSubs;
	
	 MainMenu[4].MenuCount = 10;
     MainMenu[4].DisplayString = "V_battery";
	 MainMenu[4].p = &V_battery;
     MainMenu[4].Subs = NullSubs;
	 
	 MainMenu[5].MenuCount = 10;
     MainMenu[5].DisplayString = "in_sum";
	 MainMenu[5].p = &inloop_sum;
     MainMenu[5].Subs = NullSubs;
	 
	 MainMenu[6].MenuCount = 10;
     MainMenu[6].DisplayString = "V_battery";
	 MainMenu[6].p = &V_battery;
     MainMenu[6].Subs = NullSubs;
     
     MainMenu[7].MenuCount = 10;
     MainMenu[7].DisplayString = "out_sum";
	 MainMenu[7].p = &outloop_sum;
     MainMenu[7].Subs = NullSubs;
	 
	 MainMenu[8].MenuCount = 10;
     MainMenu[8].DisplayString = "adjust";
	 MainMenu[8].p = &adjust;
     MainMenu[8].Subs = NullSubs;
	 
	 MainMenu[9].MenuCount = 10;
     MainMenu[9].DisplayString = "V_battery";
	 MainMenu[9].p = &V_battery;
     MainMenu[9].Subs = NullSubs;
	 
}

///*******************flash*********************/

struct MenuItem (*MenuPoint) = MainMenu;
short DisplayStart = 0;
short UserChoose = 0;
short DisplayPoint = 0;
short MaxItems; 
uint8_t kong=2;


void ShowMenu(void)

{
    uint8_t n=0;
    MaxItems = MenuPoint[0].MenuCount;
    DisplayPoint = DisplayStart;
    
    OLED_display_number_6x8(0,5,L_outside);
    OLED_display_number_6x8(0,35,L_inside); 
    OLED_display_number_6x8(0,65,R_inside);
    OLED_display_number_6x8(0,95,R_outside);
      
    OLED_display_number_6x8(1,5,L_speed);
    OLED_display_number_6x8(1,35,L_vertical);
    OLED_display_number_6x8(1,65,R_vertical);
    OLED_display_number_6x8(1,95,R_speed);
    
    for (n=0;n<8-kong;n++)
    { 
        OLED_display_string_6x8(n+kong,20,MenuPoint[DisplayPoint].DisplayString);
        OLED_display_number_6x8(n+kong,90,*MenuPoint[DisplayPoint].p);
            
        if (DisplayPoint == UserChoose)
        {
			if(n+kong!=2) 	OLED_display_string_6x8(n+kong-1,2,"  ");
			OLED_display_string_6x8(n+kong+1,2,"  ");
            OLED_display_string_6x8(n+kong,2,"->");
        }
              
        if (DisplayPoint == MaxItems-1)
        {
            break;
        }
		DisplayPoint +=1;
    }
}
 

void Menu(void)
{
    uint8 key;
	key = KEY_Scan(0);
    if (key != 0)
		{
			switch(key)
			{
                case 1:
                    if(keyflag==1)
						 *MainMenu[UserChoose].p -= 1;
                    else
						 *MainMenu[UserChoose].p -= 10;
					break;
							
				case 2:
					if(keyflag==1)
                        *MainMenu[UserChoose].p += 1;
                    else
                        *MainMenu[UserChoose].p += 10;					
					break;
                
				case 3:
					  
					if (UserChoose > 0)
					{
                        
						UserChoose --;
					}
					break;
				
				case 4:
					if (UserChoose < MaxItems-1)
					{
                        
						UserChoose ++;
					}  
					break;				
				
				case 5:					
                    {                      
                        outflag=-1*outflag;
                    }
					break;	

                case 6:
					keyflag=-1*keyflag;
					break;
                
                case 7:

					keyflag=-1*keyflag;
					break;
                          
			}   
			
			if (UserChoose > DisplayStart+7-kong)
			{
                page++;
				OLED_fill_ram(0x00);
				DisplayStart = UserChoose;
			} 
			if (UserChoose < DisplayStart)
			{
                page--;
				OLED_fill_ram(0x00); 
				DisplayStart -= 8-kong;
			}			               
		}
    ShowMenu();
    DWT_DelayMs(100);
}



