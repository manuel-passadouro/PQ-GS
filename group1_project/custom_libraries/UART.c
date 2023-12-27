/* Program Description: UART library for TIVA C
 * devboard (ek-tm4c123gxl) communication with pqube board. */

#include "UART.h"

//--------------------------------------------------------------------- Receive_UART: --------------------------------------------------------------------------

void Receive_UART(void)
{
    uint8_t i = 0;
    char receivedChar;
    char buffer[buffer_size];
    int charP=0;

    while (i < buffer_size - 1)
    {
        //Waits until one character be ready to be read
        while (!UARTCharsAvail(UART3_BASE));

        //Read a character from UART COM
        receivedChar = UARTCharGet(UART3_BASE);

        //Checks if it is a 'P' character
        if (receivedChar == 'P')
        {
            charP=1;
        }

        //Checks if it is a terminal character
        if (receivedChar == '\n' || receivedChar == '\r')
        {
            charP=0;
            break;
        }

        //Stores the character in the buffer
        if(charP == 1)
        {
            buffer[i++] = receivedChar;
            Lcd_Write_Char(receivedChar);
            //return buffer;
        }
    }

    //Add a null character to the end of the string
    buffer[i] = '\0';
}

void UART3IntHandler(void)
{
    uint32_t ui32Status;

       // Get the interrupt status (masked interrupt).
       ui32Status = UARTIntStatus(UART3_BASE, true);

       // Clear interrupt flag.
       UARTIntClear(UART3_BASE, ui32Status);

       // Check if the receive interrupt is triggered (if status matches UART interrupt mask).
       if (ui32Status & UART_INT_RX)
       {
           Receive_UART();
       }
}

