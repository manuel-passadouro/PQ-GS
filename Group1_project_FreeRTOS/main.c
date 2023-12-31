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

// Initialize FreeRTOS and start the initial set of tasks.

char UART_buffer[BUFFER_SIZE][MSG_SIZE];
int buffer_head;
int buffer_tail;
int num_msgs = 0;

struct tm start_time;


int main(void)
{

    //Define the frequency of the Clock
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    memset(UART_buffer, 0, sizeof(UART_buffer)); //Init buffer at 0.
    buffer_head = 0; //Set head of buffer to 0.

    Init_Peripherals(); //Place in init_task ????


    //Create a binary semaphore for protecting shared resources (lcdQueue, to allow Temperature)
    xSemaphore_Allow_Temperature = xSemaphoreCreateBinary();

    //Create a mutex semaphore for protecting shared resources (lcdQueue by tmp100, keypad and lcd)
    xMutex_lcdQueue = xSemaphoreCreateMutex();

    // Create the initialization task with the highest priority
    xTaskCreate(Initialization_Task, "Initialization_Task", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, &xinitialization_Task);


    //Create the Lcd task with low priority
    //xTaskCreate(Lcd_Task, "Lcd_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xLcd_Task);

    //Create the Keypad task with low priority
    //xTaskCreate(Keypad_Task, "Keypad_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xKeypad_Task);

    //Create the Command task with low priority
    //xTaskCreate(Command_Task, "Command_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xCommand_Task);

    //Create the Command task with low priority
    //xTaskCreate(Tmp_Task, "Tmp_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xTmp_Task);

    //Create the Uart task with low priority
    xTaskCreate(Uart_Task, "Uart_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xUart_Task);

    // Start the scheduler.  This should not return.
    vTaskStartScheduler();

    while(1)
    {

    }
}

