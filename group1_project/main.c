//Includes
#include <string.h>
#include "custom_libraries/LCD.h"
#include "custom_libraries/KEYPAD.h"
#include "custom_libraries/TMP100.h"
#include "custom_libraries/INIT_PERIPHERALS.h"

#define LED_PIN GPIO_PIN_0
#define LED_PIN_2 GPIO_PIN_1
#define BLINK_DELAY_MS 500

//--------------------------------------------------------------------- MAIN CODE: --------------------------------------------------------------------------

char UART_buffer[BUFFER_SIZE][MSG_SIZE];
int buffer_head;
int num_msgs = 0;

int main(void)
{
    memset(UART_buffer, 0, sizeof(UART_buffer)); //Init buffer at 0.
    buffer_head = 0; //Set head of buffer to 0.

    Init_Peripherals();

    while(1)
    {
        //TMP100_Read();
        //Receive_UART();

        //SysCtlDelay(7000000);
    }
}
