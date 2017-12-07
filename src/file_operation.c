#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "file_operation.h"
#include "common_define.h"

/*将buff里面的内容写到文件中
 * 文件没有就创建并且是覆盖写
 *
 *	\param filename：文件名
 *	\param buff：保存数据的缓冲区
 *	\param file_size：写到文件的字节数
 *
 *  \return 0:成功
 * */
int writeToFile(const char *filename, const char *buff, const int file_size)
{
    int fd;
    int result;

    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        result = errno != 0 ? errno : EIO;
        printf("file: "__FILE__", line: %d, " \
			"open file %s fail, " \
			"errno: %d, error info: %s", \
			__LINE__, filename, \
			result, STRERROR(result));
        return result;
    }

    if (write(fd, buff, file_size) != file_size)
    {
        result = errno != 0 ? errno : EIO;
        printf("file: "__FILE__", line: %d, " \
			"write file %s fail, " \
			"errno: %d, error info: %s", \
			__LINE__, filename, \
			result, STRERROR(result));
        close(fd);
        return result;
    }

    if (fsync(fd) != 0)
    {
        result = errno != 0 ? errno : EIO;
        printf("file: "__FILE__", line: %d, " \
			"fsync file \"%s\" fail, " \
			"errno: %d, error info: %s", \
			__LINE__, filename, \
			result, STRERROR(result));
        close(fd);
        return result;
    }

    close(fd);
    return 0;
}