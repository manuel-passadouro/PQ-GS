#ifndef UART_H
#define UART_H

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
#include "TIME.h"
#include "CUSTOM_TASK.h"

//UART defines
#define BUFFER_SIZE 20
#define MSG_SIZE 50
#define NUM_PACKET_STRS 4

extern struct packet UART_buffer[BUFFER_SIZE];
extern int num_msgs;

struct packet
{
    char PQ_ID[9];
    char SENSORS_PREQUELA[19];
    char SENSORS_SEQUELA[6];
    char LEN_RSSI_SNR[15];
    char TIMESTAMP[20];
    char RSSI_SNR[20];
};

//------------------------------------------------------------------------- PROTOTYPES: ------------------------------------------------------------------------

uint8_t Receive_UART(uint8_t buffer_head);
void UART3IntHandler(void);

#endif //UART_H
