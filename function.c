/*
 * function.c
 *
 *  Created on: Jul 5, 2019
 *      Author: dhphuong
 */

#include "servo.h"
#include "timer.h"
#include "lcd.h"
#include <math.h>
#include "uart.h"
#include "adc.h"
#include "ping.h"
#include <stdbool.h>
#include "stdint.h"
#include "stdio.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "movement.h"
#include "open_interface.h"

#define ANGLE_SAMPLES 181
#define dist_range 45

//function to move forward
void move_forward(oi_t *sensor){
    char display_message[150];
    enum obstruct{NoBump, BumpedLeft, BumpedRight, CliffLeft, CliffFrontLeft, CliffRight, CliffFrontRight, LeftWhite, RightWhite, FrontLeftWhite, FrontRightWhite};
    enum obstruct Status=NoBump;

    oi_update(sensor);
    float sum=0;
    oi_setWheels(100,100); //move forward
    while((sum<150)&&(Status==NoBump)){     //move forward 30cm
        oi_update(sensor);
        sum += sensor->distance;
        ///////////////////
        if(sensor->bumpLeft ){    // if bump into left sensor
            backward(sensor,50);
//            turn_clockwise(sensor,91);
            sprintf(display_message,"Bumped left.\n\rMoved %f cm\n\r",(sum-50)/10);
            uart_sendStr(display_message);
            Status=BumpedLeft;
        }
        else if(sensor->cliffFrontLeft ){    // if bump into left sensor
            backward(sensor,50);
            turn_clockwise(sensor,91);
            sprintf(display_message,"Cliff Front left.\n\rMoved %f cm\n\r",(sum-50)/10);
            uart_sendStr(display_message);
            Status=CliffFrontLeft;
        }

        else if(sensor->cliffLeft ){    // if bump into left sensor
            backward(sensor,50);
            turn_clockwise(sensor,30);
            sprintf(display_message,"Cliff left.\n\rMoved %f cm\n\r",(sum-50)/10);
            uart_sendStr(display_message);
            Status=CliffLeft;
        }
        ///////////////////
        ///////////////////
        else if(sensor->bumpRight ){ //if bump into right sensor
            backward(sensor,50);
//            turn_counterclockwise(sensor,91);
            sprintf(display_message,"Bumped right.\n\rMoved %f cm\n\r",(sum-50)/10);
            uart_sendStr(display_message);
            Status= BumpedRight;
        }
        else if(sensor->cliffFrontRight ){ //if cliff front right
            backward(sensor,50);
            turn_counterclockwise(sensor,91);
            sprintf(display_message,"Cliff Front Right.\n\rMoved %f cm\n\r",(sum-50)/10);
            uart_sendStr(display_message);
            Status=CliffFrontRight ;
        }
        else if(sensor->cliffRight ){ //if cliff right
            backward(sensor,50);
            turn_counterclockwise(sensor,30);
            sprintf(display_message,"White tape on right.\n\rMoved %f cm\n\r",(sum-50)/10);
            uart_sendStr(display_message);
            Status= CliffRight;
        }
        ///////////////////
        ///////////////////
        else if((sensor->cliffLeftSignal)>2600){    // white tape detected on the left
            backward(sensor,50);
            turn_clockwise(sensor,91);
            sprintf(display_message,"White tape on left.\n\rMoved %f cm\n\r",(sum-50)/10);
            uart_sendStr(display_message);
            Status=LeftWhite;
        }
        else if((sensor->cliffRightSignal)>2600){    // white tape detected on the right
            backward(sensor,50);
            turn_counterclockwise(sensor,91);
            sprintf(display_message,"White tape on right.\n\rMoved %f cm\n\r",(sum-50)/10);
            uart_sendStr(display_message);
            Status=RightWhite;
        }
        else if((sensor->cliffFrontLeftSignal)>2600){    // white tape detected on the left
            backward(sensor,50);
            turn_clockwise(sensor,91);
            sprintf(display_message,"White tape on front left.\n\rMoved %f cm\n\r",(sum-50)/10);
            uart_sendStr(display_message);
            Status=FrontLeftWhite;
        }
        else if((sensor->cliffFrontRightSignal)>2600){    // white tape detected on the left
            backward(sensor,50);
            turn_counterclockwise(sensor,91);
            sprintf(display_message,"White tape on front right.\n\rMoved %f cm\n\r",(sum-50)/10);
            uart_sendStr(display_message);
            Status=FrontRightWhite;
        }
    }

    ///////////////////
    ///////////////////



    oi_setWheels(0,0);
    if(Status==NoBump){
        sprintf(display_message,"Moved %fcm\n\r",sum/10);
        uart_sendStr(display_message);
    }


}








