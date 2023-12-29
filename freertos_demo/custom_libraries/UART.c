/* Program Description: UART library for TIVA C
 * devboard (ek-tm4c123gxl) communication with pqube board. */

#include "UART.h"


//--------------------------------------------------------------------- Receive_UART: --------------------------------------------------------------------------

void Receive_UART(void)
{
   uint8_t i = 0;
    char receivedChar;
    char msg[MSG_SIZE];
    int charP=0;

    uint64_t timestamp;
    time_t current_time_t;
    time_t raw_time_t;
    time_t start_time_t;

    struct tm start_time;
    struct tm *current_time_info;

    char current_time_str[20];

    while (i < MSG_SIZE - 1)
    {
        //Waits until one character be ready to be read
        while (!UARTCharsAvail(UART3_BASE));

        //Read a character from UART COM
        receivedChar = UARTCharGet(UART3_BASE);

        //Checks if it is a 'P' character
        if (receivedChar == 'P')
        {
            charP = 1;
        }

        //Checks if it is a terminal character
        if (receivedChar == '\n' || receivedChar == '\r')
        {
            charP = 0;
            break;
        }

        //Stores the character in the buffer
        if(charP == 1)
        {
            msg[i++] = receivedChar;
        }
    }

    start_time = getTime(); //Store start time (set manually in TIME.h).

    // Get the current timestamp from the system timer
    timestamp = TimerValueGet(WTIMER5_BASE, TIMER_A);

    // Calculate elapsed seconds using clock frequency
    raw_time_t =  (time_t)((timestamp / SysCtlClockGet()));

    // Convert start_time to time_t format for calculations (returns the value in seconds!)
    start_time_t = mktime(&start_time);

    // Calculate the current time as time_t
    current_time_t = start_time_t + raw_time_t;

    // Convert time in time_t to standard C time format
    current_time_info = localtime(&current_time_t);

    // Convert from standard C time format to string
    strftime(current_time_str, sizeof(current_time_str), "%Y-%m-%d %H:%M:%S", current_time_info);

    Lcd_Clear();
    Lcd_Write_String(current_time_str);

    // Append time string to msg string using sprintf
    //sprintf(msg + strlen(msg), "%s", current_time_str);

    // Add a null character at the end of string
    msg[sizeof(msg) - 1] = '\0';

    //Store the received message in UART_buffer
    //strncpy(UART_buffer[buffer_head], msg, MSG_SIZE);

    // Store the received message in the circular buffer
    //StoreInCircularBuffer(msg);

    //Send msg string to LCD queue (sends every message)
    //xQueueSend(lcdQueue, UART_buffer[buffer_head], portMAX_DELAY);

    //Send a specific msg string to LCD queue (ex: msg no. 5 in buffer)


     //xQueueSend(lcdQueue, circularBuffer[5], portMAX_DELAY);

}

void UART3IntHandler(void)
{

    uint32_t ui32Status;

    //Get the interrupt status (masked interrupt should be RX type)
    ui32Status = UARTIntStatus(UART3_BASE, true);

    //Clear interrupt flag
    UARTIntClear(UART3_BASE, ui32Status);

   //Check if the receive interrupt is triggered (if status matches UART interrupt mask)
   if (ui32Status & UART_INT_RX)
   {
       Receive_UART();
   }

   //portYIELD_FROM_ISR(true);
}

void StoreInCircularBuffer(const char *msg)
{
    xSemaphoreTake(bufferMutex, portMAX_DELAY);

    strncpy(UART_buffer[buffer_head], msg, MSG_SIZE);

    buffer_head = (buffer_head + 1) % BUFFER_SIZE;

    xSemaphoreGive(bufferMutex);
}


