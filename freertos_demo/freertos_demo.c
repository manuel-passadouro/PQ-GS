#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "led_task.h"
#include "switch_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//Includes
#include <string.h>
#include "custom_libraries/LCD.h"
#include "custom_libraries/KEYPAD.h"
#include "custom_libraries/TMP100.h"
#include "custom_libraries/TIME.h"
#include "custom_libraries/INIT_PERIPHERALS.h"
#include "custom_libraries/CUSTOM_TASK.h"
#include "driverlib/timer.h"

// The mutex that protects concurrent access of UART from multiple tasks.
//xSemaphoreHandle g_pUARTSemaphore;

// This hook is called by FreeRTOS when an stack overflow error is detected.
void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    while(1)
    {
    }
}

// Initialize FreeRTOS and start the initial set of tasks.

char UART_buffer[BUFFER_SIZE][MSG_SIZE];
int buffer_head;
int buffer_tail;
int num_msgs = 0;

struct tm start_time;

//SemaphoreHandle_t bufferMutex;

int main(void)
{
    // Create a mutex for protecting shared resources
    //bufferMutex = xSemaphoreCreateMutex();

    //Define the frequency of the Clock
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    memset(UART_buffer, 0, sizeof(UART_buffer)); //Init buffer at 0.
    buffer_head = 0; //Set head of buffer to 0.

    Init_Peripherals();

    //Create a binary semaphore for protecting shared resources (lcdQueue, in keep message mode)
    xSemaphore_Keep_Message = xSemaphoreCreateBinary();

    //Create a binary semaphore for protecting shared resources (lcdQueue, to allow Temperature)
    xSemaphore_Allow_Temperature = xSemaphoreCreateBinary();

    //Create a mutex semaphore for protecting shared resources (lcdQueue by tmp100, keypad and lcd)
    xMutex_lcdQueue = xSemaphoreCreateMutex();

    //Create the Lcd task with priority 1 (less priority)
    //xTaskCreate(Lcd_Task, "Lcd_Task", configMINIMAL_STACK_SIZE, NULL, 2, &xLcd_Task);

    //Create the Keypad task with priority 1 (less priority)
    //xTaskCreate(Keypad_Task, "Keypad_Task", configMINIMAL_STACK_SIZE, NULL, 1, &xKeypad_Task);

    //Create the Command task with priority 1 (less priority)
    //xTaskCreate(Command_Task, "Command_Task", configMINIMAL_STACK_SIZE, NULL, 1, &xCommand_Task);

    //Create the Command task with priority 1 (less priority)
    xTaskCreate(Tmp_Task, "Tmp_Task", configMINIMAL_STACK_SIZE, NULL, 1, &xTmp_Task);

    //Create the Uart task with priority 1 (less priority)
    xTaskCreate(Uart_Task, "Uart_Task", configMINIMAL_STACK_SIZE, NULL, 1, &xUart_Task);

    // Start the scheduler.  This should not return.
    vTaskStartScheduler();

    while(1)
    {

    }
}

