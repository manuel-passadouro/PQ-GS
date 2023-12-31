//Includes
#include "TIME.h"

QueueHandle_t lcdQueue;
QueueHandle_t commandQueue;

TaskHandle_t xKeypad_Task;
TaskHandle_t xLcd_Task;
TaskHandle_t xCommand_Task;
TaskHandle_t xTmp_Task;
TaskHandle_t xUart_Task;

SemaphoreHandle_t xSemaphore_Keep_Message;
SemaphoreHandle_t xSemaphore_Allow_Temperature;
SemaphoreHandle_t xMutex_lcdQueue;

//------------------------------------------------------------------- Lcd_Task: -------------------------------------------------------------------

void Lcd_Task(void *pvParameters)
{
    char MsgToWrite[MSG_SIZE];
    int flag_keep_message = 0;

    // Create a queue for communication between tasks
    lcdQueue = xQueueCreate(1, sizeof(char[MSG_SIZE]));

    while (1)
    {
        // Wait for a message with a timeout of 5 seconds
        if (xQueueReceive(lcdQueue, MsgToWrite, 5 * configTICK_RATE_HZ) == pdPASS)
        {

            // Message received within the timeout
            if((strcmp(MsgToWrite, "C") == 0) || (strcmp(MsgToWrite, "E") == 0))
            {
                Lcd_Clear();
            }
            else if(flag_keep_message == 1)
            {
                Lcd_Clear();
                Lcd_Write_String(MsgToWrite);

                flag_keep_message = 0;
            }
            else
            {
                if(xSemaphoreTake(xSemaphore_Keep_Message, 0) == pdTRUE)
                {
                    flag_keep_message = 1;
                }

                Lcd_Write_String(MsgToWrite);
            }
        }
        else
        {
            //Timeout occurred, handle it (clear LCD or do something else)
            Lcd_Clear();

            //Notify the Tmp_Task in order to let it know that can send to lcdQueue again
            xTaskNotifyGive(xTmp_Task);
        }
    }
}


//------------------------------------------------------------------- Keypad_Task: -------------------------------------------------------------------

void Keypad_Task(void *pvParameters)
{
    while(1)
    {
        //Wait to receive the notification from Keypad ISR
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xSemaphoreGive(xSemaphore_Allow_Temperature);

        if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
        {

            //Send the key value to the lcdQueue
            xQueueOverwrite(lcdQueue, &key);

            xSemaphoreGive(xMutex_lcdQueue);
        }

        //Send the key value to the commandQueue
        xQueueOverwrite(commandQueue, &key);
    }
}

//------------------------------------------------------------------- Command_Task: -------------------------------------------------------------------

void Command_Task(void *pvParameters)
{
    char command_buffer[4];
    char str_null[4];
    char parameter[MSG_SIZE];
    int command_buffer_head = 0;

    bool buzzer_toggle = 0;

    //Create a queue for communication between keypad and command tasks
    commandQueue = xQueueCreate(1, sizeof(char[MSG_SIZE]));

    while (1)
    {
        // Wait for a message with a timeout of 5 seconds
        if (xQueueReceive(commandQueue, parameter, portMAX_DELAY) == pdPASS)
        {
            if(strcmp(parameter, "E") == 0)
            {
                if((command_buffer[0] == '1') && (command_buffer_head == 1))
                {
                    buzzer_toggle = !buzzer_toggle;

                    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, buzzer_toggle);

                    if(buzzer_toggle == 1)
                    {
                        if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
                        {
                            xSemaphoreGive(xSemaphore_Keep_Message);
                            xQueueOverwrite(lcdQueue, "Buzzer ON!");

                            xSemaphoreGive(xMutex_lcdQueue);
                        }
                    }
                    else
                    {
                        if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
                        {
                            xSemaphoreGive(xSemaphore_Keep_Message);
                            xQueueOverwrite(lcdQueue, "Buzzer OFF!");

                            xSemaphoreGive(xMutex_lcdQueue);
                        }
                    }
                }
                else if(strcmp(command_buffer, "A") == 0)
                {

                }
                else
                {
                    if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
                    {
                        xSemaphoreGive(xSemaphore_Keep_Message);
                        xQueueOverwrite(lcdQueue, "Invalid Command!");

                        xSemaphoreGive(xMutex_lcdQueue);
                    }
                }

                command_buffer_head = 0;
                strcpy(command_buffer, str_null);
            }
            else if (strcmp(parameter, "C") == 0)
            {
                command_buffer_head = 0;
                strcpy(command_buffer, str_null);
            }
            else
            {
                command_buffer[command_buffer_head++] = parameter[0];
            }
        }
    }
}

//------------------------------------------------------------------- Tmp_Task: -------------------------------------------------------------------

void Tmp_Task(void *pvParameters)
{
    float temperature_raw = 0;
    char temperature_array[20];

    while (1)
    {
        char temperature[20] = "Temp: ";
        char units[3] = " C";

        temperature_raw = TMP100_Read();

        vTaskDelay(2000);

        //Convert the float value of the reading to an array to show in LCD later
        FloatToArray(temperature_raw, temperature_array, 3);

        sprintf(temperature + strlen(temperature), "%s", temperature_array);
        sprintf(temperature + strlen(temperature), "%s", units);

        //0 = Non blocking check
        if(xSemaphoreTake(xSemaphore_Allow_Temperature, 0) == pdTRUE)
        {
            //Checks if it get a notification from lcd timeout

            while(!ulTaskNotifyTake(pdTRUE, portMAX_DELAY))
            {
                temperature_raw = TMP100_Read();
            }

            //ulTaskNotifyTake(pdTRUE,portMAX_DELAY);

        }

        if(xSemaphoreTake(xMutex_lcdQueue, 0) == pdTRUE)
        {
            //Toggle the flag_keep_message to keep the temperature message
            //xSemaphoreGive(xSemaphore_Keep_Message);

            //Send the key value to the lcdQueue
            xQueueOverwrite(lcdQueue, "C");
            xSemaphoreGive(xSemaphore_Keep_Message);
            xQueueOverwrite(lcdQueue, &temperature);

            xSemaphoreGive(xMutex_lcdQueue);
        }
    }
}

//------------------------------------------------------------------- Uart_Task: -------------------------------------------------------------------

void Uart_Task(void *pvParameters)
{
    uint32_t ui32Status;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    while(1)
    {

   //Get the interrupt status (masked interrupt should be RX type)
   ui32Status = UARTIntStatus(UART3_BASE, true);

   //Clear interrupt flag
   UARTIntClear(UART3_BASE, ui32Status);

   //Check if the receive interrupt is triggered (if status matches UART interrupt mask)
   if (ui32Status & UART_INT_RX)
   {

       Lcd_Clear();
       //Lcd_Write_String(current_time_str);
       Lcd_Write_String("AQUI");
       //Receive_UART();
   }

   //To prevent the program to get stuck in the ISR
   portYIELD_FROM_ISR(&xHigherPriorityTaskWoken);

    }
}
