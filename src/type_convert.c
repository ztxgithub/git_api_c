#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "type_convert.h"
#include "common_define.h"

/* 大端转化
 *
 * */
int64_t buff2long(const char *buff)
{
    unsigned char *p;
    p = (unsigned char *)buff;
    return  (((int64_t)(*p)) << 56) | \
		(((int64_t)(*(p+1))) << 48) |  \
		(((int64_t)(*(p+2))) << 40) |  \
		(((int64_t)(*(p+3))) << 32) |  \
		(((int64_t)(*(p+4))) << 24) |  \
		(((int64_t)(*(p+5))) << 16) |  \
		(((int64_t)(*(p+6))) << 8) | \
		((int64_t)(*(p+7)));
}

/* 大端转化　int64->char *buf
 *
 * */
void long2buff(int64_t n, char *buff)
{
    unsigned char *p;
    p = (unsigned char *)buff;
    *p++ = (n >> 56) & 0xFF;
    *p++ = (n >> 48) & 0xFF;
    *p++ = (n >> 40) & 0xFF;
    *p++ = (n >> 32) & 0xFF;
    *p++ = (n >> 24) & 0xFF;
    *p++ = (n >> 16) & 0xFF;
    *p++ = (n >> 8) & 0xFF;
    *p++ = n & 0xFF;
}