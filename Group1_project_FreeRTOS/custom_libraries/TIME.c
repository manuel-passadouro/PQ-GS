/* Program Description: Data and time library for tiva c based project.*/

//Includes
#include "TIME.h"

//Structures
struct tm getTime(void)
{
    // Declare a struct tm variable
    struct tm my_time = {0};

    // Set the date and time components manually
    my_time.tm_year = 2023 - 1900;  // Years since 1900
    my_time.tm_mon = 11;            // Month (0-11)
    my_time.tm_mday = 28;           // Day of the month (1-31)
    my_time.tm_hour = 16;           // Hour (0-23)
    my_time.tm_min = 30;            // Minutes (0-59)
    my_time.tm_sec = 39;            // Seconds (0-61)

    return my_time;
}
