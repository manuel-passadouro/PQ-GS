#ifndef CUSTOM_TASK_H
#define CUSTOM_TASK_H

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
#include "INIT_PERIPHERALS.h"

//CUSTOM TASK defines
#define CMD_MAX_SIZE 4
#define DATE_MAX_SIZE 11
#define TIME_MAX_SIZE 9
#define TIMEOUT_5 5*configTICK_RATE_HZ
#define TIMEOUT_7 7*configTICK_RATE_HZ
#define TIMEOUT_8 8*configTICK_RATE_HZ

extern char key[1];

extern TaskHandle_t xKeypad_Task;
extern TaskHandle_t xLcd_Task;
extern TaskHandle_t xCommand_Task;
extern TaskHandle_t xTmp_Task;
extern TaskHandle_t xUart_Task;
extern TaskHandle_t xSystem_Init_Task;

extern SemaphoreHandle_t xSemaphore_Allow_Temperature;
extern SemaphoreHandle_t xMutex_lcdQueue;

//------------------------------------------------------------------------- PROTOTYPES: ------------------------------------------------------------------------

void Lcd_Task(void *pvParameters);
void Keypad_Task(void *pvParameters);
void Command_Task(void *pvParameters);
void Tmp_Task(void *pvParameters);
void Uart_Task(void *pvParameters);
void System_Init_Task(void *pvParameters);
void Date_Time_Task(void *pvParameters);

bool Command_Process(const char *command, bool buzzer_toggle);
bool Date_Process(const char *date_buffer)
bool Time_Process(const char *time_buffer)

#endif // CUSTOM_TASK_H
