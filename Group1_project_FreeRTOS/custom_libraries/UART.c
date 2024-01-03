//Includes
#include "UART.h"

struct packet UART_buffer[BUFFER_SIZE];

//--------------------------------------------------------------------- Receive_UART: --------------------------------------------------------------------------

void Receive_UART(void)
{
    uint8_t count = 0;
    char receivedChar;
    char msg[MSG_SIZE];

    uint64_t timestamp;
    time_t current_time_t;
    time_t raw_time_t;
    time_t start_time_t;

    struct tm start_time;
    struct tm *current_time_info;

    char current_time_str[20];

    while (count < MSG_SIZE - 1)
    {
        //Waits until one character be ready to be read
        while (!UARTCharsAvail(UART3_BASE));

        //Read a character from UART3
        receivedChar = UARTCharGet(UART3_BASE);

        //Checks if it is a terminal character
        if (receivedChar == '\n' || receivedChar == '\r')
        {
            //Add a null character at the end of string
            msg[count++] = '\0';
            break;
        }
        else
        {
            msg[count++] = receivedChar;
        }
    }

    sscanf(msg, "%19[^,],%19[^;];%19[^\0]\0", UART_buffer[0].PQ_ID, UART_buffer[0].SENSORS, UART_buffer[0].LEN_RSSI_SNR);

    xQueueOverwrite(lcdQueue, UART_buffer[0].PQ_ID);
    vTaskDelay(2000);
    xQueueOverwrite(lcdQueue, UART_buffer[0].SENSORS);
    vTaskDelay(2000);
    xQueueOverwrite(lcdQueue, UART_buffer[0].LEN_RSSI_SNR);
    vTaskDelay(2000);

    //Store start time (set manually in TIME.h)
    start_time = getTime();

    //Get the current timestamp from the system timer
    timestamp = TimerValueGet64(WTIMER5_BASE);

    //Calculate elapsed seconds using clock frequency
    raw_time_t =  (time_t)((timestamp / SysCtlClockGet()));

    //Convert start_time to time_t format for calculations (returns the value in seconds!)
    start_time_t = mktime(&start_time);

    //Calculate the current time as time_t
    current_time_t = start_time_t + raw_time_t;

    //Convert time in time_t to standard C time format
    current_time_info = localtime(&current_time_t);

    //Convert from standard C time format to string
    strftime(current_time_str, sizeof(current_time_str), "%Y-%m-%d %H:%M:%S", current_time_info);

    // Append time string to msg string using sprintf
    //sprintf(msg + strlen(msg), "%s", current_time_str);

    // Add a null character at the end of string
    //msg[sizeof(msg) - 1] = '\0';

    //Store the received message in UART_buffer
    //strncpy(UART_buffer[buffer_head], msg, MSG_SIZE);

    //num_msgs++;

    //Increments buffer head, wraps around when buffer is full (circular buffer)
    //buffer_head = (buffer_head + 1) % BUFFER_SIZE;
}

//--------------------------------------------------------------------- UART3IntHandler: --------------------------------------------------------------------------

void UART3IntHandler(void)
{
    uint32_t ui32Status;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

   //Get the interrupt status (masked interrupt should be RX type)
   ui32Status = UARTIntStatus(UART3_BASE, true);

   //Clear interrupt flag
   UARTIntClear(UART3_BASE, ui32Status);

   //Check if the receive interrupt is triggered (if status matches UART interrupt mask)
   if (ui32Status & UART_INT_RX)
   {
       vTaskNotifyGiveFromISR(xUart_Task, &xHigherPriorityTaskWoken);
   }

   //To prevent the program to get stuck in the ISR
   portYIELD_FROM_ISR(true);
}
