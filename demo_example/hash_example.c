
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
    int file_hash_codes[4];
    /*hash初始化*/
    INIT_HASH_CODES4(pFileContext->file_hash_codes)
    /* 对数据进行hash计算
     * \param pDataBuff:数据的起始地址
     * \param write_bytes:要加入hash计算的字节数
     * */
    CALC_HASH_CODES4(pDataBuff, write_bytes, file_hash_codes)


}
