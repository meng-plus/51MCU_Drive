

#include "T0PWM.h"

#include <REGX52.H>

unsigned int PWMcycle = 20000 - 1-0x1A; //周期
unsigned int PWMHTimes = 5000 - 1; //高电平时间 20000us
unsigned int PWMLTimes = 15000 - 1; //高电平时间 20000us


void T0_Init()
{
  TMOD = TMOD & 0xf0 | 0x01; //T0 16位手动重载
  TF0 = 0;
  TL0 = PWMHTimes;            //initial timer0 low byte
  TH0 = PWMHTimes >> 8;       //initial timer0 high byte
  TR0 = 1;                        //timer0 start running
  ET0 = 1;                        //enable timer0 interrupt
  EA = 1;                         //open global interrupt switch
}

//设置站空比例
void SetDutyCycle (float value)
{

  PWMHTimes = value * PWMcycle;
  PWMLTimes = PWMcycle - PWMHTimes;
}

void changPWMValue (unsigned int value)
{
  value = value > PWMcycle ? PWMcycle : value; //防止越界
  PWMHTimes = value;
  PWMLTimes = PWMcycle - PWMHTimes;
}


//舵机的控制范围 500us ~2500us ->0°~180°
void SetMotoangle (float angle)
{
  //    //防止越界而损伤电机
  if (angle > 175) angle = 175;

  if (angle < 5) angle = 5;

  PWMHTimes = 500 + angle * 2000.0 / 180; //计算出高电平时间
  PWMLTimes = PWMcycle - PWMHTimes;
}

/*****PWM参数说明************/
// 0xB1E0 = 65536-PWM周期(20ms)
// PWMTimes 为高电平持续时间
/* Timer2 interrupt routine */
void tm0_isr() interrupt 1 using 1//PWM控制定时器程序
{
  T0_PWMout = ~T0_PWMout;

  if (T0_PWMout)
  {
    TL0 = (0 - PWMHTimes);
    TH0 = (0 - PWMHTimes) >> 8;

  }
  else
  {
    TL0 = (0 - PWMLTimes);
    TH0 = (0 - PWMLTimes) >> 8;
  }
}