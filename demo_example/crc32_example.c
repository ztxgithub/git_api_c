
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
#include "hash.h"

/*例子*/
int main()
{
    int crc32 = CRC32_XINIT;

    /*
     *  对CRC32进行计算,其中pDataBuff为数据起始地址,write_bytes为需要计算的字节数
     *   crc32 为原来的crc32
     * */
    crc32 = CRC32_ex(pDataBuff, write_bytes, crc32);

    /*
     * 最后的收尾操作
     * */
    crc32 = CRC32_FINAL(crc32);

}
