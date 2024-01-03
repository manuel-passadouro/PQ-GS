//Includes
#include "TIME.h"

QueueHandle_t lcdQueue;
QueueHandle_t commandQueue;

TaskHandle_t xKeypad_Task;
TaskHandle_t xLcd_Task;
TaskHandle_t xCommand_Task;
TaskHandle_t xTmp_Task;
TaskHandle_t xUart_Task;
TaskHandle_t xSystem_Init_Task;

SemaphoreHandle_t xSemaphore_Allow_Temperature;
SemaphoreHandle_t xMutex_lcdQueue;

int flag_next_key_clear = 0;

//------------------------------------------------------------------- Lcd_Task: -------------------------------------------------------------------

void Lcd_Task(void *pvParameters)
{
    char MsgToWrite[MSG_SIZE];
    int flag_next_key_clear = 0;

    // Create a queue for communication between tasks
    lcdQueue = xQueueCreate(1, sizeof(char[MSG_SIZE]));

    while (1)
    {
        // Wait for a message with a timeout of 5 seconds
        if(xQueueReceive(lcdQueue, MsgToWrite, TIMEOUT_5) == pdPASS)
        {
            // Message received within the timeout
           if((strcmp(MsgToWrite, "C") == 0) || (strcmp(MsgToWrite, "E") == 0)) //These keys always clear LCD.
           {
               Lcd_Clear();
           }
           else if(strlen(MsgToWrite) > 1) //If msg is cmd output or temp output string, the next letter from keypad should clear the lcd.
           {
               flag_next_key_clear = 1;
               Lcd_Clear();
               Lcd_Write_String(MsgToWrite);
           }
           else
           {
               if(flag_next_key_clear) //If previous msg was cmd or temp string, key clears LCD.
               {
                   Lcd_Clear();

                   flag_next_key_clear = 0;
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
            //Checks if it get a notification from LCD timeout
            while(!ulTaskNotifyTake(pdTRUE, portMAX_DELAY))
            {
                //Get temperature even if it cannot send to LCD yet
                temperature_raw = TMP100_Read();
            }
        }

        if(xSemaphoreTake(xMutex_lcdQueue, 0) == pdTRUE)
        {
            //Send temperature info LCD queue.
            xQueueOverwrite(lcdQueue, &temperature);

            xSemaphoreGive(xMutex_lcdQueue);
        }
    }
}

//---------------------------------------------------------------------- Uart_Task: -------------------------------------------------------------------

void Uart_Task(void *pvParameters)
{
    while(1)
    {
        //Wait to receive the notification from Keypad ISR
        if(ulTaskNotifyTake(pdTRUE, TIMEOUT_8) == pdPASS)
        {
            Receive_UART();
        }
        else
        {
            //Timeout occurred, handle it (clear LCD or do something else)
            Lcd_Clear();

            Lcd_Write_String("PACKET MISSED! ");

            Init_Uart();
        }

    }
}

//------------------------------------------------------------------- System_Init_Task: -------------------------------------------------------------------

void System_Init_Task(void *pvParameters)
{
    //Initialize system peripherals
    Init_Peripherals();

    //Suspend the initialization task (it will not run again)
    vTaskSuspend(NULL);
}

//------------------------------------------------------------------- Command_Task: -------------------------------------------------------------------

void Command_Task(void *pvParameters)
{
    char key_input;
    char cmd_buffer[CMD_MAX_SIZE] = "";
    bool buzzer_toggle = 0;

    //Create a queue for communication between keypad and command tasks
    commandQueue = xQueueCreate(1, sizeof(char)); //Already created in Date_Time_Task

    while (1)
    {
        // Wait for a message with a timeout of 5 seconds
        if(xQueueReceive(commandQueue, &key_input , TIMEOUT_5) == pdPASS)
        {
            //If enter is pressed (user has submitted command)
            if(key_input == 'E')
            {
                //Call the Command_Process function store the state of the buzzer
                buzzer_toggle = Command_Process(cmd_buffer, buzzer_toggle);

                //Clear the command buffer
                memset(cmd_buffer, 0, sizeof(cmd_buffer));
            }
            else if(key_input == 'C')
            {
                //If cancel is pressed (user has cancelled input)
                //Clear the command buffer
                memset(cmd_buffer, 0, sizeof(cmd_buffer));
            }
            else
            {
                if((strlen(cmd_buffer)) < (CMD_MAX_SIZE - 1))
                {
                    //Check if there is enough space in the cmd_buffer
                    //Add the key to the end of the command buffer
                    cmd_buffer[strlen(cmd_buffer)] = key_input;

                    //Null character - terminate the string
                    cmd_buffer[strlen(cmd_buffer) + 1] = '\0';
                }
                else
                {
                    //Handle buffer overflow (invalidate buffer in case of user write more than 3 character)
                    cmd_buffer[0] = 'X';
                }
            }
        }
        else
        {
            //Input timeout occurred, clear the command buffer
            memset(cmd_buffer, 0, sizeof(cmd_buffer));
        }
    }
}

//------------------------------------------------------------------- Command_Process: -------------------------------------------------------------------

bool Command_Process(const char *cmd_buffer, bool buzzer_toggle)
{
    if(strcmp(cmd_buffer, "1") == 0)
    {
        buzzer_toggle = !buzzer_toggle;

        PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, buzzer_toggle);

        if(buzzer_toggle == 1)
        {
            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
            {
                xQueueOverwrite(lcdQueue, "Buzzer ON!");

                xSemaphoreGive(xMutex_lcdQueue);
            }
        }
        else
        {
            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
            {
                xQueueOverwrite(lcdQueue, "Buzzer OFF!");

                xSemaphoreGive(xMutex_lcdQueue);
            }
        }
    }
    else if(strcmp(cmd_buffer, "A") == 0)
    {
        //Show number of PQube packets received
        //Test print
        if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
        {
            xQueueOverwrite(lcdQueue, "Number Packages: ");

            xSemaphoreGive(xMutex_lcdQueue);
        }
    }
    else if(strcmp(cmd_buffer, "000") == 0)
    {
        //Reset System

        //Test print
        if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
        {
            xQueueOverwrite(lcdQueue, "Reset System!");

            xSemaphoreGive(xMutex_lcdQueue);
        }
    }
    else if(cmd_buffer[0] == 'B' && (strlen(cmd_buffer) == 3))
    {
        //Show PQcube packet BXX
        //Extract the packet number part from the command string
        const char *packet_index_str = &cmd_buffer[1];

        //Convert the indexString to an integer
        int packet_index = atoi(packet_index_str);

        if(packet_index >= 1 && packet_index <= BUFFER_SIZE)
        {
            //Fetch packet message data according to selected number
            //Example: data = UART_buufer[packet_index - 1][msg_index]
            //Send data to LCD_queue

            //Test print
            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
            {
                xQueueOverwrite(lcdQueue, "Package XX: ");

                xSemaphoreGive(xMutex_lcdQueue);
            }
        }
        else
        {
            //Packet number is invalid, write message to LCD
            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
            {
                xQueueOverwrite(lcdQueue, "Invalid packet num!");

                xSemaphoreGive(xMutex_lcdQueue);
            }
        }
    }
    else if(cmd_buffer[0] == 'F' && (strlen(cmd_buffer) == 3))
    {
        //Show PQcube packet RSSI & SNR FXX
        //Extract the packet number part from the command string
        const char *packet_index_str = &cmd_buffer[1];

        // Convert the indexString to an integer
        int packet_index = atoi(packet_index_str);

        //check if requested packet makes sense
        if (packet_index >= 1 && packet_index <= BUFFER_SIZE)
        {
            //Fetch packet RSSI & SNR data according to selected number
            //Example: data = UART_buufer[packet_index - 1][rssi_snr_index]
            //Send data to LCD_queue

            //Test print
            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
            {
                xQueueOverwrite(lcdQueue, "RSSI:   SNR: ");

                xSemaphoreGive(xMutex_lcdQueue);
            }
        }
        else
        {
            //Packet number is invalid, write message to LCD
            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
            {
                xQueueOverwrite(lcdQueue, "Invalid Packet Number!");

                xSemaphoreGive(xMutex_lcdQueue);
            }
        }
    }
    else
    {
        //Input is not a recognized command
        if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
        {
            xQueueOverwrite(lcdQueue, "Invalid Command!");

            xSemaphoreGive(xMutex_lcdQueue);
        }
    }

    return buzzer_toggle;
}

//------------------------------------------------------------------- Date_Time_Task: -------------------------------------------------------------------

void Date_Time_Task(void *pvParameters)
{
    //Create a queue for communication between keypad and command tasks
    commandQueue = xQueueCreate(1, sizeof(char)); //This queue will be used by command_task after start-up
    char date_buffer[DATE_MAX_SIZE] = "";
    char time_buffer[TIME_MAX_SIZE] = "";
    char key_input;
    bool date_ok;
    bool time_ok;

    while (1)
    {
        //Write Initial message (LCD must be running)
        if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
        {
            xQueueOverwrite(lcdQueue, "Date? MM/DD/YYYY");

            xSemaphoreGive(xMutex_lcdQueue);
        }
        //Start time loop
        while(1)
        {
            //Start date loop
            while(1)
            {
                // Wait for a message with a timeout of 5 seconds
                if(xQueueReceive(commandQueue, &key_input , TIMEOUT_5) == pdPASS)
                {
                    //If enter is pressed (user has submitted command)
                    if(key_input == 'E')
                    {
                        //Call the Date_Process function store t
                        date_ok = Date_Process(date_buffer);

                        //Clear the command buffer
                        memset(date_buffer, 0, sizeof(date_buffer));

                        //if date ok, exit date loop
                        if(date_ok){
                            break;
                        }
                        //if date not ok, print error msg and stay in date loop.
                        else{
                            //Clear the date buffer
                            memset(date_buffer, 0, sizeof(date_buffer));
                            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
                            {
                                xQueueOverwrite(lcdQueue, "Invalid Date!");

                                xSemaphoreGive(xMutex_lcdQueue);
                            }
                            //Delay so user can see error message
                            vTaskDelay(3000);

                            //Write Initial message again
                            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
                            {
                                xQueueOverwrite(lcdQueue, "Date? MM/DD/YYYY");

                                xSemaphoreGive(xMutex_lcdQueue);
                            }
                        }
                    }
                    else if(key_input == 'C')
                    {
                        //If cancel is pressed (user has cancelled input)
                        //Clear the command buffer
                        memset(date_buffer, 0, sizeof(date_buffer));

                        //Write Initial message again
                        if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
                        {
                            xQueueOverwrite(lcdQueue, "Date? MM/DD/YYYY");

                            xSemaphoreGive(xMutex_lcdQueue);
                        }
                    }
                    else
                    {
                        //replace "A" with "/" because keypad has no "/".
                        if(key_input == 'A'){
                            key_input = '/';
                        }
                        if((strlen(cmd_buffer)) < (CMD_MAX_SIZE - 1))
                        {
                            //Check if there is enough space in the cmd_buffer
                            //Add the key to the end of the command buffer
                            date_buffer[strlen(date_buffer)] = key_input;

                            //Null character - terminate the string
                            date_buffer[strlen(date_buffer) + 1] = '\0';
                        }
                        else
                        {
                            //Handle buffer overflow (invalidate buffer in case of user write more than 3 character)
                            date_buffer[0] = 'X';
                        }

                    }

                }
                else
                {
                    //Input timeout occurred, clear the command buffer
                    memset(date_buffer, 0, sizeof(date_buffer));
                }

            }

        }

        //THIS NEED TO COME AFTER THE DATE IS ENTERED
        //This task runs only once, after its done it creates the remaining tasks, thus completing system init.

        //Create the Command task with low priority
        xTaskCreate(Command_Task, "Command_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xCommand_Task);

        //Create the Command task with low priority
        xTaskCreate(Tmp_Task, "Tmp_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xTmp_Task);

        //Create the Uart task with low priority
        xTaskCreate(Uart_Task, "Uart_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xUart_Task);

        //Set flag to warn LCD_task of task completion (use binary semaphore?)
        //Time_Date_Task_Done = 1;

        // Suspend the task (it will not run again)
        vTaskSuspend(NULL);

    }

}

//------------------------------------------------------------------- Date_Process: -------------------------------------------------------------------

void Date_Process(const char *date_buffer)
{

}
