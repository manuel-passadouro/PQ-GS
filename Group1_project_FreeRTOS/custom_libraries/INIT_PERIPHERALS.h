#ifndef INIT_PERIPHERALS_H
#define INIT_PERIPHERALS_H

//Includes
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "LCD.h"
#include "TMP100.h"
#include "KEYPAD.h"
#include "UART.h"
#include "CUSTOM_TASK.h"

//------------------------------------------------------------------------- PROTOTYPES: ------------------------------------------------------------------------

void Init_Peripherals(void);
void Init_Buzzer(void);
void Init_Keypad(void);
void Init_Lcd(void);
void Init_Tmp100(void);
void Init_Uart(void);
void Init_Timer(void);

#endif //Interrupts_H
