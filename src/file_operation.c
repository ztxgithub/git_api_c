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

/* 设置文件的属性值,像O_NONBLOCK or FD_CLOEXEC
 * 先获取文件的flags状态,再保持原来其他flag不变,只该某一个属性值
 *
 * */
static int fcntl_add_flags(int fd, int get_cmd, int set_cmd, int adding_flags)
{
    int flags;

    flags = fcntl(fd, get_cmd, 0);
    if (flags < 0)
    {
        printf("file: "__FILE__", line: %d, " \
			"fcntl fail, errno: %d, error info: %s.", \
			__LINE__, errno, STRERROR(errno));
        return errno != 0 ? errno : EACCES;
    }

    if (fcntl(fd, set_cmd, flags | adding_flags) == -1)
    {
        printf("file: "__FILE__", line: %d, " \
			"fcntl fail, errno: %d, error info: %s.", \
			__LINE__, errno, STRERROR(errno));
        return errno != 0 ? errno : EACCES;
    }

    return 0;
}

/*　设置文件TFL类型的属性,例如　O_NONBLOCK　非阻塞
 *
 * */
int fd_add_flags(int fd, int adding_flags)
{
    return fcntl_add_flags(fd, F_GETFL, F_SETFL, adding_flags);
}

int fd_set_cloexec(int fd)
{
    return fcntl_add_flags(fd, F_GETFD, F_SETFD, FD_CLOEXEC);
}

/* 通过不同的函数名取得对应getsockname功能或则getpeername功能
 *
 * \param getname:函数名 getsockname或则getpeername
 * \param sock:套接字
 * \param buff:保存ip地址的字符串的指针
 * \param bufferSize:限定最大的字符数字
 *
 * \return 失败返回　INADDR_NONE
 * 		　 成功返回 网络字节序ip
 * */
in_addr_t getIpaddr(getnamefunc getname, int sock, \
		char *buff, const int bufferSize)
{
    struct sockaddr_in addr;
    socklen_t addrlen;

    memset(&addr, 0, sizeof(addr));
    addrlen = sizeof(addr);

    if (getname(sock, (struct sockaddr *)&addr, &addrlen) != 0)
    {
        *buff = '\0';
        return INADDR_NONE;
    }

    if (addrlen > 0)
    {
        if (inet_ntop(AF_INET, &addr.sin_addr, buff, bufferSize) == NULL)
        {
            *buff = '\0';
        }
    }
    else
    {
        *buff = '\0';
    }

    return addr.sin_addr.s_addr;
}