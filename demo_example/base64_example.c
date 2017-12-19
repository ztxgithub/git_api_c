
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
    struct base64_context context;
    base64_init(&context, 8);
    base64_set_line_separator(&context, "]]]]]]");
    char src[] = "this is a example";
    char dst[256];
    memset(dst, 0, sizeof(dst));
    int dst_len;
//进行base64标准编码
    char *pbuf = base64_encode_ex(&context, src, strlen(src), dst, &dst_len, true);
    int encode_len = base64_get_encode_length(&context, strlen(src));
    printf("encode_len[%d]\n", encode_len);
    if(pbuf)
    {
        printf("dst[%s], dst_len[%d]\n", pbuf, dst_len);
    } else
    {
        printf("pbuf null\n");
    }

    char str_decode[256];
    int decode_len;
    char *pdecode = base64_decode(&context, pbuf, dst_len, str_decode, &decode_len);
//    char *pdecode = base64_decode_auto(&context, pbuf, dst_len, str_decode, &decode_len);
    printf("pdecode[%s], decode_len[%d]\n", pdecode, decode_len);
}
