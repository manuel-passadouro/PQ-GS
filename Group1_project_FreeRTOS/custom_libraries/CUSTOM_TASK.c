// Includes.
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
SemaphoreHandle_t xMutex_User_Input;

struct tm start_time;

//------------------------------------------------------------------- Lcd_Task: -------------------------------------------------------------------

void Lcd_Task(void *pvParameters)
{
    char MsgToWrite[MSG_SIZE];
    int flag_next_key_clear = 0;

    // Create a queue for communication between tasks.
    lcdQueue = xQueueCreate(1, sizeof(char[MSG_SIZE]));

    while (1)
    {
        // Wait for a message with a timeout of 5 seconds.
        if(xQueueReceive(lcdQueue, MsgToWrite, TIMEOUT_5) == pdPASS)
        {
            // Message received within the timeout.
           if((strcmp(MsgToWrite, "C") == 0) || (strcmp(MsgToWrite, "E") == 0)) // These keys always clear LCD.
           {
               Lcd_Clear();
           }
           else if(strlen(MsgToWrite) > 1) // If message is cmd output or temperature output string, the next letter from keypad should clear the lcd.
           {
               flag_next_key_clear = 1;

               Lcd_Clear();

               Lcd_Write_String(MsgToWrite);
           }
           else
           {
               if(flag_next_key_clear) // If previous message was cmd or temperature string, key clears LCD.
               {
                   Lcd_Clear();

                   flag_next_key_clear = 0;
               }

               Lcd_Write_String(MsgToWrite);
           }
        }
        else
        {
            // Timeout occurred, handle it (clear LCD or do something else).
            Lcd_Clear();

            // Notify the Tmp_Task in order to let it know that can send to lcdQueue again.
            xTaskNotifyGive(xTmp_Task);
        }
    }
}

//------------------------------------------------------------------- Keypad_Task: -------------------------------------------------------------------

void Keypad_Task(void *pvParameters)
{
    while(1)
    {
        // Wait to receive the notification from Keypad ISR.
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xSemaphoreGive(xSemaphore_Allow_Temperature);

        // 0 = Non blocking check.
        if(xSemaphoreTake(xMutex_User_Input, 0) == pdTRUE)
        {
            // Send the key value to the lcdQueue.
            xQueueOverwrite(lcdQueue, &key);

            // Send the key value to the commandQueue.
            xQueueOverwrite(commandQueue, &key);

            xSemaphoreGive(xMutex_User_Input);
        }

        // Send the key value to the skip_msg_Queue.
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
        char temperature[20] = "Temperature: ";
        char units[3] = " C";

        temperature_raw = TMP100_Read();

        vTaskDelay(2000);

        // Convert the float value of the reading to an array to show in LCD later.
        FloatToArray(temperature_raw, temperature_array, 1);

        snprintf(temperature + strlen(temperature),sizeof(temperature), "%s", temperature_array);
        snprintf(temperature + strlen(temperature),sizeof(temperature), "%s", units);

        // 0 = Non blocking check.
        if(xSemaphoreTake(xSemaphore_Allow_Temperature, 0) == pdTRUE)
        {
            // Checks if it get a notification from LCD timeout.
            while(!ulTaskNotifyTake(pdTRUE, portMAX_DELAY))
            {
                // Get temperature even if it cannot send to LCD yet.
                temperature_raw = TMP100_Read();
            }
        }

        // Send temperature info to lcdQueue.
        xQueueOverwrite(lcdQueue, &temperature);
    }
}

//---------------------------------------------------------------------- Uart_Task: -------------------------------------------------------------------

void Uart_Task(void *pvParameters)
{
    uint8_t buffer_head = 0;

    num_msgs = 0;

    while(1)
    {
        // Wait to receive the notification from Keypad ISR.
        if(ulTaskNotifyTake(pdTRUE, TIMEOUT_20) == pdPASS)
        {
            buffer_head = Receive_UART(buffer_head);
        }
        else
        {
            // Reset UART module if too much time since last interrupt (when PQ resets for example).
            // Send data to LCD (We shouldn't be able to write commands while it's showing the packet content).
            if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
            {
                // Send data to LCD (We shouldn't be able to write commands while it's showing the packet content).

                    // Waits until the condition is verified.
                    xQueueOverwrite(lcdQueue, "UART Reset!");

                    // Display for 2 seconds or skip if any key is pressed.
                    //xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                    // Any key pressed will be handled as "C" (clear command).
                    //xQueueOverwrite(lcdQueue, "C");

                xSemaphoreGive(xMutex_User_Input);
            }

            Init_Uart();
        }
    }
}

