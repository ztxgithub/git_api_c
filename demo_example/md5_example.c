
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
#include "md5.h"

/*例子*/
int main()
{
    MD5_CTX md5_context;
    my_md5_init(&md5_context);
    /*把输入的数据进行分组，并进行加密,未用到的数据把其储存在MD5 text中
     *
     * \param pDataBuff:输入数据的起始地址
     * \param write_bytes:输入要加入md5计算的字节数
     * */
    my_md5_update(&md5_context, (unsigned char *)pDataBuff, write_bytes);

    int file_hash_codes[4];   //file hash code
    my_md5_final((unsigned char *)file_hash_codes, &pFileContext->md5_context);
}
