//Includes
#include "TMP100.h"

//--------------------------------------------------------------------- Init_I2C: --------------------------------------------------------------------------

void Init_I2C(void)
{
    //Set the I2C module clock frequency
    I2CMasterInitExpClk(I2C_BASE_ADDR, SysCtlClockGet(), false);

    //Set the I2C slave address to TMP100 (0x48)
    I2CMasterSlaveAddrSet(I2C_BASE_ADDR, TMP100_I2C_ADDRESS, false);    //Write

    //Write to config register and set 12-bit mode
    I2CMasterDataPut(I2C_BASE_ADDR, TMP100_CONFIG_REG);

    //Start burst sequence to send temp data configuration
    I2CMasterControl(I2C_BASE_ADDR, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C_BASE_ADDR));

    //Set the resolution to 12 bit (0.0625°C)
    I2CMasterDataPut(I2C_BASE_ADDR, 0x60);

    //Finish the burst sequence
    I2CMasterControl(I2C_BASE_ADDR, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(I2C_BASE_ADDR));
}

//-------------------------------------------------------------------- TMP100_Read: ------------------------------------------------------------------------

void TMP100_Read(void)
{
    uint16_t tempData;
    float temperature_raw;
    char temperature_array[20];

    //Set the I2C slave address to TMP100 (0x48)
    I2CMasterSlaveAddrSet(I2C_BASE_ADDR, TMP100_I2C_ADDRESS, false);    //Write

    //Send command byte to read temperature
    I2CMasterDataPut(I2C_BASE_ADDR, TMP100_TEMP_REG);

    //Start single sequence
    I2CMasterControl(I2C_BASE_ADDR, I2C_MASTER_CMD_SINGLE_SEND);
    while(I2CMasterBusy(I2C_BASE_ADDR));

    I2CMasterSlaveAddrSet(I2C_BASE_ADDR, TMP100_I2C_ADDRESS, true);     //Read

    //Start data reception, get the 1st byte
    I2CMasterControl(I2C_BASE_ADDR, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(I2CMasterBusy(I2C_BASE_ADDR));

    //Store the 1st byte on temData but first does an 8 bit shift left to get storage for the 2nd byte
    tempData = I2CMasterDataGet(I2C_BASE_ADDR) << 8;

    //Finish data reception, get 2nd byte
    I2CMasterControl(I2C_BASE_ADDR, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(I2C_BASE_ADDR));

    //Update tempData by performing a bitwise OR operation with the data retrieved from the I2C master
    tempData |= I2CMasterDataGet(I2C_BASE_ADDR);

    //Perform a 4 bit shift right on tempData because the temperature data is just a 12 bit
    tempData = tempData >> 4;

    //Convert the tempData value by dividing by 256
    temperature_raw =  tempData * 0.0625;

    //Convert the float value of the reading to an array to show in LCD later
    FloatToArray(temperature_raw, temperature_array, 3);

    Lcd_Clear();
    Lcd_Write_String(temperature_array);
}

//---------------------------------------------------------------------- Reverse: --------------------------------------------------------------------------

void Reverse(char* str, int len)
{
    //Reverses a string 'str' of length 'len'

    int i = 0, j = len - 1, temp;

    while (i < j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

//-------------------------------------------------------------------- IntToString: ------------------------------------------------------------------------

int IntToString(int x, char str[], int d)
{
    //Converts a given integer x to string str[].
    //d is the number of digits required in the output.
    //If d is more than the number of digits in x, then 0s are added at the beginning.

    int i = 0;

    while (x)
    {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }

    // If number of digits required is more, then add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    Reverse(str, i);

    str[i] = '\0';

    return i;
}

//-------------------------------------------------------------------- FloatToArray: -----------------------------------------------------------------------

void FloatToArray(float n, char* res, int afterpoint)
{
    // Converts a floating-point/double number to a string

    //Extract integer part
    int ipart = (int)n;

    //Extract floating part
    float fpart = n - (float)ipart;

    //Convert integer part to string
    int i = IntToString(ipart, res, 0);

    //Check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';   //Add dot

        //Get the value of fraction part up to given number of points after dot. The third parameter is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        IntToString((int)fpart, res + i + 1, afterpoint);
    }
}





