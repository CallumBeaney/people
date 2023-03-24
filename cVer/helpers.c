#include <time.h>
const char *getDate(char *);


const char *getDate(char *date)
{
    time_t t = time(NULL);               // get current time data
    struct tm *timeData = localtime(&t); // parse into fields using time.h lib fn

    int year = timeData->tm_year + 1900; // add 1900 to tm_year field to get actual year value
    int month = timeData->tm_mon + 1;    // add 1 to the tm_mon field
    int day = timeData->tm_mday;

    sprintf(date, "%04d-%02d-%02d\n", year, month, day);

    return date;
}