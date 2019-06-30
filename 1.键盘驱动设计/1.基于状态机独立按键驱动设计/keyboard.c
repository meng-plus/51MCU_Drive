
#include <REGX52.H>
#include "keyBoard.h"


void delay2ms (void)  //延时函数
{
  unsigned char i, j;

  for (i = 133; i > 0; i--)
    for (j = 6; j > 0; j--);
}


unsigned char IndependentKeyboard()
{
  unsigned char keyValue = 0xff;
  keyValue &= ~((!(char)Key0)<<0);
  keyValue &= ~((!(char)Key1)<<1);
  keyValue &= ~((!(char)Key2)<<2);
  keyValue &= ~((!(char)Key3)<<3);
  keyValue &= ~((!(char)Key4)<<4);
  keyValue &= ~((!(char)Key5)<<5);
  keyValue &= ~((!(char)Key6)<<6);
  keyValue &= ~((!(char)Key7)<<7);

  return keyValue;
}
//按键检测
unsigned char key_scan (KeyStruct* KeyNum)
{
  unsigned char keyValue = 0;
  unsigned char State = KeyNum->State;
  unsigned char KeyTemp = KeyNum->Value;
  /*******矩阵键盘读取操作******/
  keyValue =	IndependentKeyboard();//独立按键检测

  switch (State)
  {
  case CheckKey://检查是否有按键按下
    if (keyValue == 0xff)
    {
      break;
    }

    KeyTemp = keyValue;
    State = DelayKey;

  case DelayKey://跳过时间消抖
    delay2ms();
    State = KeyTemp == keyValue ? KeyFallEdge : CheckKey;
    break;

  case KeyFallEdge://产生下降沿信号(仅一次)
    State = KeyDown;
    break;

  case KeyDown://返回按下状态(持续)
    if (KeyTemp != keyValue)
    {
      State = KeyRisEdge;
      KeyNum->lastValue = KeyTemp;
    }

    break;

  case KeyRisEdge://按键上升沿信号(一次)
    State = CheckKey;
    break;

  default:
    break;
  }

  KeyNum->State = State;
  KeyNum->Value = keyValue;
  return State;
}
