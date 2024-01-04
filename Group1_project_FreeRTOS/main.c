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

struct tm start_time;

int main(void)
{

    //Define the frequency of the Clock
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // Initialize FreeRTOS and start the initial set of tasks.

    //Create a binary semaphore for protecting shared resources (lcdQueue, to allow Temperature)
    xSemaphore_Allow_Temperature = xSemaphoreCreateBinary();

    //Create a mutex semaphore for protecting shared resources (lcdQueue, to Access UART Buffer)
    xMutex_Access_UART_Buffer = xSemaphoreCreateMutex();

    //Create a mutex semaphore for protecting shared resources (lcdQueue, to Access UART Buffer)
    xMutex_Access_Num_Msgs = xSemaphoreCreateMutex();


    //Create a mutex semaphore for protecting shared resources (lcdQueue by tmp100, keypad and lcd)
    xMutex_lcdQueue = xSemaphoreCreateMutex();


    // Create the initialization task with the highest priority
    xTaskCreate(System_Init_Task, "System_Init_Task", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, &xSystem_Init_Task);

    // Create the initialization task with the highest priority
    xTaskCreate(System_Init_Task, "System_Init_Task", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 2, &xSystem_Init_Task);

    //Create the Lcd task with low priority
    xTaskCreate(Lcd_Task, "Lcd_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xLcd_Task);

    //Create the Keypad task with low priority
    xTaskCreate(Keypad_Task, "Keypad_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xKeypad_Task);

    //Create the Command task with low priority
    //xTaskCreate(Command_Task, "Command_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xCommand_Task);

    //Create the Temperature task with low priority
    //xTaskCreate(Tmp_Task, "Tmp_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xTmp_Task);

    //Create the Uart task with low priority
    //xTaskCreate(Uart_Task, "Uart_Task", configMINIMAL_STACK_SIZE + 400, NULL, tskIDLE_PRIORITY + 1, &xUart_Task);


    // Start the scheduler.  This should not return.
    vTaskStartScheduler();

    while(1)
    {

    }
}

