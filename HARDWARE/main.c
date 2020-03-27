#include "headfile.h"

static void PIT_ISR (void)
{ 

		
	charge();
	GetData();
	Judge();
	PID_realize();	
//	if(LZflag==1) LZ();
//	else PID_realize();	
	

}

void All_Init(void)
 {
    DelayInit(); 
	UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);     
    MainMenuInit();
    
    OLED_init();
 
//	/*配置FTM*/

    FTM_PWM_QuickInit(FTM2_CH0_PA10, kPWM_EdgeAligned, 5500);
    FTM_PWM_QuickInit(FTM2_CH1_PA11, kPWM_EdgeAligned, 5500);
    FTM_PWM_QuickInit(FTM0_CH2_PC03, kPWM_EdgeAligned, 30000);	//恒功率控制
	FTM_PWM_QuickInit(FTM0_CH1_PC02, kPWM_EdgeAligned, 30000);

	
	FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH0, 0); /* 0-10000 对应 0-100% */
	FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH1, 0);	
	FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, 0); /* 0-10000 对应 0-100% */
	FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1, 0); /* 0-10000 对应 0-100% */




    /* 初始化ADC模块*/	

//    ADC_QuickInit(ADC0_SE8_PB0, kADC_SingleDiff10or11); 	//左外
//    ADC_QuickInit(ADC0_SE9_PB1, kADC_SingleDiff10or11); 	//左竖
//    ADC_QuickInit(ADC0_SE12_PB2, kADC_SingleDiff10or11); 	//左内
//    ADC_QuickInit(ADC1_SE11_PB05, kADC_SingleDiff10or11); 	//右内
//    ADC_QuickInit(ADC0_SE13_PB3, kADC_SingleDiff10or11); 	//右竖
//    ADC_QuickInit(ADC1_SE13_PB07, kADC_SingleDiff10or11); 	//右外

    ADC_QuickInit(ADC0_SE8_PB0, kADC_SingleDiff10or11); 	//左外
    ADC_QuickInit(ADC0_SE13_PB3, kADC_SingleDiff10or11); 	//左竖
    ADC_QuickInit(ADC0_SE12_PB2, kADC_SingleDiff10or11); 	//左内
    ADC_QuickInit(ADC1_SE11_PB05, kADC_SingleDiff10or11); 	//右内
    ADC_QuickInit(ADC1_SE10_PB04, kADC_SingleDiff10or11); 	//右竖
    ADC_QuickInit(ADC1_SE13_PB07, kADC_SingleDiff10or11); 	//右外

	ADC_QuickInit(ADC0_SE15_PC1, kADC_SingleDiff10or11); 	//电流检测
	ADC_QuickInit(ADC0_SE17_E24, kADC_SingleDiff10or11); 	//电压检测

    /* 初始化PIT模块 */
    PIT_QuickInit(HW_PIT_CH0, 1000*5); /* 5 ms 触发一次 */
    /* 注册PIT 中断回调函数 */
    PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR); //0号定时器的中断处理
    /* 开启PIT0定时器中断 */
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true);
      
    
    /* 将GPIO设置为输入模式 芯片内部自动配置上拉电阻 */
    GPIO_QuickInit(HW_GPIOA, 28, kGPIO_Mode_IPU);
    GPIO_QuickInit(HW_GPIOA, 27, kGPIO_Mode_IPU);
    GPIO_QuickInit(HW_GPIOA, 26, kGPIO_Mode_IPU);
    GPIO_QuickInit(HW_GPIOA, 24, kGPIO_Mode_IPU);
    GPIO_QuickInit(HW_GPIOA, 25, kGPIO_Mode_IPU);
    GPIO_QuickInit(HW_GPIOB, 20, kGPIO_Mode_IPU);
    GPIO_QuickInit(HW_GPIOB, 22, kGPIO_Mode_IPU);
	
	GPIO_QuickInit(HW_GPIOD, 15, kGPIO_Mode_OPP); //蜂鸣器
}

int main(void)
{
	All_Init();	
	
	while(1)
	{
		Menu();
	} 
}

