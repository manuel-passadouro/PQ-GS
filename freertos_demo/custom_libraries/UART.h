#ifndef UART_H
#define UART_H
/* Program Description: UART library for TIVA C
 * devboard (ek-tm4c123gxl) communication with pqube board. */

//Includes
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "inc/hw_i2c.h"
#include "LCD.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

//UART defines
#define BUFFER_SIZE 20
#define MSG_SIZE 50

extern char UART_buffer[BUFFER_SIZE][MSG_SIZE];
extern int buffer_head;
extern int buffer_tail;
extern int num_msgs;
extern char str_msgs;

extern struct tm start_time;

extern SemaphoreHandle_t bufferMutex;


//------------------------------------------------------------------------- PROTOTYPES: ------------------------------------------------------------------------

void Receive_UART(void);
void UART3IntHandler(void);
void StoreInCircularBuffer(const char*);


#endif //UART_H
