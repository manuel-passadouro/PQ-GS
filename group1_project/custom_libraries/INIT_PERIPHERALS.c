
/* Program Description: Library for interupt configuration for
 * TIVA C (ek-tm4c123gxl) based project. */

#include "INIT_PERIPHERALS.h"

//-------------------------------------------------------------------- Init_Peripherals: ------------------------------------------------------------------------

void Init_Peripherals()
{
    //Enable and wait for the port to be ready for access
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);  //PWM Buzzer (PC4)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC); //LCD EN (PC5)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); //Keypad X (PE2, PE3, PE4, PE5) => (X1, X2, X3, X4) and LCD RS (PE0)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); //Keypad Y (PB0, PB1, PB2, PB3) => (Y1, Y2, Y3, Y4) and LCD Data pins (PB4, PB5, PB6, PB7) => (DB4, DB5, DB6, DB7)

    //Check if the peripheral access is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0));

    //For debug purposes we can comment the peripherals we don't want to use
    Init_Buzzer();
    Init_Keypad();
    Init_Lcd();
    Init_Tmp100();
    Init_Uart();

    //Initializing LCD
    Lcd_Init();

    //Initializing I2C
    Init_I2C();
}

//-------------------------------------------------------------------- Init_Buzzer: ------------------------------------------------------------------------

void Init_Buzzer(void)
{
    //PWM pin
    GPIOPinConfigure(GPIO_PC4_M0PWM6);
    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);

    //Configure PWM
    PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    // Configure PWM period and starting pulse width
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 20000);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 10000);
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
}

//-------------------------------------------------------------------- Init_Keypad: ------------------------------------------------------------------------

void Init_Keypad(void)
{
    //Enable port E interrupt handler keypad input
    GPIOIntRegister(GPIO_PORTE_BASE, PortEIntHandler);

    //Enable the GPIO pin for digital function (OUTPUTS)
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);       //Keypad X (PE2, PE3, PE4, PE5) => (X1, X2, X3, X4)
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);      //Keypad Y (PB0, PB1, PB2, PB3) => (Y1, Y2, Y3, Y4)

    //Set the type of the interrupt detection for all X pins (on HIGH).
    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_HIGH_LEVEL);

    //Enable interrupts on PE (X)
    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

    //Puts all Y pins to HIGH to check if any key was pressed. (Key pressed = complete path)
    GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3), GPIO_PIN_ALL);
}

//-------------------------------------------------------------------- Init_Lcd: ------------------------------------------------------------------------

void Init_Lcd(void)
{
    //Enable the GPIO pin for digital function (OUTPUTS).
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);      //LCD Data pins (PB4, PB5, PB6, PB7) => (DB4, DB5, DB6, DB7)
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);                                             //RS (PE0)
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);                                             //EN (PC5)
}

//-------------------------------------------------------------------- Init_Tmp100: ------------------------------------------------------------------------

void Init_Tmp100(void)
{
    //Enable the I2C1 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1));

    //Reset I2C1 module
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1));

    //Enable GPIOA peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //Configure PA6 as I2C1SCL and PA7 as I2C1SDA
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
}

//-------------------------------------------------------------------- Init_Uart: ------------------------------------------------------------------------

void Init_Uart()
{
   GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

   //Configure the UART pins (PC6 and PC7)
   GPIOPinConfigure(GPIO_PC6_U3RX);
   GPIOPinConfigure(GPIO_PC7_U3TX);

   //Enable the UART peripheral
   SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
   while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART3)) {}

   //Configure UART with a baud rate of 9600, 8-bit data, no parity, and one stop bit
   UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}
