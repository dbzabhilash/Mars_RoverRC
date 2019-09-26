#ifndef PING_H_
#define PING_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "timer.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

void ping_init(void);
int ping_read(void);
void send_pulse(void);
void TIMER3B_HANDLER(void);
int return_overflow(void);

#endif
