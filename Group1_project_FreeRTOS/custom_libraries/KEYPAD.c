// Includes.
#include "KEYPAD.h"

char key[1];

// KEYPAD INTERRUPT CODE:
//------------------------------------------------------------------- PortEIntHandler: -------------------------------------------------------------------

void PortEIntHandler(void)
{
    int32_t path;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Write in all lines the value correspondent to LINE_Y1, setting to 1 only the line Y1 (in this case) (example: 0x01 => 0 0 0 1).
    GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3), LINE_Y1);

    // Scan all X columns and write in "path" variable the hex value (ex:. 0 0 1 0 => 0x02).
    path = GPIOPinRead(GPIO_PORTE_BASE,(GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5));

    // Because of GPIOPinRead() function that make a shift of 2 bits before returning he value...
    path = (path >> 2);

    if (path == COLUMN_X1)
    {
        strcpy(key, "1");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);

    }
    else if (path == COLUMN_X2)
    {
        strcpy(key, "2");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);

    }
    else if (path == COLUMN_X3)
    {
        strcpy(key, "3");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);
    }
    else if (path == COLUMN_X4)
    {
        strcpy(key, "F");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);
    }

    // Write in all lines the value correspondent to LINE_Y2, setting to 1 only the line Y2 (in this case) (example: 0x02 => 0 0 1 0).
    GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3), LINE_Y2);

    // Scan all X columns and write in "path" variable the hex value (example: 0 0 1 0 => 0x02).
    path = GPIOPinRead(GPIO_PORTE_BASE,(GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5));

    // Because of GPIOPinRead() function that make a shift of 2 bits before returning he value...
    path = (path >> 2);

    if (path == COLUMN_X1)
    {
        strcpy(key, "4");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);
    }
    else if (path == COLUMN_X2)
    {
        strcpy(key, "5");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);
    }
    else if (path == COLUMN_X3)
    {
        strcpy(key, "6");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);
    }
    else if (path == COLUMN_X4)
    {
        strcpy(key, "E");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);
    }

    // Write in all lines the value correspondent to LINE_Y3, setting to 1 only the line Y3 (in this case) (example: 0x04 => 0 1 0 0)
    GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3), LINE_Y3);

    // Scan all X columns and write in "path" variable the hex value (example: 0 0 1 0 => 0x02)
    path = GPIOPinRead(GPIO_PORTE_BASE,(GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5));

    // Because of GPIOPinRead() function that make a shift of 2 bits before returning he value...
    path = (path >> 2);

    if (path == COLUMN_X1)
    {
        strcpy(key, "7");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);
    }
    else if (path == COLUMN_X2)
    {
        strcpy(key, "8");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);
    }
    else if (path == COLUMN_X3)
    {
        strcpy(key, "9");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);
    }
    else if (path == COLUMN_X4)
    {
        strcpy(key, "D");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);
    }


    // Write in all lines the value correspondent to LINE_Y4, setting to 1 only the line Y4 (in this case) (example: 0x08 => 1 0 0 0).
    GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3), LINE_Y4);

    // Scan all X columns and write in "path" variable the hex value (example: 0 0 1 0 => 0x02)
    path = GPIOPinRead(GPIO_PORTE_BASE,(GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5));

    // Because of GPIOPinRead() function that make a shift of 2 bits before returning he value...
    path = (path >> 2);

    if (path == COLUMN_X1)
    {
        strcpy(key, "A");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);
    }
    else if (path == COLUMN_X2)
    {
        strcpy(key, "0");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);
    }
    else if (path == COLUMN_X3)
    {
        strcpy(key, "B");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);
    }
    else if (path == COLUMN_X4)
    {
        strcpy(key, "C");

        vTaskNotifyGiveFromISR(xKeypad_Task, &xHigherPriorityTaskWoken);
    }

    // Clear the INT status for all GPIO pins of the port E.
    GPIOIntClear(GPIO_PORTE_BASE, GPIOIntStatus(GPIO_PORTE_BASE, true));

    // Puts all Y pins to HIGH to check if any key was pressed. (Key pressed = complete path).
    GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3), GPIO_PIN_ALL);

    // To prevent the program to get stuck in the ISR.
    portYIELD_FROM_ISR(true);
}
