#include "DigitaTube.h"
#include <REGX52.H>

char DigData=0;
//0~F段码
unsigned char code DIG_CODE[17] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                                   0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
                                  ,0x00};


void  display()//刷新显示
{
  DigIO = ~DIG_CODE[DigData];

}
void DigSetData (char Num) //修改显示
{
    if(Num>15)DigData=16;
  else
  DigData = Num;//(数据范围 0~15)

}