//------------------------------------------------------------------- System_Init_Task: -------------------------------------------------------------------

void System_Init_Task(void *pvParameters)
{
    uint8_t i;

    // Initialize essential system peripherals.
    Init_Peripherals_Phase_1();

    // Initialize UART_buffer.
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        // Set each member of the structure to an empty string.
        strcpy(UART_buffer[i].PQ_ID, "");
        strcpy(UART_buffer[i].SENSORS_PREQUELA, "");
        strcpy(UART_buffer[i].SENSORS_SEQUELA, "");
        strcpy(UART_buffer[i].LEN_RSSI_SNR, "");
        strcpy(UART_buffer[i].TIMESTAMP, "");
        strcpy(UART_buffer[i].RSSI_SNR, "");
    }

    // Create a binary semaphore for protecting shared resources (lcdQueue, to allow temperature).
    xSemaphore_Allow_Temperature = xSemaphoreCreateBinary();

    // Create a mutex semaphore for protecting shared resources (lcdQueue, to access UART buffer).
    xMutex_Access_UART_Buffer = xSemaphoreCreateMutex();

    // Create a mutex semaphore for protecting shared resources (lcdQueue, to Access the number of received packets).
    xMutex_Access_Num_Msgs = xSemaphoreCreateMutex();

    // Create a mutex semaphore for protecting shared resources (lcdQueue by TMP100, Keypad and LCD).
    xMutex_User_Input = xSemaphoreCreateMutex();

    // Create necessary start-up tasks (LCD, Keypad and Date_Time).

    // Create the Lcd_Task with low priority (tskIDLE_PRIORITY + 2).
    xTaskCreate(Lcd_Task, "Lcd_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, &xLcd_Task);

    // Create the Keypad_Task with low priority (tskIDLE_PRIORITY + 2).
    xTaskCreate(Keypad_Task, "Keypad_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, &xKeypad_Task);

    // Create the Date_Time_Task with low priority (tskIDLE_PRIORITY + 1).
    xTaskCreate(Date_Time_Task, "Date_Time_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xDate_Time_Task);

    // Wait for Date_Time_task notification.
    if(ulTaskNotifyTake(pdTRUE, portMAX_DELAY) == pdPASS)
    {
        // Suspend Date_Time_Task (it will not run again).
        vTaskSuspend(xDate_Time_Task);
    }
    else
    {
        // Suspend Date_Time_Task (it will not run again).
        vTaskSuspend(xDate_Time_Task);

        start_time = getTime();
    }

    // Initialize remaining peripherals.
    Init_Peripherals_Phase_2();

    // Create the Command_Task with low priority (tskIDLE_PRIORITY + 2).
    xTaskCreate(Command_Task, "Command_Task", configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 2, &xCommand_Task);

    // Create the Tmp_Task with low priority (tskIDLE_PRIORITY + 1).
    xTaskCreate(Tmp_Task, "Tmp_Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xTmp_Task);

    // Create the Uart_Task with low priority (tskIDLE_PRIORITY + 2).
    xTaskCreate(Uart_Task, "Uart_Task", configMINIMAL_STACK_SIZE + 200, NULL, tskIDLE_PRIORITY + 4, &xUart_Task);

    // Send data to LCD (We shouldn't be able to write commands while it's showing the packet content).
    if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
    {
        // Pop key that entered command so that first message does not automatically skip.
        xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

        // Waits until the condition is verified.
        xQueueOverwrite(lcdQueue, "System Init Done!");

        // Display for 2 seconds or skip if any key is pressed.
        xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

        // Any key pressed will be handled as "C" (clear command).
        xQueueOverwrite(lcdQueue, "C");

        xSemaphoreGive(xMutex_User_Input);
    }

    // Suspend the System_Init_Task (it will not run again).
    vTaskSuspend(NULL);
}

//------------------------------------------------------------------- Command_Task: -------------------------------------------------------------------

void Command_Task(void *pvParameters)
{
    char key_input;
    char cmd_buffer[CMD_MAX_SIZE] = "";
    bool buzzer_toggle = 0;

    while (1)
    {
        // Wait for a message with a timeout of 5 seconds.
        if(xQueueReceive(commandQueue, &key_input , TIMEOUT_5) == pdPASS)
        {
            // If enter is pressed (user has submitted command).
            if(key_input == 'E')
            {
                // Call the Command_Process function store the state of the buzzer.
                buzzer_toggle = Command_Process(cmd_buffer, buzzer_toggle);

                // Clear the cmd_buffer.
                memset(cmd_buffer, 0, sizeof(cmd_buffer));
            }
            else if(key_input == 'C')
            {
                // If cancel is pressed (user has cancelled input).
                // Clear the cmd_buffer.
                memset(cmd_buffer, 0, sizeof(cmd_buffer));
            }
            else
            {
                if((strlen(cmd_buffer)) < (CMD_MAX_SIZE - 1))
                {
                    // Check if there is enough space in the cmd_buffer.
                    // Add the key to the end of the cmd_buffer.
                    cmd_buffer[strlen(cmd_buffer)] = key_input;

                    // Null character to terminate the string.
                    cmd_buffer[strlen(cmd_buffer) + 1] = '\0';
                }
                else
                {
                    // Handle buffer overflow (invalidate buffer in case of user write more than 3 characters).
                    cmd_buffer[0] = 'X';
                }
            }
        }
        else
        {
            // Input timeout occurred, clear the cmd_buffer.
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

        // Buzzer ON/OFF.
        if(buzzer_toggle == 1)
        {
            // Send data to LCD (We shouldn't be able to write commands while its showing the message).
            if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
            {
                // Pop key that entered command so that first message does not automatically skip.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2);

                // Waits until the condition is verified.
                xQueueOverwrite(lcdQueue, "Buzzer ON!");

                // Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                // Any key pressed will be handled as "C" (clear command).
                xQueueOverwrite(lcdQueue, "C");

                xSemaphoreGive(xMutex_User_Input);
            }
        }
        else
        {
            // Send data to LCD (We shouldn't be able to write commands while its showing the message).
            if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
            {
                // Pop key that entered command so that first message does not automatically skip.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2);

                // Waits until the condition is verified.
                xQueueOverwrite(lcdQueue, "Buzzer OFF!");

                // Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                // Any key pressed will be handled as "C" (clear command).
                xQueueOverwrite(lcdQueue, "C");

                xSemaphoreGive(xMutex_User_Input);
            }
        }
    }
    else if(strcmp(cmd_buffer, "A") == 0)
    {
        // Show number of PQube packets received.
        char num_msgs_str[] = "Num msgs: ";
        char num_msgs_str_aux[10] = "";

        if(xSemaphoreTake(xMutex_Access_Num_Msgs, portMAX_DELAY) == pdTRUE)
        {
            // Convert the number of received messages into a string for later display in LCD.
            IntToString(num_msgs, num_msgs_str_aux, 0);

            xSemaphoreGive(xMutex_Access_Num_Msgs);
        }

        // Rearrange the string to show the number of packets received.
        snprintf(num_msgs_str + strlen(num_msgs_str),sizeof(num_msgs_str), "%s", num_msgs_str_aux);

        // Send data to LCD (We shouldn't be able to write commands while its showing the message).
        if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
        {
            // Pop key that entered command so that first message does not automatically skip.
            xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2);

            // Waits until the condition is verified.
            xQueueOverwrite(lcdQueue, num_msgs_str);

            //Display for 2 seconds or skip if any key is pressed.
            xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

            // Any key pressed will be handled as "C" (clear command).
            xQueueOverwrite(lcdQueue, "C");

            xSemaphoreGive(xMutex_User_Input);
        }
    }
    else if(cmd_buffer[0] == 'B' && (strlen(cmd_buffer) == 3))
    {
        // Show PQcube packet BXX (XX is the position of the wanted packet in buffer (01 -20).

        // Extract the packet number part from the cmd_buffer.
        const char *packet_index_str = &cmd_buffer[1];

        // Convert the indexString to an integer.
        int packet_index = atoi(packet_index_str);

        if(packet_index >= 1 && packet_index <= BUFFER_SIZE)
        {
            // Check if requested packet is empty.
            if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
            {
                if(!strcmp(UART_buffer[packet_index - 1].PQ_ID, ""))
                {
                    // Send data to LCD (We shouldn't be able to write commands while its showing the message).
                    if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
                    {
                        // Pop key that entered command so that first message does not automatically skip.
                        xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2);

                        // Waits until the condition is verified.
                        xQueueOverwrite(lcdQueue, "Packet doesn't exist");

                        // Display for 2 seconds or skip if any key is pressed.
                        xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                        // Any key pressed will be handled as "C" (clear command).
                        xQueueOverwrite(lcdQueue, "C");

                        xSemaphoreGive(xMutex_User_Input);
                    }

                    // We have to give semaphore if it enters the condition or not.
                    xSemaphoreGive(xMutex_Access_UART_Buffer);

                    return buzzer_toggle;
                }

                xSemaphoreGive(xMutex_Access_UART_Buffer);
            }

            // Send data to LCD (We shouldn't be able to write commands while its showing the packet content).
            if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
            {
                // Pop key that entered command so that first message does not automatically skip.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                // Fetch packet message data according to selected number (protect UART_buffer).
                if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
                {
                    xQueueOverwrite(lcdQueue, UART_buffer[packet_index - 1].PQ_ID);

                    xSemaphoreGive(xMutex_Access_UART_Buffer);
                }

                // Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                // Fetch packet message data according to selected number (protect UART_buffer).
                if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
                {
                    xQueueOverwrite(lcdQueue, UART_buffer[packet_index - 1].SENSORS_PREQUELA);

                    xSemaphoreGive(xMutex_Access_UART_Buffer);
                }

                // Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                // Fetch packet message data according to selected number (protect UART_buffer).
                if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
                {
                    xQueueOverwrite(lcdQueue, UART_buffer[packet_index - 1].SENSORS_SEQUELA);

                    xSemaphoreGive(xMutex_Access_UART_Buffer);
                }

                // Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                // Fetch packet message data according to selected number (protect UART_buffer).
                if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
                {
                    xQueueOverwrite(lcdQueue, UART_buffer[packet_index - 1].LEN_RSSI_SNR);

                    xSemaphoreGive(xMutex_Access_UART_Buffer);
                }

                // Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                // Fetch packet message data according to selected number (protect UART_buffer).
                if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
                {
                    xQueueOverwrite(lcdQueue, UART_buffer[packet_index - 1].TIMESTAMP);

                    xSemaphoreGive(xMutex_Access_UART_Buffer);
                }
                // Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                // Any key pressed will be handled as "C" (clear command).
                xQueueOverwrite(lcdQueue, "C");

                xSemaphoreGive(xMutex_User_Input);
            }
        }
        else
        {
            // Send data to LCD (We shouldn't be able to write commands while its showing the message).
            if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
            {
                // Pop key that entered command so that first message does not automatically skip.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2);

                // Waits until the condition is verified.
                xQueueOverwrite(lcdQueue, "Invalid packet num!");

                // Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                // Any key pressed will be handled as "C" (clear command).
                xQueueOverwrite(lcdQueue, "C");

                xSemaphoreGive(xMutex_User_Input);
            }
        }
    }
    else if(cmd_buffer[0] == 'F' && (strlen(cmd_buffer) == 3))
    {
        // Show PQcube packet RSSI & SNR FXX.
        // Extract the packet number part from the command string.
        const char *packet_index_str = &cmd_buffer[1];

        // Convert the indexString to an integer.
        int packet_index = atoi(packet_index_str);

        // Check if requested packet number makes sense.
        if (packet_index >= 1 && packet_index <= BUFFER_SIZE)
        {
            // Check if requested packet is empty.
            if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
            {
                if(!strcmp(UART_buffer[packet_index - 1].RSSI_SNR, ""))
                {
                    // Send data to LCD (We shouldn't be able to write commands while its showing the message).
                    if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
                    {
                        // Pop key that entered command so that first message does not automatically skip.
                        xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2);

                        // Waits until the condition is verified.
                        xQueueOverwrite(lcdQueue, "Packet doesn't exist");

                        // Display for 2 seconds or skip if any key is pressed.
                        xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                        // Any key pressed will be handled as "C" (clear command).
                        xQueueOverwrite(lcdQueue, "C");

                        xSemaphoreGive(xMutex_User_Input);
                    }

                    // We have to give semaphore if it enters the condition or not.
                    xSemaphoreGive(xMutex_Access_UART_Buffer);

                    return buzzer_toggle;
                }

                xSemaphoreGive(xMutex_Access_UART_Buffer);
            }

            // Send data to LCD (We shouldn't be able to write commands while its showing the packet content).
            if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
            {
                // Pop key that entered command so that first message does not automatically skip.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2);

                // Fetch packet message data according to selected number (protect UART_buffer).
                if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
                {
                    xQueueOverwrite(lcdQueue, UART_buffer[packet_index - 1].RSSI_SNR);

                    xSemaphoreGive(xMutex_Access_UART_Buffer);
                }

                // Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2);

                // Any key pressed will be handled as "C" (clear command).
                xQueueOverwrite(lcdQueue, "C");

                xSemaphoreGive(xMutex_User_Input);
            }
        }
        else
        {
            // Send data to LCD (We shouldn't be able to write commands while its showing the message).
            if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
            {
                // Pop key that entered command so that first message does not automatically skip.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2);

                // Waits until the condition is verified.
                xQueueOverwrite(lcdQueue, "Invalid packet num!");

                // Display for 2 seconds or skip if any key is pressed.
                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                // Any key pressed will be handled as "C" (clear command).
                xQueueOverwrite(lcdQueue, "C");

                xSemaphoreGive(xMutex_User_Input);
            }
        }
    }
    else
    {
        // Send data to LCD (We shouldn't be able to write commands while its showing the message).
        if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
        {
            // Pop key that entered command so that first message does not automatically skip.
            xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2);

            // Waits until the condition is verified.
            xQueueOverwrite(lcdQueue, "Invalid Command!");

            // Display for 2 seconds or skip if any key is pressed.
            xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

            // Any key pressed will be handled as "C" (clear command).
            xQueueOverwrite(lcdQueue, "C");

            xSemaphoreGive(xMutex_User_Input);
        }
    }

    return buzzer_toggle;
}

