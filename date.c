#include "date.h"

time_t to_seconds(char *date)
{

    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));

    time_t t;

    //The strptime() function converts the character string pointed to by buf to values which are stored in the tm structure pointed to by tm, using the format specified by format. 
    //     strptime - date and time conversion 
    strptime(date,"%d-%m-%Y",&tm);

    //mktime - convert broken-down time into time since the Epoch 
    t = mktime(&tm);

    return t;
}

int check_dates(char*date1, char* date2){

    time_t d1 , d2;

    d1 = to_seconds(date1);
    d2 = to_seconds(date2);

    if (d1 < d2 || d1 == d2){

        return 0;

        } else
        {
            if(d2 < 0){

                return 0;

            }
            else return 1;
        }

}

//The <time.h> header declares the structure tm, which includes at least the following members:

// int    tm_sec   seconds [0,61]
// int    tm_min   minutes [0,59]
// int    tm_hour  hour [0,23]
// int    tm_mday  day of month [1,31]
// int    tm_mon   month of year [0,11]