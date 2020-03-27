#include "OLED.h"
#include "FONT_ASCII.h"
#include "FONT_CHINESE.h"
#include "common.h"
//#include<include.h>



/********************************************************************
��������:��OLEDдһ���ֽ�����
��������:OLED_write_byte
��ʽ����:command:ָ��,ȡֵ��Χ:0/1 0������,1������  data:8λ����
����ֵ:  ��
�޸���:  kofee
�޸�ʱ��:2014/5/12
*********************************************************************/
void OLED_write_byte(uint8 command,uint8 data)
{
    uint8 i = 8;
    OLED_DC = command ? 1 : 0;   //command��1DC��Ϊ1 ����1DC��Ϊ0  1������ 0������
    while(i--)
    {
        OLED_SCLK = 0;
        OLED_SDIN = (data&0x80) ? 1 : 0;		
        OLED_SCLK = 1;
        data <<= 1;        //����һλ	
    }
}
/********************************************************************
��������:����OLED����ʾλ��
��������:OLED_set_pos
��ʽ����:row �� ȡֵ��Χ:0-7  col �� ȡֵ��Χ0-127
����ֵ:  ��
�޸���:  kofee
�޸�ʱ��:2014/5/12
*********************************************************************/
void OLED_set_pos(uint8 row,uint8 col)
{
    OLED_write_byte(CMD,0xB0+row);            //������        
    OLED_write_byte(CMD,col&0x0F);            //����ĳ�еĵ���λ��ַ 0x00-0x0F
    OLED_write_byte(CMD,(col>>4)|0x10);       //����ĳ�еĸ���λ��ַ 0x10-0x1F


}

/********************************************************************
��������:��ĳһ�������OLED��RAM��
��������:OLED_fill_ram
��ʽ����:data:Ҫ��������
����ֵ:  ��
�޸���:  kofee
�޸�ʱ��:2014/5/12
*********************************************************************/
void OLED_fill_ram(uint8 data)
{
    uint8 row,col;
    for(row=0;row<8;row++)
    {
        OLED_write_byte(CMD,0xB0+row);  //ÿ��
        OLED_write_byte(CMD,0x00);      //��һ��
        OLED_write_byte(CMD,0x10);
        {
            for(col=0;col<128;col++)
            {
                OLED_write_byte(DATA1,data);  //д������
            }
        }
    }
}


/********************************************************************
��������:��ʼ��OLED
��������:OLED_init
��ʽ����:��
����ֵ:  ��
�޸���:  kofee
�޸�ʱ��:2014/5/12
*********************************************************************/
void OLED_init()
{
    //��ƽ̨���
    GPIO_QuickInit(HW_GPIOD,0,kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOD,1,kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOD,2,kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOD,3,kGPIO_Mode_OPP); 
    GPIO_QuickInit(HW_GPIOE,4,kGPIO_Mode_OPP);  
 
    OLED_CS  = 0;  //оƬѡ������ �͵�ƽʹ��   
    OLED_RES = 0;  //��λ�ź�,����ʡ��
//    DELAY_NOP;
//    DELAY_NOP;
    OLED_RES = 1;
    
   
    OLED_write_byte(CMD,0xA8);    //set Multiplex Ratio
    OLED_write_byte(CMD,0x3F);  
    
    OLED_write_byte(CMD,0xD3);    //������ʾƫ��
    OLED_write_byte(CMD,0x00);
    
    OLED_write_byte(CMD,0x40);    //���ÿ�ʼ��ʾ�� 
    
    OLED_write_byte(CMD,0xA1);    //���ö���ӳ��  A0h/A1h
    
    OLED_write_byte(CMD,0xC8);    //����COM��ɨ�跽��  C0h/C8h
    
    OLED_write_byte(CMD,0xDA);    //����COM�ܽŵ�Ӳ������
    OLED_write_byte(CMD,0x12);
    
    OLED_write_byte(CMD,0x81);    //���öԱȶ�
    OLED_write_byte(CMD,0x7F);
    
    OLED_write_byte(CMD,0xA4);    //��ֹ��Ļ��ʾ
    
    OLED_write_byte(CMD,0xA6);    //����������ʾ
    
    OLED_write_byte(CMD,0xD5);    //����OSCƵ��
    OLED_write_byte(CMD,0x80);
    
    OLED_write_byte(CMD,0x8D);    //Enable charge pupm regulator
    OLED_write_byte(CMD,0x14);
    
    OLED_write_byte(CMD,0xAF);    //����ʾ
    
    OLED_fill_ram(0x00);          //����  
}

/********************************************************************
��������:��OLEDд��һ��6*8�ַ�
��������:OLED_write_char_6x8
��ʽ����:ascii:��׼ASCII�ַ�
����ֵ:  ��
�޸���:  kofee
�޸�ʱ��:2014/5/12
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
��������:��OLEDд��һ6*8�ַ���
��������:OLED_display_string_6x8
��ʽ����:row:�� 0-7 col:�� 0-127  
         *str:��׼ASCII�ַ��� ÿ�����21���ַ�,�����ַ���ѭ������ǰ�еĿ�ʼλ��,����֮ǰ�����
����ֵ:  ��
�޸���:  kofee
�޸�ʱ��:2014/5/12
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
��������:��OLEDд��һ8*16�ַ���
��������:OLED_display_string_8x16
��ʽ����:row:�� 0-6 �������в�ֵΪ1 �����һ����0 ��ڶ���Ϊ2 ����Ḳ�ǵ�һ���°벿��
         col:�� 0-127  
         *str:��׼ASCII�ַ���  ÿ�����16���ַ� �����ַ���ѭ���ص���ǰ�еĿ�ʼλ��,����֮ǰ�����
����ֵ:  ��
�޸���:  kofee
�޸�ʱ��:2014/5/13
*********************************************************************/
void OLED_display_string_8x16(uint8 row,uint8 col,char * str)
{        
    uint8 temp ,i;            
    while(*str)                         
    {
        temp = (uint8)(*str - 32);
        for(i=0;i<8;i++)
        {
            OLED_set_pos(row,col);                     //��д�ϰ벿��        
            OLED_write_byte(DATA1,ASCII_8X16[temp][i]);
            
            OLED_set_pos(row+1,col);                   //��д�°벿��  
            OLED_write_byte(DATA1,ASCII_8X16[temp][8+i]);
            
            col++;                                     //�е�ַ++
        }
        *str++;
    }
}    


