#ifndef TMP100_H
#define TMP100_H

// Includes.
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "inc/hw_i2c.h"
#include <math.h>
#include "LCD.h"

// I2C TMP100 defines.
#define I2C_BASE_ADDR I2C1_BASE
#define TMP100_I2C_ADDRESS 0x48
#define TMP100_CONFIG_REG 0x01
#define TMP100_TEMP_REG 0x00
#define MAX_STRING_LENGTH 10

//------------------------------------------------------------------------- PROTOTYPES: ------------------------------------------------------------------------

void Init_I2C(void);
float TMP100_Read(void);
void Reverse(char* str, int len);
int IntToString(int x, char str[], int d);
void FloatToArray(float n, char* res, int afterpoint);

#endif // TMP100_H.
