
#ifndef __T2PWM_H__
#define __T2PWM_H__

#define T2_PWMout P2_0  //PWM输出引脚


void T2_Init();

//设置站空比例
void SetDutyCycle (float value);
//设置占空比
void changPWMValue (unsigned int value);
//设定舵机角度
void SetMotoangle (float angle);
#endif