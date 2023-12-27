//Includes
#include "custom_libraries/LCD.h"
#include "custom_libraries/KEYPAD.h"
#include "custom_libraries/TMP100.h"
#include "custom_libraries/INIT_PERIPHERALS.h"

#define LED_PIN GPIO_PIN_0
#define LED_PIN_2 GPIO_PIN_1
#define BLINK_DELAY_MS 500

//--------------------------------------------------------------------- MAIN CODE: --------------------------------------------------------------------------

int main(void)
{
    Init_Peripherals();

    while(1)
    {
        TMP100_Read();
        //Receive_UART();

        //SysCtlDelay(7000000);
    }
}
