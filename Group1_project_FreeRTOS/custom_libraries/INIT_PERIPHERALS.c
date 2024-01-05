// Includes.
#include "INIT_PERIPHERALS.h"

//-------------------------------------------------------------------- Init_Peripherals_Phase_1: ------------------------------------------------------------------------

void Init_Peripherals_Phase_1()
{
    // Enable and wait for the port to be ready for access.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);        // LCD EN (PC5).
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);        // Keypad X (PE2, PE3, PE4, PE5) => (X1, X2, X3, X4) and LCD RS (PE0).
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);        // Keypad Y (PB0, PB1, PB2, PB3) => (Y1, Y2, Y3, Y4) and LCD Data pins (PB4, PB5, PB6, PB7) => (DB4, DB5, DB6, DB7).
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));

    // Enable interrupts globally.
    IntMasterEnable();

    // Peripheral specific initialization routines.
    Init_Keypad();
    Init_Lcd();
}

//-------------------------------------------------------------------- Init_Peripherals_Phase_2: ------------------------------------------------------------------------

void Init_Peripherals_Phase_2()
{
    // Enable and wait for the port to be ready for access.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);         // PWM Buzzer (PC4).
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0));

    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER5);      // Initialize timer5 (32 bit) to count time for UART timestamp.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_WTIMER5));

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);        // Enable the UART peripheral.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART3));

    // Peripheral specific initialization routines.
    Init_Buzzer();
    Init_Tmp100();
    Init_Uart();
    Init_Timer();
}

//-------------------------------------------------------------------- Init_Buzzer: ------------------------------------------------------------------------

void Init_Buzzer(void)
{
    // PWM pin.
    GPIOPinConfigure(GPIO_PC4_M0PWM6);
    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);

    // Configure PWM.
    PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    // Configure PWM period and starting pulse width.
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 20000);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 10000);
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
}

//-------------------------------------------------------------------- Init_Keypad: ------------------------------------------------------------------------

void Init_Keypad(void)
{
    // Enable port E interrupt handler keypad input.
    GPIOIntRegister(GPIO_PORTE_BASE, PortEIntHandler);

    // Enable the GPIO pin for digital function (OUTPUTS).
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);       // Keypad X (PE2, PE3, PE4, PE5) => (X1, X2, X3, X4).
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);      // Keypad Y (PB0, PB1, PB2, PB3) => (Y1, Y2, Y3, Y4).

    // Set the type of the interrupt detection for all X pins (on HIGH).
    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_HIGH_LEVEL);

    // Enable interrupts on PE (X).
    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

    // Puts all Y pins to HIGH to check if any key was pressed. (Key pressed = complete path).
    GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3), GPIO_PIN_ALL);
}

//-------------------------------------------------------------------- Init_Lcd: ------------------------------------------------------------------------

void Init_Lcd(void)
{
    // Enable the GPIO pin for digital function (OUTPUTS).
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);      // LCD Data pins (PB4, PB5, PB6, PB7) => (DB4, DB5, DB6, DB7).
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);                                             // RS (PE0).
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);                                             // EN (PC5).

    // Calls LCD start-up commands.
    Lcd_Init();

    Lcd_Clear();
}

//-------------------------------------------------------------------- Init_Tmp100: ------------------------------------------------------------------------

void Init_Tmp100(void)
{
    // Enable the I2C1 peripheral.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1));

    // Reset I2C1 module.
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1));

    // Enable GPIOA peripheral.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Configure PA6 as I2C1SCL and PA7 as I2C1SDA.
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

    // Calls I2C setup.
    Init_I2C();
}

//-------------------------------------------------------------------- Init_Uart: ------------------------------------------------------------------------

void Init_Uart()
{
   // Configure the UART pins (PC6 and PC7).
   GPIOPinConfigure(GPIO_PC6_U3RX);
   GPIOPinConfigure(GPIO_PC7_U3TX);

   GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

   // Configure UART with a baud rate of 9600, 8-bit data, no parity, and one stop bit.
   UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

   // Enable UART3 interrupt with a priority appropriate for your application.
   IntEnable(INT_UART3);
   UARTIntRegister(UART3_BASE, UART3IntHandler);
   UARTIntEnable(UART3_BASE, UART_INT_RX);
}

//-------------------------------------------------------------------- Init_Timer: ------------------------------------------------------------------------

void Init_Timer()
{
    // Full width time that count up.
    TimerConfigure(WTIMER5_BASE, TIMER_CFG_PERIODIC_UP);

    // Load the value 0xFFFFFFFFFFFFFFFF.
    TimerLoadSet64(WTIMER5_BASE, 0xFFFFFFFFFFFFFFFF);

    // Enable the timer A.
    TimerEnable(WTIMER5_BASE, TIMER_A);
}