/********************************************************************
��������:�ڵ�row�е�col����ʾ����  ��ʾ��������FONT_CHINESE.h�е� FONT_CHINESE[]������
��������:OLED_display_chinese
��ʽ����:row:�� 0-6 �������в�ֵΪ1 �����һ����0 ��ڶ���Ϊ2 ����Ḳ�ǵ�һ���°벿��
         col:�� 0-127
         start �ӵڼ������ӿ�ʼ��ʾ
         len  ��ʾ���ֵĳ���
����ֵ:  ��
�޸���:  kofee
�޸�ʱ��:2014/5/13
*********************************************************************/
void OLED_display_chinese(uint8 row,uint8 col,uint8 start,uint8 len)
{
    uint8 i,j;
    for(i=start;i<start+len;i++)
    {
        OLED_set_pos(row,col);
        for(j=0;j<16;j++)                             //��ʾ���ֵ��ϰ벿��
        {
            OLED_write_byte(DATA1,FONT_CHINESE[j+i*32]);
        }
        
        OLED_set_pos(row+1,col);                      //��ʾ���ֵ��°벿��
        for(j=16;j<32;j++)
        {
            OLED_write_byte(DATA1,FONT_CHINESE[j+i*32]);
        }
        col += 16;
    }
}

/********************************************************************
��������:��һ��int16������ת���ɶ�Ӧ���ַ�����ʽ
��������:int16_to_string
��ʽ����:number ��Ҫת������
         *str   ת����ɵ�����
����ֵ:  ��
�޸���:  kofee
�޸�ʱ��:2014/5/13
*********************************************************************/
void int16_to_string(int16 number,char *str)
{
    uint16 temp1,temp2;
    uint8 len = 0;           //���ֳ���
    if(number==0)
    {
        str[0] = '0';
        len++;
    }
    else
    {
        temp1 = temp2 = number>=0 ? number : -number; 
        while(temp1)         //�����ֳ���
        {
            temp1 /= 10;
            len++;
        }
        
        if(number<0)
        {
            str[0] = '-';
        }
        
        while(temp2)    //�Ӻ���ǰ������
        {
            str[len--] = temp2 % 10 + '0';
            temp2 /= 10;
        }
    }
}


/********************************************************************
��������:�ڵ�row�е�col����ʾһ��6*8������,���Դ�����,int16�͵�
��������:OLED_display_number_6x8
��ʽ����:row:�� 0-7 
         col:�� 0-127
         number Ҫ��ʾ������
����ֵ:  ��
�޸���:  kofee
�޸�ʱ��:2014/5/13
*********************************************************************/
void OLED_display_number_6x8(uint8 row,uint8 col,int16 number)
{
    char str[6] = {32,32,32,32,32,32};         //32����ո�ȫ����ʼ��Ϊ��
    int16_to_string(number,str);
    OLED_display_string_6x8(row,col,str);    
}

/********************************************************************
��������:�ڵ�row�е�col����ʾһ��8*16������,���Դ�����,int16�͵�
��������:OLED_display_number_8x16
��ʽ����:row:�� 0-6 �������в�ֵΪ1 �����һ����0 ��ڶ���Ϊ2 ����Ḳ�ǵ�һ���°벿��
         col:�� 0-127  
         number Ҫ��ʾ������
����ֵ:  ��
�޸���:  kofee
�޸�ʱ��:2014/5/13
*********************************************************************/
void OLED_display_number_8x16(uint8 row,uint8 col,int16 number)
{        
    char str[6] = {32,32,32,32,32,32};      //ȫ����ʼ��Ϊ��
    int16_to_string(number,str);
    OLED_display_string_8x16(row,col,str);

}


/********************************************************************
��������:��ʾ128�����ص�Ķ�ֵ����Ϣ
��������:OLED_display_128pixel
��ʽ����:row:��
         *pixel ͼ������ 
         threshold ��ֵ
����ֵ:  ��
�޸���:  kofee
�޸�ʱ��:2014/5/13
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
        for(j=0;j<16;j++)                             //��ʾ���ֵ��ϰ벿��
        {
            OLED_write_byte(DATA1,FONT_HANGZI[j+i*32]);
        }
        
        OLED_set_pos(row+1,col);                      //��ʾ���ֵ��°벿��
        for(j=16;j<32;j++)
        {
            OLED_write_byte(DATA1,FONT_HANGZI[j+i*32]);
        }
        col += 16;
    }
}


void OLED_test()
{
    OLED_display_string_6x8(0,0,"Chang'an University!");     //��ʾ���ַ���
    OLED_display_number_6x8(1,0,12343);                      //��ʾ����
    
    OLED_display_string_8x16(2,0,"CHD University!");         //��ʾ�ַ���
    
    OLED_display_chinese(4,0,0,4);  //��ʾǰ�ĸ�����
    OLED_display_chinese(6,0,4,6);  //��ʾ��������������
    
    OLED_display_number_8x16(4,64,22353);
                          
}

