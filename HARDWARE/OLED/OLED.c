#include "OLED.h"
#include "FONT_ASCII.h"
#include "FONT_CHINESE.h"
#include "common.h"
//#include<include.h>



/********************************************************************
函数功能:向OLED写一个字节数据
函数名称:OLED_write_byte
形式参数:command:指令,取值范围:0/1 0是命令,1是数据  data:8位数据
返回值:  空
修改人:  kofee
修改时间:2014/5/12
*********************************************************************/
void OLED_write_byte(uint8 command,uint8 data)
{
    uint8 i = 8;
    OLED_DC = command ? 1 : 0;   //command是1DC就为1 不是1DC就为0  1是数据 0是命令
    while(i--)
    {
        OLED_SCLK = 0;
        OLED_SDIN = (data&0x80) ? 1 : 0;		
        OLED_SCLK = 1;
        data <<= 1;        //左移一位	
    }
}
/********************************************************************
函数功能:设置OLED的显示位置
函数名称:OLED_set_pos
形式参数:row 行 取值范围:0-7  col 列 取值范围0-127
返回值:  空
修改人:  kofee
修改时间:2014/5/12
*********************************************************************/
void OLED_set_pos(uint8 row,uint8 col)
{
    OLED_write_byte(CMD,0xB0+row);            //设置行        
    OLED_write_byte(CMD,col&0x0F);            //设置某列的低四位地址 0x00-0x0F
    OLED_write_byte(CMD,(col>>4)|0x10);       //设置某列的高四位地址 0x10-0x1F


}

/********************************************************************
函数功能:用某一数据填充OLED的RAM区
函数名称:OLED_fill_ram
形式参数:data:要填充的数据
返回值:  空
修改人:  kofee
修改时间:2014/5/12
*********************************************************************/
void OLED_fill_ram(uint8 data)
{
    uint8 row,col;
    for(row=0;row<8;row++)
    {
        OLED_write_byte(CMD,0xB0+row);  //每行
        OLED_write_byte(CMD,0x00);      //第一列
        OLED_write_byte(CMD,0x10);
        {
            for(col=0;col<128;col++)
            {
                OLED_write_byte(DATA1,data);  //写入数据
            }
        }
    }
}


/********************************************************************
函数功能:初始化OLED
函数名称:OLED_init
形式参数:空
返回值:  空
修改人:  kofee
修改时间:2014/5/12
*********************************************************************/
void OLED_init()
{
    //与平台相关
    GPIO_QuickInit(HW_GPIOD,0,kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOD,1,kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOD,2,kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOD,3,kGPIO_Mode_OPP); 
    GPIO_QuickInit(HW_GPIOE,4,kGPIO_Mode_OPP);  
 
    OLED_CS  = 0;  //芯片选择输入 低电平使能   
    OLED_RES = 0;  //复位信号,不可省略
//    DELAY_NOP;
//    DELAY_NOP;
    OLED_RES = 1;
    
   
    OLED_write_byte(CMD,0xA8);    //set Multiplex Ratio
    OLED_write_byte(CMD,0x3F);  
    
    OLED_write_byte(CMD,0xD3);    //设置显示偏移
    OLED_write_byte(CMD,0x00);
    
    OLED_write_byte(CMD,0x40);    //设置开始显示行 
    
    OLED_write_byte(CMD,0xA1);    //设置段重映射  A0h/A1h
    
    OLED_write_byte(CMD,0xC8);    //设置COM的扫描方向  C0h/C8h
    
    OLED_write_byte(CMD,0xDA);    //设置COM管脚的硬件配置
    OLED_write_byte(CMD,0x12);
    
    OLED_write_byte(CMD,0x81);    //设置对比度
    OLED_write_byte(CMD,0x7F);
    
    OLED_write_byte(CMD,0xA4);    //禁止屏幕显示
    
    OLED_write_byte(CMD,0xA6);    //设置正常显示
    
    OLED_write_byte(CMD,0xD5);    //设置OSC频率
    OLED_write_byte(CMD,0x80);
    
    OLED_write_byte(CMD,0x8D);    //Enable charge pupm regulator
    OLED_write_byte(CMD,0x14);
    
    OLED_write_byte(CMD,0xAF);    //开显示
    
    OLED_fill_ram(0x00);          //清屏  
}

