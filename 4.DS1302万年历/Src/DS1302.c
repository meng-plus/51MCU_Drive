#include "DS1302.h"
#include<intrins.h>
#define Delay1us()   _nop_()

//---DS1302写入和读取时分秒的地址命令---//
//---秒分时日月周年 最低位读写位;-------//
unsigned  char  code READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d};
unsigned  char  code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};
//---存储顺序是秒分时日月周年,存储格式是用BCD码---//
unsigned  char  TIME[7] = {0, 0, 0x12, 0x01, 0x01, 0x02, 0x13};

void Ds1302Write (unsigned char  addr, unsigned  char  dat);
unsigned char Ds1302Read (unsigned  char  addr);

void Ds1302Init()
{
  unsigned  char  n;
  Ds1302Write (0x8E, 0X00);		 //禁止写保护，就是关闭写保护功能

  for (n = 0; n < 7; n++) //写入7个字节的时钟信号：分秒时日月周年
  {
    //Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);
  }

  Ds1302Write (0x8E, 0x80);		 //打开写保护功能
}

char BCD2HEX (char BCD)
{
  char date;
  date = BCD & 0xf0 * 10 + BCD & 0x0f;
  return date;
}
char HEX2BCD (char HEX)
{
  char date;
  date =  HEX%10+(HEX/10%10)<<4;
  return date;
}
#define _BCD2HEX(BCD) BCD & 0xf0 * 10 + BCD & 0x0f
#define _HEX2BCD(HEX) HEX%10+(HEX/10%10)<<4;
void ReadTime (Ds1302_TypeDef* time)
{
  char tmp;
  /*读取秒*/
  tmp = Ds1302Read (READ_RTC_ADDR[0]);
  time->sec = BCD2HEX (tmp);
  /*读取分*/
  tmp = Ds1302Read (READ_RTC_ADDR[1]);
  time->min = _BCD2HEX (tmp);
  /*读取时*/
  tmp = Ds1302Read (READ_RTC_ADDR[2]);

  if (tmp & 0x80)
  {
    time->mode = (tmp & 0x20)? PM: AM;
    time->hour = tmp & 0x10 * 10 + tmp & 0x0f;
  }
  else
  {
    time->mode = H24;
    time->hour = _BCD2HEX(tmp);
  }

  time->hour = BCD2HEX (tmp);
  /*读取日*/
  tmp = Ds1302Read (READ_RTC_ADDR[3]);
  time->date = BCD2HEX (tmp);
  /*读取月*/
  tmp = Ds1302Read (READ_RTC_ADDR[4]);
  time->mon = BCD2HEX (tmp);
  /*读取周*/
  tmp = Ds1302Read (READ_RTC_ADDR[5]);
  time->day = BCD2HEX (tmp);
  /*读取年*/
  tmp = Ds1302Read (READ_RTC_ADDR[6]);
  time->year = BCD2HEX (tmp);
}
void WriteTime(Ds1302_TypeDef* time)
{
  char tmp;
  /*写入秒*/
  tmp=time->sec;
  tmp=_HEX2BCD(tmp);
  Ds1302Write(WRITE_RTC_ADDR[0],tmp);
  /*写入分*/
  tmp=time->min;
  tmp=_HEX2BCD(tmp);
  Ds1302Write(WRITE_RTC_ADDR[1],tmp);
  /*写入时*/
  tmp=time->hour;
  tmp=_HEX2BCD(tmp);
  if(time->mode !=H24)tmp |=(time->mode =AM)?0 :0x20;
  Ds1302Write(WRITE_RTC_ADDR[2],tmp);
  /*写入日*/
  tmp=time->date;
  tmp=_HEX2BCD(tmp);
  Ds1302Write(WRITE_RTC_ADDR[3],tmp);
  /*写入月*/
  tmp=time->mon;
  tmp=_HEX2BCD(tmp);
  Ds1302Write(WRITE_RTC_ADDR[4],tmp);
  /*写入周*/
  tmp=time->day;
  tmp=_HEX2BCD(tmp);
  Ds1302Write(WRITE_RTC_ADDR[5],tmp);
  /*写入年*/
  tmp=time->year;
  tmp=_HEX2BCD(tmp);
  Ds1302Write(WRITE_RTC_ADDR[6],tmp);

}
void Ds1302Write (unsigned  char  addr, unsigned  char  dat)
{
  unsigned  char  n;
  CE = 0;
  Delay1us();

  SCLK = 0;//先将SCLK置低电平。
  Delay1us();
  CE = 1; //然后将CE(CE)置高电平。
  Delay1us();

  for (n = 0; n < 8; n++) //开始传送八位地址命令
  {
    DSIO = addr & 0x01;//数据从低位开始传送
    addr >>= 1;
    SCLK = 1;//数据在上升沿时，DS1302读取数据
    Delay1us();
    SCLK = 0;
    Delay1us();
  }

  for (n = 0; n < 8; n++) //写入8位数据
  {
    DSIO = dat & 0x01;
    dat >>= 1;
    SCLK = 1;//数据在上升沿时，DS1302读取数据
    Delay1us();
    SCLK = 0;
    Delay1us();
  }

  CE = 0;//传送数据结束
  Delay1us();
}


unsigned char Ds1302Read (unsigned  char  addr)
{
  unsigned  char  n, dat, dat1;
  CE = 0;
  Delay1us();

  SCLK = 0;//先将SCLK置低电平。
  Delay1us();
  CE = 1;//然后将CE(CE)置高电平。
  Delay1us();

  for (n = 0; n < 8; n++) //开始传送八位地址命令
  {
    DSIO = addr & 0x01;//数据从低位开始传送
    addr >>= 1;
    SCLK = 1;//数据在上升沿时，DS1302读取数据
    Delay1us();
    SCLK = 0;//DS1302下降沿时，放置数据
    Delay1us();
  }

  Delay1us();

  for (n = 0; n < 8; n++) //读取8位数据
  {
    dat1 = DSIO;//从最低位开始接收
    dat = (dat >> 1) | (dat1 << 7);
    SCLK = 1;
    Delay1us();
    SCLK = 0;//DS1302下降沿时，放置数据
    Delay1us();
  }

  CE = 0;
  Delay1us();	//以下为DS1302复位的稳定时间,必须的。
  SCLK = 1;
  Delay1us();
  DSIO = 0;
  Delay1us();
  DSIO = 1;
  Delay1us();
  return dat;
}
