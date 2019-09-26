#include "adc.h"
#include "Timer.h"

void adc_init(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;                //Enable Port B
    timer_waitMillis(50);
    SYSCTL_RCGCADC_R |= 0x00000001;                         //Enable ADC module 0
    timer_waitMillis(50);
    GPIO_PORTB_AFSEL_R |= 0x00000010;                      //PB 4 correspond to AIN10
    GPIO_PORTB_DEN_R &= ~(0x00000010);                     //Disable the digital function for PB4
    GPIO_PORTB_AMSEL_R |= 0x00000010;                       //0: Analog function is disabled    1: Analog function is enabled
    GPIO_PORTB_ADCCTL_R = 0x00;                             //0: Disable trigger of ADC         1: Enable trigger of ADC

    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;                       //Disable sample sequencer 0
    ADC0_EMUX_R = ADC_EMUX_EM0_PROCESSOR;                   //Default processor
    ADC0_SSMUX0_R |= 0x000A;                                //1010  sample sequence
    ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);    //1st sample interrupt is enable , 1st sample is end of sequence
    ADC0_SAC_R |= ADC_SAC_AVG_64X;                          //110  Control the amount of hardware
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;                        //Enable sample sequence 0

}

int adc_read(void)
{
//    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;
//    ADC0_SSMUX0_R |= channel;
//    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;
    ADC0_PSSI_R = ADC_PSSI_SS0;
    while((ADC0_RIS_R & ADC_RIS_INR0) == 0){

    }
    ADC0_ISC_R=ADC_ISC_IN0;
    return ADC0_SSFIFO0_R;
}

