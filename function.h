/*
 * function.h
 *
 *  Created on: Jul 5, 2019
 *      Author: dhphuong
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_


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

void move_forward(oi_t *sensor);


#endif /* FUNCTION_H_ */
