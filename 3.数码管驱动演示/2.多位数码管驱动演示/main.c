#include <REGX52.H>
#include "MulDigitalTube.h"
void main()
{

  MulDigInit();
  DigSetBData (0, 1);
  DigSetDData(3,23);
  while (1)
  {
    MulDigPlay();//¶¯Ì¬Ë¢ÐÂÏÔÊ¾

  }

}