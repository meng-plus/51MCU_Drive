// Header:  From https://github.com/chengmeng2018/51MCU_Drive.git
// File Name: main
// Author: chengmeng2018
// Date: 2019-06-30
//version: v1.0
//Note: 仿真界面自己画吧
#include <REGX52.H>
#include <stdio.h>
#include "keyBoard.h"

#define OutLED P2
void InitUART(void);//使用定时器1作为串口波特率发生器
void main()
{
    KeyStruct KeyNum;
    unsigned char KeyState;
    unsigned char str[32];
  InitUART();
    while(1)
    {
        KeyState = key_scan(&KeyNum);//按键检测
        if(KeyState==KeyFallEdge)
        {
            sprintf(str,"KeyState:%d Keynum:%d \r\n ",(unsigned int)KeyState,(unsigned int)(KeyNum.Value));
            printf(str);
          OutLED=KeyNum.Value;
        }
    }
}
//重写putchar函数
char putchar(char c)
{
    ES=0;           //关串口中断
    SBUF=c;           
    while(TI!=1);   //等待发送成功
    TI=0;           //清除发送中断标志
    ES=1;           //开串口中断
    return c;
}

void InitUART(void)//使用定时器1作为串口波特率发生器
{
    TH1 = 0xFD;	 //晶振11.0592mhz 波特率设为9600
    TL1 = TH1;
    TMOD |= 0x20;	 //定时器1方式2
    SCON = 0x50;	 //串口接收使能
    ES = 1;			 //串口中断使能
    TR1 = 1;		 //定时器1使能
    TI = 1;			 //发送中断标记位，必须设置
}