#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <cerrno>
#include <cstring>
#include <time.h>
#include "time_operate.h"


/**
 * \brief 根据基准时间和时间间隔由当前的时间戳得出下一个固定的时间戳
 */
int get_next_definite_time(const TimeInfo *timeInfo, int interval, time_t *timestamp) {

    if(interval <= 0)
    {
        printf("file: __FILE__, line: %d,shedule interval %d <= 0",  \
				__LINE__, interval);

        return EINVAL;
    }

    struct tm tm_current;
    time_t g_current_time = time(NULL);
    localtime_r((time_t *)&g_current_time, &tm_current);

    if (timeInfo->hour == -1)
    {
        *timestamp = g_current_time + interval;
    }
    else
    {
        struct tm tm_base;
        time_t time_base;
        if (tm_current.tm_hour > timeInfo->hour || \
				(tm_current.tm_hour == timeInfo->hour \
				&& tm_current.tm_min >= timeInfo->minute))
        {  //如果当前的时间(不考虑年,月,日)它的小时级别大于 设置的ScheduleEntry.time_base,
            //那么就用当前的时间
            memcpy(&tm_base, &tm_current, sizeof(struct tm));
        }
        else  //否则用tm_current的前一天
        {

            time_base = g_current_time - 24 * 3600;
            localtime_r(&time_base, &tm_base);
        }

        tm_base.tm_hour = timeInfo->hour;
        tm_base.tm_min = timeInfo->minute;
        if (timeInfo->second >= 0 && timeInfo->second <= 59)
        {
            tm_base.tm_sec = timeInfo->second;
        }
        else
        {
            tm_base.tm_sec = 0;
        }
        //tm_base的小时以下的时间与ScheduleEntry.time_base一样,而tm_base的年月日则和当前时间一样
        time_base = mktime(&tm_base);

        *timestamp = g_current_time +  interval - (g_current_time - \
					time_base) % interval;
    }
    return 0;
}