
/* Program Description: Library for interupt configuration for
 * TIVA C (ek-tm4c123gxl) based project. */

#include "KEYPAD.h"

//------------------------------------------------------------------- KEYPAD INTERRUPT CODE: -------------------------------------------------------------------

QueueHandle_t lcdQueue;

void PortEIntHandler(void)
{
    int32_t path;
    char key;

    //Write in all lines the value correspondent to LINE_Y1, setting to 1 only the line Y1 (in this case) (ex:. 0x01 => 0 0 0 1)
    GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3), LINE_Y1);

    //Scan all X columns and write in "path" variable the hex value (ex:. 0 0 1 0 => 0x02)
    path = GPIOPinRead(GPIO_PORTE_BASE,(GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5));

    //Because of GPIOPinRead() function that make a shift of 2 bits before returning he value...
    path = (path >> 2);

    if (path == COLUMN_X1)
    {
        key = '1';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        SysCtlDelay(7000000);
        //vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else if (path == COLUMN_X2)
    {
        key = '2';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        SysCtlDelay(7000000);
        //vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else if (path == COLUMN_X3)
    {
        key = '3';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else if (path == COLUMN_X4)
    {
        key = 'F';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    //Write in all lines the value correspondent to LINE_Y2, setting to 1 only the line Y2 (in this case) (ex:. 0x02 => 0 0 1 0)
    GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3), LINE_Y2);

    //Scan all X columns and write in "path" variable the hex value (ex:. 0 0 1 0 => 0x02)
    path = GPIOPinRead(GPIO_PORTE_BASE,(GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5));

    //Because of GPIOPinRead() function that make a shift of 2 bits before returning he value...
    path = (path >> 2);

    if (path == COLUMN_X1)
    {
        key = '4';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else if (path == COLUMN_X2)
    {
        key = '5';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else if (path == COLUMN_X3)
    {
        key = '6';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else if (path == COLUMN_X4)
    {
        key = 'E';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    //Write in all lines the value correspondent to LINE_Y3, setting to 1 only the line Y3 (in this case) (ex:. 0x04 => 0 1 0 0)
    GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3), LINE_Y3);

    //Scan all X columns and write in "path" variable the hex value (ex:. 0 0 1 0 => 0x02)
    path = GPIOPinRead(GPIO_PORTE_BASE,(GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5));

    //Because of GPIOPinRead() function that make a shift of 2 bits before returning he value...
    path = (path >> 2);

    if (path == COLUMN_X1)
    {
        key = '7';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else if (path == COLUMN_X2)
    {
        key = '8';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else if (path == COLUMN_X3)
    {
        key = '9';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else if (path == COLUMN_X4)
    {
        key = 'D';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }


    //Write in all lines the value correspondent to LINE_Y4, setting to 1 only the line Y4 (in this case) (ex:. 0x08 => 1 0 0 0)
    GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3), LINE_Y4);

    //Scan all X columns and write in "path" variable the hex value (ex:. 0 0 1 0 => 0x02)
    path = GPIOPinRead(GPIO_PORTE_BASE,(GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5));

    //Because of GPIOPinRead() function that make a shift of 2 bits before returning he value...
    path = (path >> 2);

    if (path == COLUMN_X1)
    {
        key = 'A';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else if (path == COLUMN_X2)
    {
        key = '0';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, false);

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else if (path == COLUMN_X3)
    {
        key = 'B';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else if (path == COLUMN_X4)
    {
        key = 'C';

        if (xQueueSend(lcdQueue, &key, portMAX_DELAY) != pdPASS)
        {
            // Handle queue full error if needed
        }

        //Lcd_Clear();
        //Lcd_Write_Char(key);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    //Clear the INT status for all GPIO pins of the port E
    GPIOIntClear(GPIO_PORTE_BASE, GPIOIntStatus(GPIO_PORTE_BASE, true));

    //Puts all Y pins to HIGH to check if any key was pressed. (Key pressed = complete path)
    GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3), GPIO_PIN_ALL);
}
