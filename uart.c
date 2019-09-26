/*
*
*   uart.c
*
*
*
*
*
*   @author
*   @date
*/

#include "uart.h"
#include "lcd.h"

#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80

#include <math.h>
#include "adc.h"


void uart_init(void){
	//TODO
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;                            //ENABLE clock to GPIO port B

    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;                            //ENABLE clock to UART1

    GPIO_PORTB_AFSEL_R |=(BIT0|BIT1);                                   //ENABLE function in port B pin 0 and pin 1
    GPIO_PORTB_PCTL_R |=0x00000011;                                     //ENABLE Rx and Tx in port B pin 0 and pin 1
    GPIO_PORTB_DEN_R |= (BIT0|BIT1);                                    //Set port B pin 0 and pin 1 to digital
    GPIO_PORTB_DIR_R &= ~BIT0;                                          //Set port B pin 0 to input
    GPIO_PORTB_DIR_R |= BIT1;                                           //Set port B pin 1 to output

    uint16_t iBRD = 8;                                                //Calculate Baud Rate for integer part
    uint16_t fBRD = 44;                                                 //Calculate Baud Rate for fractional part

    UART1_CTL_R &=  ~(UART_CTL_UARTEN);                                 //DISABLE UART1 while set up
    UART1_ICR_R |= (UART_ICR_TXIC|UART_ICR_RXIC);
    UART1_IM_R |= (UART_IM_TXIM|UART_IM_RXIM);
    NVIC_PRI1_R|= 0x00200000;
    NVIC_EN0_R |= 0x00000040;
    IntRegister(INT_UART1, UART1_Handler);                              //Interrupt connect to handler
    IntMasterEnable();
    UART1_IBRD_R = iBRD;                                               //Set integer Baud rate
    UART1_FBRD_R = fBRD;                                               //Set fractional Baud rate
    UART1_LCRH_R |= UART_LCRH_WLEN_8 ;                                  //Set the data length as 8 bit(lowest data length is 1 byte eg. char), start and stop bits are default
    UART1_CC_R |= UART_CC_CS_SYSCLK;                                    //Set up the system clock
    UART1_CTL_R |=(UART_CTL_RXE|UART_CTL_TXE|UART_CTL_UARTEN);          //ENABLE Function: Receive, Transmit,UART1

}

void uart_sendChar(char data){
	//TODO
    while(UART1_FR_R & UART_FR_TXFF)
    {

    }
    UART1_DR_R=data;
}

char uart_receive(void){
	//TODO
    char data = 0;
    while(UART1_FR_R & UART_FR_RXFE)
    {

    }
    data = (char)(UART1_DR_R &0xFF);

    return data;
}

void uart_sendStr(const char *data){
	//TODO for reference see lcd_puts from lcd.c file
    int i = 0;

    while(data[i]!='\0')
    {
        uart_sendChar(data[i]);
        i++;
    }
}
//part3
void UART1_Handler(void)                                    //function that connect to putty with wifi
{
   if(UART1_MIS_R & UART_MIS_RXMIS)
   {
//       double result ;
//       double newResult = 0;
//       char lcdMessage1[22];
//       char lcdMessage2[22];
//       double power = -1.331;
//       double base = 317134;
//       double newX = 0;
//       int i = 0;
//
//       result = adc_read();
//       timer_waitMillis(1000);
//
//       newX = pow(result,power);
//       newResult = newX * base;
//       sprintf(lcdMessage2,"%.3f :%.3f cm",result,newResult);
//       sprintf(puttyMessage,"%.3f :%.3f cm",result,newResult);
//       lcd_printf(puttyMessage);


       UART1_ICR_R|=UART_ICR_RXIC;
   }
   else if(UART1_MIS_R & UART_MIS_TXMIS)
   {
       //TODO
       UART1_ICR_R |=UART_ICR_TXIC;
   }
}

