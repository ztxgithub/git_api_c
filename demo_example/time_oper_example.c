
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include "base64.h"



/*例子*/
int main()
{
    TimeInfo timeInfo;
    timeInfo.hour = 1;
    timeInfo.minute = 30;
    timeInfo.second = 30;
    time_t time;
    int interval = 3600 / 2;

    int result = get_next_definite_time(&timeInfo,  interval, &time);
    printf("result=%d, next_call_time[%ld]\n", result, time);
}
