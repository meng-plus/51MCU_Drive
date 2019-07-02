
#ifndef __MULDIGITALTUBE_H__
#define __MULDIGITALTUBE_H__

#define MulDigData P0
#define MulDigCmd  P2


void MulDigInit();//初始化操作
void MulDigPlay();//动态刷新显示

void DigSetBData (char ID, char Num); //设置一位
void DigSetDData (char ID, char Num); //设置两位

#endif