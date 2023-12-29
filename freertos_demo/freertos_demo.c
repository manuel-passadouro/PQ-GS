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
#include "driverlib/timer.h"

// The mutex that protects concurrent access of UART from multiple tasks.
xSemaphoreHandle g_pUARTSemaphore;

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

SemaphoreHandle_t bufferMutex;

int main(void)
{
    //Define the frequency of the Clock
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    memset(UART_buffer, 0, sizeof(UART_buffer)); //Init buffer at 0.
    buffer_head = 0; //Set head of buffer to 0.

    Init_Peripherals();

    /* // Create a mutex for protecting shared resources
    bufferMutex = xSemaphoreCreateMutex();*/

    // Create the LCD task
    xTaskCreate(LCDTask, "LCDTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Start the scheduler.  This should not return.
    vTaskStartScheduler();

    // In case the scheduler returns for some reason, print an error and loop
    // forever.

    while(1)
    {

    }
}

