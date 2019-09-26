#include "servo.h"
#include "lcd.h"
#include "timer.h"

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80
#define PB5 BIT5

void servo_init(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    timer_waitMillis(50);
    SYSCTL_RCGCTIMER_R |=SYSCTL_RCGCTIMER_R1;
    timer_waitMillis(50);

    GPIO_PORTB_AFSEL_R |= PB5;
    GPIO_PORTB_PCTL_R |= 0x00700000;
    GPIO_PORTB_DEN_R |= PB5;
    GPIO_PORTB_DIR_R |= PB5;

    TIMER1_CTL_R &= ~(TIMER_CTL_TBEN);          //disable timer to config
    TIMER1_CFG_R |= 0x00000004;                 //Enable the 16 bit
    TIMER1_TBMR_R |= 0x0;
    TIMER1_TBMR_R |= 0xA;                       //periodic and PWM enable
    TIMER1_TBILR_R=0xE200;            // set up the period
      TIMER1_TBPR_R=0x4;                 // set up the period(20ms)

      TIMER1_TBMATCHR_R=49953.6;           // on for 1ms
      TIMER1_TBPMR_R=0x4;
    TIMER1_CTL_R |= TIMER_CTL_TBEN;             //enable timer

}
//cybot 6
int move_servo(float degree)
{
    unsigned int match_value = 0;

//    match_value = (16000 * (19-(degree/180)));//original
    match_value = (-156.4)*degree+49953.6; //cybot 10
    TIMER1_TBMATCHR_R = match_value;//set new match value
//    TIMER1_TBPMR_R = match_value >> 16;

    return match_value;
}
