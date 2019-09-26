/*
 * movement.h
 *
 *  Created on: May 30, 2019
 *      Author: dhphuong
 */
#include "lcd.h"

#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include "open_interface.h"

void turn_clockwise(oi_t *sensor, int degrees);
void backward(oi_t *sensor, int centimeters);
void turn_counterclockwise(oi_t *sensor, int degrees);
void move(oi_t *sensor, int centimeters);

#endif /* MOVEMENT_H_ */
