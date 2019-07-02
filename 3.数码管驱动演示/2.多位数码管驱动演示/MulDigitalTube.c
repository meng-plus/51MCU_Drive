#include <REGX52.H>

#include "MulDigitalTube.h"
unsigned char code DIG_CODE[17] =
{
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
  0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0xff
};
////--定义全局变量--//
//unsigned char code DIG_PLACE[8] =
//{
//  0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f
//};//位选控制   查表的方法控制

char DigBuf[8] = {0};
void MulDigInit()//初始化操作
{
  MulDigData = 0x00;
  MulDigCmd = 0x00;

}
void MulDigPlay()//动态刷新显示
{
  char i, j;

  for (i = 0; i < 8; i++)
  {
    //MulDigCmd = 0x00; //关显示
    MulDigData = ~DigBuf[i]; //送数据
    MulDigCmd = 0x01 << i; //开显示
    j = 10;

    while (j--); //显示一段时间

    MulDigCmd = 0x00; //关显示
  }

}

void DigSetBData (char ID, char Num) //设置一位
{
  if (ID > 7)return;

  if (Num > 15) Num = 16;

  DigBuf[ID] = DIG_CODE[Num];
}
void DigSetDData (char ID, char Num) //设置两位
{
  DigSetBData (ID, Num / 10 % 10);
  DigSetBData (ID + 1, Num % 10);
}