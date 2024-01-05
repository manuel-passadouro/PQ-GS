#ifndef LCD_H
#define LCD_H

// Hardware Description:
// RS ---> PE0.
// R/W ---> GND.
// EN ---> PC5.
// D4 ---> PB4.
// D5 ---> PB5.
// D6 ---> PB6.
// D7 ---> PB7.

// Includes.
#include <custom_libraries/CUSTOM_TASK.h>
#include <inc/tm4c123gh6pm.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "UART.h"

// LCD defines.
#define RS GPIO_PIN_0 // Register Select (Character or Instruction).
#define EN GPIO_PIN_5 // LCD Clock Enable PIN, Falling Edge Triggered.

// 4 bit operation.
#define D4 GPIO_PIN_4 // Bit 4.
#define D5 GPIO_PIN_5 // Bit 5.
#define D6 GPIO_PIN_6 // Bit 6.
#define D7 GPIO_PIN_7 // Bit 7.

extern QueueHandle_t lcdQueue;

//------------------------------------------------------------------------- PROTOTYPES: ------------------------------------------------------------------------

void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init(void);
void Lcd_Write_Char(char a);
void Lcd_Write_String(const char *a);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);
void Lcd_Write_Integer(int v);

#endif // LCD_H.
