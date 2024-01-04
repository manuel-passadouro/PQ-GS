//Group1_project main file.

#include "FreeRTOS.h"
#include "custom_libraries/PRIORITIES.h"
#include "custom_libraries/UART.h"
#include "custom_libraries/LCD.h"
#include "custom_libraries/KEYPAD.h"
#include "custom_libraries/TMP100.h"
#include "custom_libraries/TIME.h"
#include "custom_libraries/INIT_PERIPHERALS.h"
#include "custom_libraries/CUSTOM_TASK.h"

struct tm start_time;

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

int main(void)
{

    //Define the frequency of the Clock
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // Initialize FreeRTOS and start the initial set of tasks.

    // Create the initialization task with the highest priority
    xTaskCreate(System_Init_Task, "System_Init_Task", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, &xSystem_Init_Task);

    // Start the scheduler.  This should not return.
    vTaskStartScheduler();

    while(1)
    {

    }
}

