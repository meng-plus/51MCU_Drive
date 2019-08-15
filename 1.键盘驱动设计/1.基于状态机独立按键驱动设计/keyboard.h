#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_

sbit Key0=P1^0;
sbit Key1=P1^1;
sbit Key2=P1^2;
sbit Key3=P1^3;
sbit Key4=P1^4;
sbit Key5=P1^5;
sbit Key6=P1^6;
sbit Key7=P1^7;

typedef struct
{
  unsigned char Value;
  unsigned char State;
  unsigned char lastValue;
} KeyStruct;
typedef enum
{
  CheckKey,
  DelayKey,
  KeyFallEdge,
  KeyDown,
  KeyRisEdge
} KeyState;

//°´¼ü¼ì²â
KeyState key_scan(KeyStruct* KeyNum);
#endif