#ifndef TIME_H
#define TIME_H

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

struct tm getTime(void);

//------------------------------------------------------------------------- PROTOTYPES: ------------------------------------------------------------------------

#endif // TIME_H
