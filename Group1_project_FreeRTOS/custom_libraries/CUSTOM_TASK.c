//Includes
#include "TIME.h"

QueueHandle_t lcdQueue;
QueueHandle_t commandQueue;
QueueHandle_t skip_msg_Queue;

TaskHandle_t xKeypad_Task;
TaskHandle_t xLcd_Task;
TaskHandle_t xCommand_Task;
TaskHandle_t xTmp_Task;
TaskHandle_t xUart_Task;
TaskHandle_t xSystem_Init_Task;
TaskHandle_t xDate_Time_Task;

SemaphoreHandle_t xSemaphore_Allow_Temperature;
SemaphoreHandle_t xMutex_Access_UART_Buffer;
SemaphoreHandle_t xMutex_Access_Num_Msgs;
SemaphoreHandle_t xMutex_lcdQueue;

struct tm start_time;

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

        if(xSemaphoreTake(xMutex_lcdQueue, 0) == pdTRUE)
        {

            //Send the key value to the lcdQueue
            xQueueOverwrite(lcdQueue, &key);

            //Send the key value to the commandQueue
            xQueueOverwrite(commandQueue, &key);

            xSemaphoreGive(xMutex_lcdQueue);
        }

        //Send the key value to the skip_msg_Queue
        xQueueOverwrite(skip_msg_Queue, &key);
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

        snprintf(temperature + strlen(temperature),sizeof(temperature), "%s", temperature_array);
        snprintf(temperature + strlen(temperature),sizeof(temperature), "%s", units);

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
    uint8_t buffer_head = 0;

    num_msgs = 0;

    while(1)
    {
        //Wait to receive the notification from Keypad ISR
        if(ulTaskNotifyTake(pdTRUE, TIMEOUT_20) == pdPASS)
        {
            buffer_head = Receive_UART(buffer_head);
            vTaskDelay(1000);
        }
        else
        {
            Init_Uart();
        }

    }
}

//------------------------------------------------------------------- System_Init_Task: -------------------------------------------------------------------

