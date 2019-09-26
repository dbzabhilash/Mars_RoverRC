

/**
 * main.c
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
#include "function.h"

#define ANGLE_SAMPLES 181
#define dist_range 45

//cyBot 10

struct object{      // struct for object
    unsigned int startAngle;    // start angle
    unsigned int endAngle;    // end angle
    double distance;        // distance
    double width;
    unsigned int index;
};

typedef struct object object_t;

struct distance{        // struct for measurement
    double IR_Dist;
    double PING_Dist;
};

typedef struct distance distance_t;

int i = 0;

distance_t measurements[ANGLE_SAMPLES];

int scanSurface(object_t* objects, distance_t* measurements);
void move_forward(oi_t *sensor);

int main(void){

    // init GPIO and devices
    lcd_init();
    ping_init();
    uart_init();
    adc_init();
    servo_init();
    object_t objects[10];
    oi_t *sensor = oi_alloc();
    oi_init(sensor);
    char notes[2]= {67,69};
    char lengths[2]={30,30};
    sensor->songNumber=8;
    while(1){
        //        char display_message[150];
        for(i= 0; i < ANGLE_SAMPLES; i++){      //initializing for measurement
            measurements[i].IR_Dist = 1;
            measurements[i].PING_Dist = 1;
        }

        for(i= 0; i < 10; i++){                 // initializing for objects
            objects[i].startAngle = 0;
            objects[i].endAngle = 0;
            objects[i].distance = 0;
            objects[i].width = 0;
            objects[i].index = 0;
        }


        switch(uart_receive()){
        case 'm': scanSurface(objects,measurements);     // function to scan
        break;

        case 'w': move_forward(sensor);       //move forward 20cm
        break;

        case 'a':  turn_counterclockwise(sensor,30);     // turn counterclockwise 30 degrees
        uart_sendStr("Turn counterclockwise 30 degrees\n\r");
        break;

        case 'd': turn_clockwise(sensor,30);     // turn counterclockwise 30 degrees
        uart_sendStr("Turn clockwise 30 degrees\n\r");
        break;

        case 'e': turn_clockwise(sensor,15);     // turn counterclockwise 15 degrees
        uart_sendStr("Turn clockwise 15 degrees\n\r");
        break;

        case 'q': turn_counterclockwise(sensor,14);     // turn counterclockwise 15 degrees
        uart_sendStr("Turn counterclockwise 15 degrees\n\r");
        break;

        case 'o': turn_counterclockwise(sensor,91);     // turn counterclockwise 90 degrees
        uart_sendStr("Turn counterclockwise 90 degrees\n\r");
        break;

        case 'p': turn_clockwise(sensor,91);     // turn clockwise 90 degrees
        uart_sendStr("Turn clockwise 90 degrees\n\r");
        break;

        case 'y':     oi_update(sensor);    //play music
        oi_loadSong(0,2,notes,lengths);
        oi_play_song(0);
        break;
        case 's':    backward(sensor,150);
        uart_sendStr("Moved 15cm backward\n\r");
                break;

        default: break;
        }
    }


}
//function to scan surface
int scanSurface(object_t* objects, distance_t* measurements)// (object_t* objects)
{
    int degree = 0;

    char first_line[150];
    char display_message[150];

    volatile unsigned clock_cycle = 0;

    int adc_result = 0;
    double power = -1.138;//-1.331
    double newX = 0.0;
    double base = 96164; //317134
    double ADC_distance = 0.0;

    int pulseWidth = 0;
    float ping_distance = 0.0;

    if(degree > 180)        // prevent servo to move over 0 or 180
    {
        degree = 180;
    }
    else if(degree < 0)
    {
        degree = 0;
    }
    clock_cycle = move_servo(degree);
    timer_waitMillis(700);
    for(degree = 0; degree <=180; degree++)
    {
        clock_cycle = move_servo(degree);

        if(degree == 0){            // print header to putty
            sprintf(first_line,"Degree\tIR Distance (cm)\tSonar Distance\n\r");
            uart_sendStr(first_line);
        }

        adc_result = adc_read();        // read IR value

        pulseWidth = ping_read();       // read Ping value

        newX = pow(adc_result,power);
        ADC_distance = newX * base;
        measurements[degree].IR_Dist = ADC_distance;

        return_overflow();     //receive overflow count
        ping_distance = pulseWidth*(62.5e-7)*340/2;
        measurements[degree].PING_Dist = ping_distance;


        timer_waitMillis(10);

        sprintf(display_message," %d\t%.1f\t\t\t%.1f\r\n",degree,ADC_distance,ping_distance);
        uart_sendStr(display_message);

    }

    int detected = 0;
    int obj_count = 0;
    float totalDistance = 0.0;
    int totalSamples = 0;
    float newDistance = 0.0;
    int corendAngle = 0;
    float arcRadius = 0.0;
    int smallestIndex = 0;
    int displayIndex = 0;
    float displayDistance = 0.0;
    float displayLinearWidth = 0.0;
    int i=1;

    for(degree = 0; degree<=180; degree++)
    {
        if(measurements[degree].IR_Dist < dist_range)
        {
            if(!detected)           // detected var=0, then increment obj_count
            {
                objects[obj_count].startAngle = degree;     //take in start Angle
                objects[obj_count].index = obj_count + 1;
                detected = 1;           // set detect to 1
            }
            totalDistance += measurements[degree].IR_Dist;    // total distance to find the average distance
        }
        else if(detected == 1)      // if already detect, then update end angle
        {
            objects[obj_count].endAngle = degree;
            totalSamples =objects[obj_count].endAngle - objects[obj_count].startAngle;      // No of samples = the difference in angle

            if(totalSamples > 0)
            {
                newDistance = totalDistance / totalSamples;         // average distance
                corendAngle = objects[obj_count].endAngle - objects[obj_count].startAngle;      // angle of object
                arcRadius = newDistance * corendAngle * M_PI / 180;     //calculate linear width of object
                objects[obj_count].width = arcRadius;
                objects[obj_count].distance = newDistance;
                detected = 0;

                if(arcRadius < objects[smallestIndex].width)    // find smallest object
                {
                    smallestIndex = obj_count;
                }

                obj_count++;
            }
            totalDistance = 0;
        }
    }

    displayIndex = objects[smallestIndex].index;
    displayDistance =objects[smallestIndex].distance;//measurements[smallestDegree].IR_Dist;
    displayLinearWidth = objects[smallestIndex].width;
    if(obj_count==0){
        //        lcd_printf("No object found");
        sprintf(display_message,"No object found\n\r");
        uart_sendStr(display_message);
    }
    else{
        //        lcd_printf("Object : %d\nDistance : %.1f\nWidth : %.1f\n# of Objects : %d",displayIndex,displayDistance, displayLinearWidth, obj_count);
        //        sprintf(display_message,"Object : %d\n\rDistance : %.1f\n\rWidth : %.1f\n\r# of Objects : %d\n\r",displayIndex,displayDistance, displayLinearWidth, obj_count);
        //        uart_sendStr(display_message);
        move_servo(0);
        move_servo((objects[smallestIndex].endAngle + objects[smallestIndex].startAngle)/2);
        for( i=0;i<obj_count;i++){
            sprintf(display_message,"Object: %d\t Angle:%d\t distance: %f\t width: %f\n\r",i+1,(objects[i].endAngle + objects[i].startAngle)/2,objects[i].distance,objects[i].width);
            uart_sendStr(display_message);
        }
    }
    timer_waitMillis(10);

    return 0;
}