/********************************************************************
函数功能:向OLED写入一个6*8字符
函数名称:OLED_write_char_6x8
形式参数:ascii:标准ASCII字符
返回值:  空
修改人:  kofee
修改时间:2014/5/12
*********************************************************************/
void OLED_write_char_6x8(char ascii)
{
    uint8 temp,i;
    temp = (uint8)(ascii - 32);
    for(i=0;i<6;i++)
    {
        OLED_write_byte(DATA1,ASCII_6X8[temp][i]);
    }
}

/********************************************************************
函数功能:向OLED写入一6*8字符串
函数名称:OLED_display_string_6x8
形式参数:row:行 0-7 col:列 0-127  
         *str:标准ASCII字符串 每列最多21个字符,多余字符会循环到当前行的开始位置,覆盖之前的输出
返回值:  空
修改人:  kofee
修改时间:2014/5/12
*********************************************************************/
void OLED_display_string_6x8(uint8 row,uint8 col,char * str)
{
    OLED_set_pos(row,col);
    while(*str)
    {
        OLED_write_char_6x8(*str++);
    }
}

/********************************************************************
函数功能:向OLED写入一8*16字符串
函数名称:OLED_display_string_8x16
形式参数:row:行 0-6 相邻两行差值为1 比如第一行是0 则第二行为2 否则会覆盖第一行下半部分
         col:列 0-127  
         *str:标准ASCII字符串  每列最多16个字符 多余字符会循环回到当前行的开始位置,覆盖之前的输出
返回值:  空
修改人:  kofee
修改时间:2014/5/13
*********************************************************************/
void OLED_display_string_8x16(uint8 row,uint8 col,char * str)
{        
    uint8 temp ,i;            
    while(*str)                         
    {
        temp = (uint8)(*str - 32);
        for(i=0;i<8;i++)
        {
            OLED_set_pos(row,col);                     //先写上半部分        
            OLED_write_byte(DATA1,ASCII_8X16[temp][i]);
            
            OLED_set_pos(row+1,col);                   //再写下半部分  
            OLED_write_byte(DATA1,ASCII_8X16[temp][8+i]);
            
            col++;                                     //列地址++
        }
        *str++;
    }
}    


/********************************************************************
函数功能:在第row行第col列显示中文  显示的内容在FONT_CHINESE.h中的 FONT_CHINESE[]数组中
函数名称:OLED_display_chinese
形式参数:row:行 0-6 相邻两行差值为1 比如第一行是0 则第二行为2 否则会覆盖第一行下半部分
         col:列 0-127
         start 从第几个汉子开始显示
         len  显示汉字的长度
返回值:  空
修改人:  kofee
修改时间:2014/5/13
*********************************************************************/
void OLED_display_chinese(uint8 row,uint8 col,uint8 start,uint8 len)
{
    uint8 i,j;
    for(i=start;i<start+len;i++)
    {
        OLED_set_pos(row,col);
        for(j=0;j<16;j++)                             //显示汉字的上半部分
        {
            OLED_write_byte(DATA1,FONT_CHINESE[j+i*32]);
        }
        
        OLED_set_pos(row+1,col);                      //显示汉字的下半部分
        for(j=16;j<32;j++)
        {
            OLED_write_byte(DATA1,FONT_CHINESE[j+i*32]);
        }
        col += 16;
    }
}

