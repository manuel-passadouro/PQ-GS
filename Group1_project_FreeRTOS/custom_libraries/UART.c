//Includes
#include "UART.h"

struct packet UART_buffer[BUFFER_SIZE];
int num_msgs;

//--------------------------------------------------------------------- Receive_UART: --------------------------------------------------------------------------

uint8_t Receive_UART(uint8_t buffer_head)
{
    uint8_t count = 0;
    int position = 19;

    char receivedChar;
    char msg[MSG_SIZE];
    char sensors_total[25];
    char length[5];
    char rssi[5];
    char snr[5];
    char rssi_text[20] = "RSSI: ";
    char snr_text[20] = " SNR: ";
    char current_time_str[20];

    bool charP = 0;
    uint8_t i = 0;

    uint64_t timestamp;
    time_t current_time_t;
    time_t raw_time_t;
    time_t start_time_t;

    struct tm *current_time_info;

    //Clear msg buffer
    memset(msg, 0, sizeof(msg));

    while (count < MSG_SIZE - 1)
    {
        //Waits until one character be ready to be read
        while (!UARTCharsAvail(UART3_BASE));

        //Read a character from UART3
        receivedChar = UARTCharGet(UART3_BASE);

        //Checks if it is a 'P' character
        if (receivedChar == 'P')
        {
            charP = 1;
        }

        //Checks if it is a terminal character
        if (receivedChar == '\n' || receivedChar == '\r')
        {
            // Add a null character at the end of string
            msg[sizeof(msg) - 1] = '\0';

            vTaskDelay(10);

            break;
        }

        //Stores the character in the buffer
        if(charP == 1)
        {
            msg[i++] = receivedChar;
        }
    }

    /*
    //Make sure uart buffer is empty
    while (UARTCharsAvail(UART3_BASE))
    {
        char discard = UARTCharGet(UART3_BASE);
    }
    */

    //Empty UART FIFO to prevent msg overwrite.

    // Disable UART3 to make changes
    UART3_CTL_R &= ~UART_CTL_UARTEN;
    // Flush the FIFO by clearing the RX and TX FIFOs
    UART3_CTL_R |= UART_CTL_RXE | UART_CTL_TXE;
    // Enable UART3
    UART3_CTL_R |= UART_CTL_UARTEN;

    //Check for empty FIFO
    if(UART3_FR_R & UART_FR_RXFE != 0);


    // Add a null character at the end of string
    msg[sizeof(msg) - 1] = '\0';

    //Parse and store received message to UART_buffer (protected with semaphore)
    //Check if empty?
    if(xSemaphoreTake(xMutex_Access_UART_Buffer, portMAX_DELAY) == pdTRUE)
    {
        sscanf(msg, "%8[^,],%24[^;]%14[^\0]", UART_buffer[buffer_head].PQ_ID, sensors_total, UART_buffer[buffer_head].LEN_RSSI_SNR);

        if (position >= 0 && position < strlen(sensors_total))
        {
            //Copy the first part of the string to UART_buffer[0].SENSORS_PREQUELA
            strncpy(UART_buffer[buffer_head].SENSORS_PREQUELA, sensors_total, position);

            //Null-terminate UART_buffer[0].SENSORS_PREQUELA
            UART_buffer[buffer_head].SENSORS_PREQUELA[position] = '\0';

            // Copy the second part of the string to UART_buffer[0].SENSORS_SEQUELA
            strcpy(UART_buffer[buffer_head].SENSORS_SEQUELA, sensors_total + position);
        }

        sscanf(UART_buffer[buffer_head].LEN_RSSI_SNR, "%4[^,],%4[^,]%4[^\0]", length, rssi, snr);

        snprintf(rssi_text + strlen(rssi_text),sizeof(rssi_text), "%s", rssi);
        snprintf(rssi_text + strlen(rssi_text),sizeof(rssi_text), "%s", snr_text);
        snprintf(rssi_text + strlen(rssi_text),sizeof(rssi_text), "%s", snr);

        strcpy(UART_buffer[buffer_head].RSSI_SNR, rssi_text);

        xSemaphoreGive(xMutex_Access_UART_Buffer);
    }

    //Store start time (set manually in TIME.h)
    //start_time = getTime();

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
    strftime(current_time_str, sizeof(current_time_str), "%m-%d-%Y %H:%M:%S", current_time_info);

    strcpy(UART_buffer[buffer_head].TIMESTAMP, current_time_str);

    //Increments buffer head, wraps around when buffer is full (circular buffer)
    buffer_head = (buffer_head + 1) % BUFFER_SIZE;

    if(xSemaphoreTake(xMutex_Access_Num_Msgs, portMAX_DELAY) == pdTRUE)
    {
        num_msgs++;

        xSemaphoreGive(xMutex_Access_Num_Msgs);
    }

    return buffer_head;
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
