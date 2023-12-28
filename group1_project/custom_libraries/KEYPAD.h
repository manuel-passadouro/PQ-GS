#ifndef KEYPAD_H
#define KEYPAD_H
/* Program Description: Library for interupt configuration for
 * TIVA C (ek-tm4c123gxl) based project. */


//Includes
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "UART.h"
#include "LCD.h"
#include "TMP100.h"


//Some defines to help understanding the code :)
#define GPIO_PIN_ALL 0b1111
#define ON 0x01
#define OFF 0x00
#define LINE_Y1 0x01
#define LINE_Y2 0x02
#define LINE_Y3 0x04
#define LINE_Y4 0x08
#define COLUMN_X1 0x01
#define COLUMN_X2 0x02
#define COLUMN_X3 0x04
#define COLUMN_X4 0x08

extern char UART_buffer[BUFFER_SIZE][MSG_SIZE];


//------------------------------------------------------------------------- PROTOTYPES: ------------------------------------------------------------------------

void PortEIntHandler(void);


#endif //Interrupts_H
