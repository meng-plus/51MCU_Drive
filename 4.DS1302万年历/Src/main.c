#include <REGX52.H>
#include "DS1302.h"

void main()
{
  Ds1302_TypeDef time;
  char flag=1;
  Ds1302Init();//DS1302≥ı ºªØ
  while(1)
  {
     //display();
    if(1==flag){
        ReadTime( &time);
     // changeDisplay(&time);
    }
    if(2==flag)
    {
    WriteTime(&time);
    }

    
  }

}