/********************************************************************
函数功能:将一个int16型整数转换成对应的字符串格式
函数名称:int16_to_string
形式参数:number 将要转换的数
         *str   转换完成的数组
返回值:  空
修改人:  kofee
修改时间:2014/5/13
*********************************************************************/
void int16_to_string(int16 number,char *str)
{
    uint16 temp1,temp2;
    uint8 len = 0;           //数字长度
    if(number==0)
    {
        str[0] = '0';
        len++;
    }
    else
    {
        temp1 = temp2 = number>=0 ? number : -number; 
        while(temp1)         //求数字长度
        {
            temp1 /= 10;
            len++;
        }
        
        if(number<0)
        {
            str[0] = '-';
        }
        
        while(temp2)    //从后往前求余数
        {
            str[len--] = temp2 % 10 + '0';
            temp2 /= 10;
        }
    }
}


/********************************************************************
函数功能:在第row行第col列显示一个6*8的整数,可以带符号,int16型的
函数名称:OLED_display_number_6x8
形式参数:row:行 0-7 
         col:列 0-127
         number 要显示的整数
返回值:  空
修改人:  kofee
修改时间:2014/5/13
*********************************************************************/
void OLED_display_number_6x8(uint8 row,uint8 col,int16 number)
{
    char str[6] = {32,32,32,32,32,32};         //32代表空格全部初始化为空
    int16_to_string(number,str);
    OLED_display_string_6x8(row,col,str);    
}

/********************************************************************
函数功能:在第row行第col列显示一个8*16的整数,可以带符号,int16型的
函数名称:OLED_display_number_8x16
形式参数:row:行 0-6 相邻两行差值为1 比如第一行是0 则第二行为2 否则会覆盖第一行下半部分
         col:列 0-127  
         number 要显示的整数
返回值:  空
修改人:  kofee
修改时间:2014/5/13
*********************************************************************/
void OLED_display_number_8x16(uint8 row,uint8 col,int16 number)
{        
    char str[6] = {32,32,32,32,32,32};      //全部初始化为空
    int16_to_string(number,str);
    OLED_display_string_8x16(row,col,str);

}


/********************************************************************
函数功能:显示128个像素点的二值化信息
函数名称:OLED_display_128pixel
形式参数:row:行
         *pixel 图像数组 
         threshold 阈值
返回值:  空
修改人:  kofee
修改时间:2014/5/13
*********************************************************************/
void OLED_display_128pixel(uint8 row,uint8 *pixel,uint8 threshold)
{  
    uint8 i;
    uint8 *temp = pixel;
    OLED_set_pos(row,0);
    for(i=0;i<128;i++)
    {
        if(*temp++>threshold)
        {     
            OLED_write_byte(DATA1,0x07);
        }
        else
        {
            OLED_write_byte(DATA1,0x00);
        }
    }
    
    temp = pixel;
    OLED_set_pos(row+1,0);
    for(i=0;i<128;i++)
    {
        if(*temp++>threshold)
        {     
            OLED_write_byte(DATA1,0x00);
        }
        else
        {
            OLED_write_byte(DATA1,0xE0);
        }
    }

}




void OLED_display_chineses(uint8 row,uint8 col,uint8 start,uint8 len)
{
    uint8 i,j;
    for(i=start;i<start+len;i++)
    {
        OLED_set_pos(row,col);
        for(j=0;j<16;j++)                             //显示汉字的上半部分
        {
            OLED_write_byte(DATA1,FONT_HANGZI[j+i*32]);
        }
        
        OLED_set_pos(row+1,col);                      //显示汉字的下半部分
        for(j=16;j<32;j++)
        {
            OLED_write_byte(DATA1,FONT_HANGZI[j+i*32]);
        }
        col += 16;
    }
}


void OLED_test()
{
    OLED_display_string_6x8(0,0,"Chang'an University!");     //显示该字符串
    OLED_display_number_6x8(1,0,12343);                      //显示数字
    
    OLED_display_string_8x16(2,0,"CHD University!");         //显示字符串
    
    OLED_display_chinese(4,0,0,4);  //显示前四个汉字
    OLED_display_chinese(6,0,4,6);  //显示接下来六个汉字
    
    OLED_display_number_8x16(4,64,22353);
                          
}