void System_Init_Task(void *pvParameters)
{
    uint8_t i;
    //Initialize system peripherals
    Init_Peripherals();

    //Initialize UART_buffer
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        // Set each member of the struct to an empty string
        strcpy(UART_buffer[i].PQ_ID, "");
        strcpy(UART_buffer[i].SENSORS_PREQUELA, "");
        strcpy(UART_buffer[i].SENSORS_SEQUELA, "");
        strcpy(UART_buffer[i].LEN_RSSI_SNR, "");
        strcpy(UART_buffer[i].TIMESTAMP, "");
        strcpy(UART_buffer[i].RSSI_SNR, "");
    }

    //Create necessary start-up tasks (LCD, Keypad and Date_Time)
    //Create the Lcd task with low priority
    xTaskCreate(Lcd_Task, "Lcd_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xLcd_Task);

    //Create the Keypad task with low priority
    xTaskCreate(Keypad_Task, "Keypad_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xKeypad_Task);

    //Create the Keypad task with low priority
    xTaskCreate(Date_Time_Task, "Date_Time_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xDate_Time_Task);

    //Wait for Date_Time_task notification
    if(ulTaskNotifyTake(pdTRUE, TIMEOUT_20) == pdPASS)
    {
        //Store start time (set manually in TIME.h)
        start_time = getTime();
    }
    else
    {
        //If Date_Time_Task takes longer than notification timeout, a default date and time will be used

        //Suspend Date_Time_Task (it will not run again)
        vTaskSuspend(xDate_Time_Task);

        start_time = getTime();
    }


    //Create the Command task with low priority
    xTaskCreate(Command_Task, "Command_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xCommand_Task);

    //Create the Command task with low priority
    xTaskCreate(Tmp_Task, "Tmp_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xTmp_Task);

    //Create the Uart task with low priority
    xTaskCreate(Uart_Task, "Uart_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xUart_Task);

    //Set flag to warn LCD_task of task completion (use binary semaphore?)
    //Time_Date_Task_Done = 1;

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
    commandQueue = xQueueCreate(1, sizeof(char));

    //Create a queue for communication between keypad and command tasks
    skip_msg_Queue = xQueueCreate(1, sizeof(char));

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
        char num_msgs_str[] = "Num msgs: ";
        char num_msgs_str_aux[10] = "";

        if(xSemaphoreTake(xMutex_Access_Num_Msgs, portMAX_DELAY) == pdTRUE)
        {
            IntToString(num_msgs, num_msgs_str_aux, 0);

            xSemaphoreGive(xMutex_Access_Num_Msgs);
        }

        snprintf(num_msgs_str + strlen(num_msgs_str),sizeof(num_msgs_str), "%s", num_msgs_str_aux);

        //Send number of received packets to LCD
        if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
        {
            xQueueOverwrite(lcdQueue, num_msgs_str);

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
        /*
        char PQ_ID_aux[9];
        char SENSORS_PREQUELA_aux[19];
        char SENSORS_SEQUELA_aux[6];
        char LEN_RSSI_SNR_aux[15];
        char TIMESTAMP_aux[20];
        */

        //Convert the indexString to an integer
        int packet_index = atoi(packet_index_str);

        if(packet_index >= 1 && packet_index <= BUFFER_SIZE)
        {
            //Fetch data from UART buffer
            /*
            if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
            {
               strcpy(PQ_ID_aux, UART_buffer[0].PQ_ID);
               strcpy(SENSORS_PREQUELA_aux, UART_buffer[0].SENSORS_PREQUELA);
               strcpy(SENSORS_SEQUELA_aux, UART_buffer[0].SENSORS_SEQUELA);
               strcpy(LEN_RSSI_SNR_aux, UART_buffer[0].LEN_RSSI_SNR);
               strcpy(TIMESTAMP_aux, UART_buffer[0].TIMESTAMP);

               xSemaphoreGive(xMutex_Access_UART_Buffer);
            }
           */

            //Check if requested packet is empty
            if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
            {
                if(!strcmp(UART_buffer[packet_index - 1].PQ_ID, ""))
                {
                    if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
                    {
                        xQueueOverwrite(lcdQueue, "Packet Empty!");
                        xSemaphoreGive(xMutex_lcdQueue);
                    }
                    //We have to give semaphore if it enters the condition or not.
                    xSemaphoreGive(xMutex_Access_UART_Buffer);
                    return buzzer_toggle;
                }
                xSemaphoreGive(xMutex_Access_UART_Buffer);
            }


            //Send data to LCD (We shouldn't be able to write commands while its showing the packet content)
            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
            {
                //Pop key that entered command so that first message does not automatically skip.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                //Fetch packet message data according to selected number (protect UART_buffer)
                if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
                {
                    xQueueOverwrite(lcdQueue, UART_buffer[packet_index - 1].PQ_ID);
                    xSemaphoreGive(xMutex_Access_UART_Buffer);
                }
                //Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                //Fetch packet message data according to selected number (protect UART_buffer)
                if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
                {
                    xQueueOverwrite(lcdQueue, UART_buffer[packet_index - 1].SENSORS_PREQUELA);
                    xSemaphoreGive(xMutex_Access_UART_Buffer);
                }
                //Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                //Fetch packet message data according to selected number (protect UART_buffer)
                if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
                {
                    xQueueOverwrite(lcdQueue, UART_buffer[packet_index - 1].SENSORS_SEQUELA);
                    xSemaphoreGive(xMutex_Access_UART_Buffer);
                }
                //Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                //Fetch packet message data according to selected number (protect UART_buffer)
                if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
                {
                    xQueueOverwrite(lcdQueue, UART_buffer[packet_index - 1].LEN_RSSI_SNR);
                    xSemaphoreGive(xMutex_Access_UART_Buffer);
                }
                //Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                //Fetch packet message data according to selected number (protect UART_buffer)
                if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
                {
                    xQueueOverwrite(lcdQueue, UART_buffer[packet_index - 1].TIMESTAMP);
                    xSemaphoreGive(xMutex_Access_UART_Buffer);
                }
                //Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                //Fetch packet message data according to selected number (protect UART_buffer)
                if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
                {
                    xQueueOverwrite(lcdQueue, UART_buffer[packet_index - 1].RSSI_SNR);
                    xSemaphoreGive(xMutex_Access_UART_Buffer);
                }

                xSemaphoreGive(xMutex_lcdQueue);
            }
        }
        else
        {
            //Packet number is invalid, write message to LCD
            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
            {
                xQueueOverwrite(lcdQueue, "Invalid Packet Num!");

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

        //char RSSI_SNR_aux[20];

        //check if requested packet makes sense
        if (packet_index >= 1 && packet_index <= BUFFER_SIZE)
        {
            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
            {
                if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
                {
                    //Fetch packet message data according to selected number
                    xQueueOverwrite(lcdQueue, UART_buffer[packet_index - 1].RSSI_SNR);

                    xSemaphoreGive(xMutex_Access_UART_Buffer);
                }

                xSemaphoreGive(xMutex_lcdQueue);
            }
        }
        else
        {
            //Packet number is invalid, write message to LCD
            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
            {
                xQueueOverwrite(lcdQueue, "Invalid Packet Num!");

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
            while(!date_ok)
            {
                // Wait for a message with a timeout of 5 seconds
                if(xQueueReceive(commandQueue, &key_input , TIMEOUT_5) == pdPASS)
                {
                    //If enter is pressed (user has submitted command)
                    if(key_input == 'E')
                    {
                        //Call the Date_Process function and store date if valid.
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

                                //Delay so user can see error message
                                vTaskDelay(1000);
                            }

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
                        if((strlen(date_buffer)) < (CMD_MAX_SIZE - 1))
                        {
                            //Check if there is enough space in the cmd_buffer
                            //Add the key to the end of the command buffer
                            date_buffer[strlen(date_buffer)] = key_input;

                            if (strlen(date_buffer) == 2){
                                //Insert 3rd character ("/") automatically
                                date_buffer[strlen(date_buffer)] = '/';
                                //Send added charecter to LCD
                                if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
                                {
                                    xQueueOverwrite(lcdQueue, "/");

                                    xSemaphoreGive(xMutex_lcdQueue);
                                }
                            }
                            else if (strlen(date_buffer) == 5){
                                //Insert 6th character ("/") automatically
                                date_buffer[strlen(date_buffer)] = '/';
                                //Send added charecter to LCD
                                if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
                                {
                                    xQueueOverwrite(lcdQueue, "/");

                                    xSemaphoreGive(xMutex_lcdQueue);
                                }
                            }

                            //Null character - terminate the string
                            date_buffer[strlen(date_buffer)] = '\0';
                        }
                        else
                        {
                            //Handle buffer overflow (invalidate buffer)
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

            //Continue time loop

            //Write Initial message (LCD must be running)
            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
            {
                xQueueOverwrite(lcdQueue, "Time? hh:mm:ss");

                xSemaphoreGive(xMutex_lcdQueue);
            }

            // Wait for a message with a timeout of 5 seconds
            if(xQueueReceive(commandQueue, &key_input , TIMEOUT_5) == pdPASS)
            {
                //If enter is pressed (user has submitted command)
                if(key_input == 'E')
                {
                    //Call the Date_Process function and store time if valid
                    time_ok = Time_Process(time_buffer);

                    //Clear the command buffer
                    memset(time_buffer, 0, sizeof(time_buffer));

                    //if time ok, exit time loop
                    if(time_ok){
                        break;
                    }
                    //if date not ok, print error msg and stay in date loop.
                    else{
                        //Clear the date buffer
                        memset(time_buffer, 0, sizeof(time_buffer));
                        if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
                        {
                            xQueueOverwrite(lcdQueue, "Invalid Time!");

                            xSemaphoreGive(xMutex_lcdQueue);

                            //Delay so user can see error message
                            vTaskDelay(1000);
                        }

                        //Write Initial message again
                        if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
                        {
                            xQueueOverwrite(lcdQueue, "Time? hh:mm:ss");

                            xSemaphoreGive(xMutex_lcdQueue);
                        }
                    }
                }
                else if(key_input == 'C')
                {
                    //If cancel is pressed (user has cancelled input)
                    //Clear the command buffer
                    memset(time_buffer, 0, sizeof(time_buffer));

                    //Write Initial message again
                    if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
                    {
                        xQueueOverwrite(lcdQueue, "Time? hh:mm:ss");

                        xSemaphoreGive(xMutex_lcdQueue);
                    }
                }
                else
                {
                    if((strlen(time_buffer)) < (CMD_MAX_SIZE - 1))
                    {
                        //Check if there is enough space in the cmd_buffer
                        //Add the key to the end of the command buffer
                        time_buffer[strlen(time_buffer)] = key_input;

                        if (strlen(time_buffer) == 2)
                        {
                            //Insert 3rd character (":") automatically
                            time_buffer[strlen(time_buffer)] = ':';
                            //Send added charecter to LCD
                            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
                            {
                                xQueueOverwrite(lcdQueue, ":");

                                xSemaphoreGive(xMutex_lcdQueue);
                            }
                        }
                        else if (strlen(time_buffer) == 5)
                        {
                            //Insert 6th character (":") automatically
                            time_buffer[strlen(time_buffer)] = ':';
                            //Send added charecter to LCD
                            if(xSemaphoreTake(xMutex_lcdQueue, portMAX_DELAY) == pdTRUE)
                            {
                                xQueueOverwrite(lcdQueue, ":");

                                xSemaphoreGive(xMutex_lcdQueue);
                            }
                        }

                        //Null character - terminate the string
                        time_buffer[strlen(time_buffer)] = '\0';
                    }
                    else
                    {
                        //Handle buffer overflow (invalidate buffer)
                        time_buffer[0] = 'X';
                    }
                }

            }
            else
            {
                //Input timeout occurred, clear the command buffer
                memset(date_buffer, 0, sizeof(date_buffer));
            }


        }

        //Notify System_Init_Task to continue with start-up.
        xTaskNotifyGive(xDate_Time_Task);
        // Suspend the task (it will not run again)
        vTaskSuspend(NULL);

    }

}

//------------------------------------------------------------------- Date_Process: -------------------------------------------------------------------

bool Date_Process(const char *date_buffer)
{
    // Check if the date_buffer has a valid length for MM/DD/YYYY
    if (strlen(date_buffer) != 10) {
        return false;
    }

    // Parse the date components
    int month, day, year;
    if (sscanf(date_buffer, "%2d/%2d/%4d", &month, &day, &year) == 3)
    {
        // Check the validity of month, day, and year here
        // For simplicity, assuming a simple validation (e.g., no range checks)

        // Example: Check if month is in the valid range (1 to 12)
        if (month < 1 || month > 12) {
            return false;
        }

        // Example: Check if day is in the valid range (1 to 31)
        if (day < 1 || day > 31) {
            return false;
        }

        // Example: Check if year is in the valid range (e.g., 1900 to 2100)
        if (year < 1900 || year > 2100) {
            return false;
        }

        // Populate the start_tm structure
        start_time.tm_year = year - 1900;  // years since 1900
        start_time.tm_mon = month - 1;     // months since January (0-based)
        start_time.tm_mday = day;

        return true;
    }

    // If sscanf fails, the date format is invalid
    return false;

}

//------------------------------------------------------------------- Date_Process: -------------------------------------------------------------------

bool Time_Process(const char *time_buffer)
{
    // Check if the time_buffer has a valid length for hh:mm:ss
    if (strlen(time_buffer) != 8) {
        return false;
    }

    // Parse the time components
    int hour, minute, second;
    if (sscanf(time_buffer, "%2d:%2d:%2d", &hour, &minute, &second) == 3) {
        // Check the validity of hour, minute, and second here
        // For simplicity, assuming a simple validation (e.g., no range checks)

        // Example: Check if hour is in the valid range (0 to 23)
        if (hour < 0 || hour > 23) {
            return false;
        }

        // Example: Check if minute is in the valid range (0 to 59)
        if (minute < 0 || minute > 59) {
            return false;
        }

        // Example: Check if second is in the valid range (0 to 59)
        if (second < 0 || second > 59) {
            return false;
        }

        // Populate the result_tm structure
        start_time.tm_hour = hour;
        start_time.tm_min = minute;
        start_time.tm_sec = second;

        return true;
    }

    // If sscanf fails, the time format is invalid
    return false;

}

