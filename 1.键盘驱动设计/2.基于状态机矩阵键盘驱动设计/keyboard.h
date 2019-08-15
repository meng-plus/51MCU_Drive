#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_


#define MatrixKeyboard P2

typedef struct
{
    unsigned char Value;
    unsigned char State;
    unsigned char lastValue;
} KeyStruct;
typedef enum  {CheckKey,DelayKey,KeyFallEdge,KeyDown,KeyRisEdge}KeyState;

//°´¼ü¼ì²â
KeyState key_scan(KeyStruct* KeyNum);
#endif