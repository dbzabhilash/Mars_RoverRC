/*
 * movement.c
 *
 *  Created on: May 30, 2019
 *      Author: dhphuong
 */
#include "open_interface.h"
#include "lcd.h"

void move(oi_t *sensor, int centimeters){
        oi_update(sensor);
        int total=0;
        oi_setWheels(100,100); //move forward
        while(total<centimeters){
            oi_update(sensor);
             total += sensor->distance;
        }
        oi_setWheels(0, 0); // stop
}
void backward(oi_t *sensor, int centimeters){
    oi_update(sensor);
    int total=0;
    oi_setWheels(-100,-100); //move forward
    while(total>(-1)*centimeters){
        oi_update(sensor);
        total += sensor->distance;
    }
    oi_setWheels(0, 0); // stop
}
void turn_clockwise(oi_t *sensor, int degrees){

    int sum = 0;
    degrees=degrees;      //callibrated for the angle
    oi_setWheels(-100,100); //move clockwise
    while(sum>degrees*(-1)){
        oi_update(sensor);
        sum += sensor->angle;
        lcd_printf("%d",sum);
    }
    oi_setWheels(0,0);

}
void turn_counterclockwise(oi_t *sensor, int degrees){

    int sum = 0;
    degrees=degrees;
    oi_setWheels(100,-100); //move clockwise
    while(sum<degrees){
        oi_update(sensor);
        sum += sensor->angle;
        lcd_printf("%d",sum);
    }
    oi_setWheels(0,0);

}

