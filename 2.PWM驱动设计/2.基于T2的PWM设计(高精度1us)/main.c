#include <REGX52.H>
#include "T2PWM.h"

void main()
{
  /*各种外设初始化*/
  T2_Init();
  
  SetDutyCycle(0.5);//占空比 百分比设定
  changPWMValue(10000);
  SetMotoangle(90);
  /*循环执行过程*/
  while(1)
  {
  
  }

}