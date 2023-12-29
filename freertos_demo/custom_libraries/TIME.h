#ifndef TIME_H
#define TIME_H
/* Program Description: Data and time library for tiva c based project.*/

//Includes
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "LCD.h"

//------------------------------------------------------------------------- PROTOTYPES: ------------------------------------------------------------------------

struct tm getTime(void);

#endif // TIME_H
