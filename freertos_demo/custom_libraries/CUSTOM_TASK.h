#ifndef CUSTOM_TASK_H
#define CUSTOM_TASK_H
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
#include "KEYPAD.h"
#include "TMP100.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"

extern char key[1];

extern TaskHandle_t xKeypad_Task;
extern TaskHandle_t xLcd_Task;
extern TaskHandle_t xCommand_Task;
extern TaskHandle_t xTmp_Task;
extern TaskHandle_t xUart_Task;

extern SemaphoreHandle_t xSemaphore_Keep_Message;
extern SemaphoreHandle_t xSemaphore_Allow_Temperature;
extern SemaphoreHandle_t xMutex_lcdQueue;

//------------------------------------------------------------------------- PROTOTYPES: ------------------------------------------------------------------------

void Lcd_Task(void *pvParameters);
void Keypad_Task(void *pvParameters);
void Command_Task(void *pvParameters);
void Tmp_Task(void *pvParameters);
void Uart_Task(void *pvParameters);

#endif // CUSTOM_TASK_H
