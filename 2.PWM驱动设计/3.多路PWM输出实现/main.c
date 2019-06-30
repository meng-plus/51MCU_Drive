/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 -----------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.GXWMCU.com --------------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序        */
/*---------------------------------------------------------------------*/


/*************  功能说明    **************

使用Timer0模拟16通道PWM驱动程序。

输出为 P1.0 ~ P1.7, P2.0 ~ P2.7, 对应 PWM0 ~ PWM15.

定时器中断频率一般不要超过100KHZ, 留足够的时间给别的程序运行.

本例子使用11.0592MHZ时钟, 25K的中断频率, 250级PWM, 周期为10ms.

中断里处理的时间不超过6us, 占CPU时间大约为15%.

******************************************/

#include    <reg52.h>

#define MAIN_Fosc       12000000UL  //定义主时钟
#define Timer0_Rate     2500       //中断频率


typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

#define Timer0_Reload   (65536UL -(MAIN_Fosc/12 / Timer0_Rate))        //Timer 0 重装值

//************** PWM8 变量和常量以及IO口定义 ***************
//********************  8通道8 bit 软PWM    ********************

#define     PWM_DUTY_MAX    250 // 0~255    PWM周期, 最大255
#define     PWM_ON          1   // 定义占空比的电平, 1 或 0

#define     PWM_OFF         (!PWM_ON)
#define     PWM_ALL_ON      (0xff * PWM_ON)

u8 bdata PWM_temp1,PWM_temp2;       //影射一个RAM，可位寻址，输出时同步刷新
sbit    P_PWM0  =   PWM_temp1^0;    //  定义影射RAM每位对应的IO
sbit    P_PWM1  =   PWM_temp1^1;
sbit    P_PWM2  =   PWM_temp1^2;
sbit    P_PWM3  =   PWM_temp1^3;
sbit    P_PWM4  =   PWM_temp1^4;
sbit    P_PWM5  =   PWM_temp1^5;
sbit    P_PWM6  =   PWM_temp1^6;
sbit    P_PWM7  =   PWM_temp1^7;
sbit    P_PWM8  =   PWM_temp2^0;
sbit    P_PWM9  =   PWM_temp2^1;
sbit    P_PWM10 =   PWM_temp2^2;
sbit    P_PWM11 =   PWM_temp2^3;
sbit    P_PWM12 =   PWM_temp2^4;
sbit    P_PWM13 =   PWM_temp2^5;
sbit    P_PWM14 =   PWM_temp2^6;
sbit    P_PWM15 =   PWM_temp2^7;

u8  pwm_duty;       //周期计数值
u8  pwm[16];        //pwm0~pwm15 为0至15路PWM的宽度值

bit     B_1ms;
u8      cnt_1ms;
u8      cnt_20ms;

/**********************************************/
void main(void)
{
    u8  i;
    TMOD &= ~(1<<2);    // Timer0 set as Timer
    TMOD = TMOD&0x0f0|0x01;      // Timer0 set as 16 bits 
    TH0 = Timer0_Reload / 256;  //Timer0 Load
    TL0 = Timer0_Reload % 256;
    ET0 = 1;        //Timer0 Interrupt Enable
    PT0 = 1;        //高优先级
    TR0 = 1;        //Timer0 Run
    EA = 1;         //打开总中断

    cnt_1ms = Timer0_Rate / 1000;   //1ms计数
    cnt_20ms = 20;
    
    for(i=0; i<16; i++)     pwm[i] = i * 15 + 15;   //给PWM一个初值
    
    while(1)
    {
        if(B_1ms)   //1ms到
        {
            B_1ms = 0;
            if(--cnt_20ms == 0) //PWM 20ms改变一阶
            {
                cnt_20ms = 20;
                for(i=0; i<16; i++) pwm[i]++;
            }
        }
    }
} 


/********************** Timer0 1ms中断函数 ************************/
void timer0 (void) interrupt 1
{
    TH0 = Timer0_Reload / 256;  //Timer0 Load
    TL0 = Timer0_Reload % 256;
    P1 = PWM_temp1;         //影射RAM输出到实际的PWM端口
    P2 = PWM_temp2;
        
    if(++pwm_duty == PWM_DUTY_MAX)      //PWM周期结束，重新开始新的周期
    {
        pwm_duty = 0;
        PWM_temp1 = PWM_ALL_ON; 
        PWM_temp2 = PWM_ALL_ON; 
    }
    ACC = pwm_duty;
    if(ACC == pwm[0])       P_PWM0  = PWM_OFF;  //判断PWM占空比是否结束
    if(ACC == pwm[1])       P_PWM1  = PWM_OFF;
    if(ACC == pwm[2])       P_PWM2  = PWM_OFF;
    if(ACC == pwm[3])       P_PWM3  = PWM_OFF;
    if(ACC == pwm[4])       P_PWM4  = PWM_OFF;
    if(ACC == pwm[5])       P_PWM5  = PWM_OFF;
    if(ACC == pwm[6])       P_PWM6  = PWM_OFF;
    if(ACC == pwm[7])       P_PWM7  = PWM_OFF;
    if(ACC == pwm[8])       P_PWM8  = PWM_OFF;
    if(ACC == pwm[9])       P_PWM9  = PWM_OFF;
    if(ACC == pwm[10])      P_PWM10 = PWM_OFF;
    if(ACC == pwm[11])      P_PWM11 = PWM_OFF;
    if(ACC == pwm[12])      P_PWM12 = PWM_OFF;
    if(ACC == pwm[13])      P_PWM13 = PWM_OFF;
    if(ACC == pwm[14])      P_PWM14 = PWM_OFF;
    if(ACC == pwm[15])      P_PWM15 = PWM_OFF;

    if(--cnt_1ms == 0)
    {
        cnt_1ms = Timer0_Rate / 1000;
        B_1ms = 1;      // 1ms标志
    }
}

