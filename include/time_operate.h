

#ifndef TIME_OPERATE_H
#define TIME_OPERATE_H

#ifdef __cplusplus
extern "C" {
#endif

#define byte signed char

typedef struct
{
    byte hour;
    byte minute;
    byte second;
} TimeInfo;

/**
 * \brief 根据基准时间和时间间隔由当前的时间戳得出下一个固定的时间戳
 *
 * \param timeInfo 基准时间 00:30:30
 *                 如果 timeInfo.hour = -1 则下一次是 当前的时间戳 + interval
 * \param interval 时间间隔(小于一天),单位秒 例如3600, 则会得出　01:30:30 02:30:30
 * \param timestamp 返回的时间戳
 *
 * \return 0:成功 -1:失败
 */
int get_next_definite_time(const TimeInfo *timeInfo, int interval, time_t *timestamp);

#ifdef __cplusplus
}
#endif

#endif //TIME_OPERATE_H