//------------------------------------------------------------------- Date_Time_Task: -------------------------------------------------------------------

void Date_Time_Task(void *pvParameters)
{
    char date_buffer[DATE_MAX_SIZE] = "";
    char time_buffer[TIME_MAX_SIZE] = "";
    char key_input;
    bool date_ok = false;
    bool time_ok = false;
    bool flag_time_repeat = false;

    // Create a queue for communication between keypad and command tasks.
    commandQueue = xQueueCreate(1, sizeof(char)); // This queue will be used by command_task after start-up.

    // Create a queue for communication between keypad and command tasks.
    skip_msg_Queue = xQueueCreate(1, sizeof(char));

    while (1)
    {
        // Send data to LCD (We shouldn't be able to write commands while its showing the message).
        if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
        {
            // Waits until the condition is verified.
            xQueueOverwrite(lcdQueue, "--Group 1 Project!--");

            // Display for 2 seconds or skip if any key is pressed.
            xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

            // Any key pressed will be handled as "C" (clear command).
            xQueueOverwrite(lcdQueue, "C");

            xSemaphoreGive(xMutex_User_Input);
        }

        // Write Initial message (LCD must be running).
        xQueueOverwrite(lcdQueue, "Date? MM/DD/YYYY");

        // Start time loop.
        while(1)
        {
            // Start date loop.
            while(!date_ok)
            {
                // Wait for a message with a timeout of 5 seconds.
                if(xQueueReceive(commandQueue, &key_input , TIMEOUT_5) == pdPASS)
                {
                    // If enter is pressed (user has submitted command).
                    if(key_input == 'E')
                    {
                        // Call the Date_Process function and store date if valid.
                        date_ok = Date_Process(date_buffer);

                        // Clear the date_buffer.
                        memset(date_buffer, 0, sizeof(date_buffer));

                        // If date_ok == 1, the user entered a valid date and the program can exit date loop.
                        if(date_ok == 1)
                        {
                            break;
                        }
                        else // If date not ok, print error message and stay in date loop.
                        {
                            // Clear the date_buffer.
                            memset(date_buffer, 0, sizeof(date_buffer));

                            // Send data to LCD (We shouldn't be able to write commands while its showing the packet content).
                            if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
                            {
                                // Pop key that entered command so that first message does not automatically skip.
                                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                                // Waits until the condition is verified.
                                xQueueOverwrite(lcdQueue, "Invalid Date!");

                                // Display for 2 seconds or skip if any key is pressed.
                                xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                                // Any key pressed will be handled as "C" (clear command).
                                xQueueOverwrite(lcdQueue, "C");

                                xSemaphoreGive(xMutex_User_Input);
                            }

                            // Write Initial message again.
                            xQueueOverwrite(lcdQueue, "Date? MM/DD/YYYY");
                        }
                    }
                    else if(key_input == 'C')  // If cancel is pressed (user has cancelled input).
                    {
                        // Clear the date_buffer.
                        memset(date_buffer, 0, sizeof(date_buffer));

                        // Write Initial message again.
                        xQueueOverwrite(lcdQueue, "Date? MM/DD/YYYY");
                    }
                    else
                    {
                        if((strlen(date_buffer)) < (DATE_MAX_SIZE - 1))  // Check if there is enough space in the date_buffer.
                        {
                            // Add the key to the end of the date_buffer.
                            date_buffer[strlen(date_buffer)] = key_input;

                            if (strlen(date_buffer) == 2)
                            {
                                // Insert 3rd character ("/") automatically.
                                date_buffer[strlen(date_buffer)] = '/';

                                // Send added character to LCD.
                                xQueueOverwrite(lcdQueue, "/");
                            }
                            else if (strlen(date_buffer) == 5)
                            {
                                // Insert 6th character ("/") automatically.
                                date_buffer[strlen(date_buffer)] = '/';

                                // Send added character to LCD.
                                xQueueOverwrite(lcdQueue, "/");
                            }

                            // Null character to terminate the string.
                            date_buffer[strlen(date_buffer)] = '\0';
                        }
                        else
                        {
                            // Handle buffer overflow (invalidate buffer).
                            date_buffer[0] = 'X';
                        }
                    }
                }
                else
                {
                    // Input timeout occurred, clear the date_buffer.
                    memset(date_buffer, 0, sizeof(date_buffer));

                    // Write Initial message again.
                    xQueueOverwrite(lcdQueue, "Date? MM/DD/YYYY");
                }
            }

            // Continue Time loop.

            // Write Initial message (LCD must be running).
            if(!flag_time_repeat)
            {
                xQueueOverwrite(lcdQueue, "Time? hh:mm:ss");

                flag_time_repeat = 1;
            }

            // Wait for a message with a timeout of 5 seconds.
            if(xQueueReceive(commandQueue, &key_input , TIMEOUT_5) == pdPASS)
            {
                // If enter is pressed (user has submitted command).
                if(key_input == 'E')
                {
                    // Call the Time_Process function and store time if valid.
                    time_ok = Time_Process(time_buffer);

                    // Clear the time_buffer.
                    memset(time_buffer, 0, sizeof(time_buffer));

                    // If time_ok == 1, the user entered a valid time and the program can exit date loop.
                    if(time_ok == 1)
                    {
                        break;
                    }
                    else    // If time not ok, print error message and stay in date loop.
                    {
                        // Clear the time_buffer.
                        memset(time_buffer, 0, sizeof(time_buffer));

                        // Send data to LCD (We shouldn't be able to write commands while its showing the packet content).
                        if(xSemaphoreTake(xMutex_User_Input, portMAX_DELAY) == pdTRUE)
                        {
                            // Pop key that entered command so that first message does not automatically skip.
                            xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                            // Waits until the condition is verified.
                            xQueueOverwrite(lcdQueue, "Invalid Time!");

                            // Display for 2 seconds or skip if any key is pressed.
                            xQueueReceive(skip_msg_Queue, NULL , TIMEOUT_2) == pdPASS;

                            // Any key pressed will be handled as "C" (clear command).
                            xQueueOverwrite(lcdQueue, "C");

                            xSemaphoreGive(xMutex_User_Input);
                        }

                        // Write Initial message again.
                        xQueueOverwrite(lcdQueue, "Time? hh:mm:ss");
                    }
                }
                else if(key_input == 'C')   // If cancel is pressed (user has cancelled input).
                {
                    // Clear the time_buffer.
                    memset(time_buffer, 0, sizeof(time_buffer));

                    // Write Initial message again.
                    xQueueOverwrite(lcdQueue, "Time? hh:mm:ss");
                }
                else
                {
                    if((strlen(time_buffer)) < (TIME_MAX_SIZE - 1))  // Check if there is enough space in the time_buffer.
                    {
                        // Add the key to the end of the time_buffer.
                        time_buffer[strlen(time_buffer)] = key_input;

                        if (strlen(time_buffer) == 2)
                        {
                            // Insert 3rd character (":") automatically.
                            time_buffer[strlen(time_buffer)] = ':';

                            // Send added character to LCD.
                            xQueueOverwrite(lcdQueue, ":");
                        }
                        else if (strlen(time_buffer) == 5)
                        {
                            // Insert 6th character (":") automatically.
                            time_buffer[strlen(time_buffer)] = ':';

                            // Send added character to LCD.
                            xQueueOverwrite(lcdQueue, ":");
                        }

                        // Null character to terminate the string.
                        time_buffer[strlen(time_buffer)] = '\0';
                    }
                    else
                    {
                        // Handle buffer overflow (invalidate buffer).
                        time_buffer[0] = 'X';
                    }
                }
            }
            else
            {
                // Input timeout occurred, clear the time_buffer.
                memset(time_buffer, 0, sizeof(time_buffer));

                // Write Initial message again.
                xQueueOverwrite(lcdQueue, "Time? hh:mm:ss");
            }
        }

        // Notify Date_Time_Task to continue with start-up.
        xTaskNotifyGive(xSystem_Init_Task);
    }
}

