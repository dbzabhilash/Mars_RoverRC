/*
 * servo.h
 *
 *  Created on: Jun 21, 2019
 *      Author: bashirm
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "timer.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

void servo_init(void);
int move_servo(float);

#endif /* SERVO_H_ */
