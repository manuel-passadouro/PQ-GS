#ifndef UART_H
#define UART_H
/* Program Description: UART library for TIVA C
 * devboard (ek-tm4c123gxl) communication with pqube board. */

//Includes
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "inc/hw_i2c.h"
#include "LCD.h"

//UART defines
#define buffer_size 40

//------------------------------------------------------------------------- PROTOTYPES: ------------------------------------------------------------------------

void Receive_UART(void);

void UART3IntHandler(void);

#endif //UART_H