//------------------------------------------------------------------- Date_Process: -------------------------------------------------------------------

bool Date_Process(const char *date_buffer)
{
    int month, day, year;

    // Check if the date_buffer has a valid length for MM/DD/YYYY.
    if (strlen(date_buffer) != 10)
    {
        return false;
    }

    // Parse the date components.
    if(sscanf(date_buffer, "%2d/%2d/%4d", &month, &day, &year) == 3)
    {
        // Check the validity of month, day, and year here.
        // For simplicity, assuming a simple validation (example: no range checks).

        // Example: Check if month is in the valid range (1 to 12).
        if (month < 1 || month > 12)
        {
            return false;
        }

        // Example: Check if day is in the valid range (1 to 31).
        if (day < 1 || day > 31)
        {
            return false;
        }

        // Example: Check if year is in the valid range (example: 1900 to 2100).
        if (year < 1900 || year > 2100)
        {
            return false;
        }

        // Populate the start_tm structure.
        start_time.tm_year = year - 1900;  // Years since 1900.
        start_time.tm_mon = month - 1;     // Months since January (0-based).
        start_time.tm_mday = day;

        return true;
    }

    // If sscanf fails, the date format is invalid.
    return false;

}

//------------------------------------------------------------------- Time_Process: -------------------------------------------------------------------

bool Time_Process(const char *time_buffer)
{
    int hour, minute, second;

    // Check if the time_buffer has a valid length for hh:mm:ss.
    if (strlen(time_buffer) != 8)
    {
        return false;
    }

    // Parse the time components.
    if(sscanf(time_buffer, "%2d:%2d:%2d", &hour, &minute, &second) == 3)
    {
        // Check the validity of hour, minute, and second here.
        // For simplicity, assuming a simple validation (example: no range checks).

        // Example: Check if hour is in the valid range (0 to 23).
        if (hour < 0 || hour > 23)
        {
            return false;
        }

        // Example: Check if minute is in the valid range (0 to 59).
        if (minute < 0 || minute > 59)
        {
            return false;
        }

        // Example: Check if second is in the valid range (0 to 59).
        if (second < 0 || second > 59)
        {
            return false;
        }

        // Populate the result_tm structure.
        start_time.tm_hour = hour;
        start_time.tm_min = minute;
        start_time.tm_sec = second;

        return true;
    }

    // If sscanf fails, the time format is invalid.
    return false;
}
