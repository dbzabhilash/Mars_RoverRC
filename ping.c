#include <ping.h>
#include "timer.h"

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80
#define PB3 BIT3

volatile unsigned currentTime = 0;
volatile unsigned lastTime = 0;
volatile int updateFlag = 0;
volatile unsigned int pulse = 0;
int num_overflows = 0;

void ping_init()
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;                        //Enable port B system clock
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCGPIO_R3;                       //Enable timer 3 system clock

    /*Set up GPIO PB3*/
    //    GPIO_PORTB_AFSEL_R |= BIT3;                                     //Enable alternate function for PB3
    //    GPIO_PORTB_PCTL_R |= 0x00007000;                                //Enable timer function at PB3
    GPIO_PORTB_DEN_R |= BIT3;                                       //Enable digital function
    GPIO_PORTB_DIR_R &= ~(BIT3);                                     //Set PB3 as input / Clear PB3 direction

    /* Set up TIMER3 for edge detect mode */
    TIMER3_CTL_R &= ~TIMER_CTL_TBEN;                                // Disable while we set up
    TIMER3_CFG_R = TIMER_CFG_16_BIT;                                // Configure for 16 bit mode
    TIMER3_TBMR_R = 0x00;                                           // Reset TIMER3B mode
    TIMER3_TBMR_R |= TIMER_TBMR_TBMR_CAP;                           // Set to input capture mode
    TIMER3_TBMR_R |= TIMER_TBMR_TBCMR;                              // Set to edge-time mode
    TIMER3_TBMR_R |= TIMER_TBMR_TBCDIR;                             // Set to count up mode
    TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;                         // Set to capture on both edges
    TIMER3_TBPR_R = 0xFF;                                           // Use full prescaler for 24 bits
    TIMER3_TBILR_R = 0xFFFE;                                        // Use the full counter range b
    TIMER3_IMR_R |= TIMER_IMR_CBEIM;                                // Allow interrupts from edge events
    TIMER3_ICR_R |= TIMER_ICR_CBECINT;                              // Ensure interrupt flag is clear

    NVIC_EN1_R |= (0x10);                                           // Enable interrupts from TIMER3B
    NVIC_PRI9_R |= (0X20);

    TIMER3_CTL_R |= TIMER_CTL_TBEN;                                 // Enable TIMER3

    IntRegister(INT_TIMER3B, TIMER3B_HANDLER);                      // Bind the ISR
    IntMasterEnable();

}
int ping_read(void) {
    //sends a pulse
    send_pulse();
    //waits for it to be complete
    while(updateFlag !=2)
    {

    }
    //calculate dist
    if (currentTime < lastTime) {
        num_overflows++;
        pulse = currentTime + ((16777215*num_overflows) - lastTime);

    } else {
        pulse = currentTime - lastTime;
    }

    // Let the ISR know we're expecting a rising edge
    updateFlag = 0;

    // Clear interrupt flag
    TIMER3_ICR_R |= TIMER_ICR_CBECINT;

    // Reenable interrupts from TIMER3B
    TIMER3_IMR_R |= TIMER_IMR_CBEIM;

    return pulse;

}
void send_pulse(void)
{
    //part1
    //        GPIO_PORTB_DATA_R &= 0b11110111;
    //        timer_waitMillis(50);
    //        GPIO_PORTB_DATA_R |= 0b00001000;
    //        timer_waitMillis(50);

    GPIO_PORTB_AFSEL_R &= ~BIT3;                                  //Disable alternate function for PB3
    GPIO_PORTB_PCTL_R &= ~(0x00007000);                           //Disable timer function at PB3
    GPIO_PORTB_DIR_R |= BIT3;                                     //Set PB3 as output / Clear PB3 direction

    GPIO_PORTB_DATA_R &= 0b11110111;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R |= 0b00001000;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= 0b11110111;
    timer_waitMicros(5);

    GPIO_PORTB_AFSEL_R |= BIT3;                                     //Enable alternate function for PB3
    GPIO_PORTB_PCTL_R |= 0x00007000;                                //Enable timer function at PB3
    GPIO_PORTB_DIR_R &= ~(BIT3);                                    //Set PB3 as input / Clear PB3 direction



}
void TIMER3B_HANDLER(void)
{
    // If an edge event triggered this interrupt
    if (TIMER3_MIS_R & TIMER_MIS_CBEMIS) {
        TIMER3_ICR_R |= TIMER_ICR_CBECINT; // Clear the interrupt flag
        if (updateFlag==0) {
            lastTime = TIMER3_TBR_R;
            updateFlag = 1; // Next trigger will be a falling edge
        } else if (updateFlag==1){
            currentTime= TIMER3_TBR_R;

            updateFlag = 2; // Next trigger will be a rising edge
        }
    }

}
int return_overflow(void){
    return num_overflows;
}
