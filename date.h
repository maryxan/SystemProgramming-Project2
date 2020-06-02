#ifndef DATE_H
#define DATE_H

#include <stdio.h>
#define __USE_XOPEN
#include <time.h>
#include <string.h>

time_t to_seconds(char *date);
int check_dates(char*date1, char* date2);


#endif