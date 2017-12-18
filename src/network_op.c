#include <ifaddrs.h>
#include "network_op.h"
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <fcntl.h>

/* 对指定的ip:port进行socket bind绑定
 *
 * */
static int socketBind(int sock, const char *bind_ipaddr, const int port)
{
    struct sockaddr_in bindaddr;

    bindaddr.sin_family = AF_INET;
    bindaddr.sin_port = htons(port);
    if (bind_ipaddr == NULL || *bind_ipaddr == '\0')
    {
        bindaddr.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        if (inet_aton(bind_ipaddr, &bindaddr.sin_addr) == 0)
        {
            printf("file: "__FILE__", line: %d, " \
				"invalid ip addr %s", \
				__LINE__, bind_ipaddr);
            return EINVAL;
        }
    }

    if (bind(sock, (struct sockaddr*)&bindaddr, sizeof(bindaddr)) < 0)
    {
        printf("file: "__FILE__", line: %d, " \
			"bind port %d failed, " \
			"errno: %d, error info: %s.", \
			__LINE__, port, errno, STRERROR(errno));
        return errno != 0 ? errno : ENOMEM;
    }

    return 0;
}

/*　进行服务器的初始化
 * 	创建sock,bind,lisen(限定socket客户端连接请求数为1024)
 *
 * 	\param bind_ipaddr:字符串ip地址
 * 	\param port:要绑定的端口号
 * 	\param *err_no:错误代码,成功为0
 *
 * 	\return 成功返回对应的服务器socket
 * 			失败 < 0
 * */
int socketServer(const char *bind_ipaddr, const int port, int *err_no)
{
    int sock;
    int result;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        *err_no = errno != 0 ? errno : EMFILE;
        printf("file: "__FILE__", line: %d, " \
			"socket create failed, errno: %d, error info: %s", \
			__LINE__, errno, STRERROR(errno));
        return -1;
    }

    result = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &result, sizeof(int))<0)
    {
        *err_no = errno != 0 ? errno : ENOMEM;
        printf("file: "__FILE__", line: %d, " \
			"setsockopt failed, errno: %d, error info: %s", \
			__LINE__, errno, STRERROR(errno));
        close(sock);
        return -2;
    }

    if ((*err_no=socketBind(sock, bind_ipaddr, port)) != 0)
    {
        close(sock);
        return -3;
    }

    if (listen(sock, 1024) < 0)
    {
        *err_no = errno != 0 ? errno : EINVAL;
        printf("file: "__FILE__", line: %d, " \
			"listen port %d failed, " \
			"errno: %d, error info: %s", \
			__LINE__, port, errno, STRERROR(errno));
        close(sock);
        return -4;
    }

    *err_no = 0;
    return sock;
}

/*　获取本机所有的网络接口地址保存到字符串数组中
 *  \param ip_addrs:存放本机ip地址的指针
 *  \param max_count: 可以保存的最多的ip的个数
 *  \param *count: 实际保存ip的个数
 *
 * 	\return 0：成功
 * */
int getlocaladdrs(char ip_addrs[][IP_ADDRESS_SIZE], \
	const int max_count, int *count)
{
	struct ifaddrs *ifc;
	struct ifaddrs *ifc1;

	*count = 0;
	if (0 != getifaddrs(&ifc))
	{
		printf("file: "__FILE__", line: %d, " \
			"call getifaddrs fail, " \
			"errno: %d, error info: %s", \
			__LINE__, errno, STRERROR(errno));
		return errno != 0 ? errno : EMFILE;
	}

	ifc1 = ifc;  //保存起始指针
	while (NULL != ifc)
    {
        struct sockaddr *s;
        s = ifc->ifa_addr;
        if (NULL != s && AF_INET == s->sa_family)
        {
            if (max_count <= *count)
            {
                printf("file: "__FILE__", line: %d, "\
                        "max_count: %d < iterface count: %d", \
                        __LINE__, max_count, *count);
                freeifaddrs(ifc1);
                return ENOSPC;
            }

            if (inet_ntop(AF_INET, &((struct sockaddr_in *)s)-> \
                        sin_addr, ip_addrs[*count], IP_ADDRESS_SIZE) != NULL)
            {
                (*count)++;
            }
            else
            {
                printf("file: "__FILE__", line: %d, " \
                        "call inet_ntop fail, " \
                        "errno: %d, error info: %s", \
                        __LINE__, errno, STRERROR(errno));
            }
        }

        ifc = ifc->ifa_next;
    }

	freeifaddrs(ifc1);
	return *count > 0 ? 0 : ENOENT;
}

/* 非阻塞超时连接到服务器(注意这个函数的sock一定要是非阻塞,如果传入的sock为阻塞,则要auto_detect为true)
 *
 * \param sock: socket()函数后的socket
 * \param server_ip：服务器ip
 * \param server_port：服务器监听的port
 * \param timeout：连接超时时间
 * \param auto_detect：是否自动设置sock为非阻塞,之后变回原来的特性
 *
 * \return 0:成功连接
 * 		  EACCES：连接成功,但不是立马成功而是要超时建立成功
 * */
int connectserverbyip_nb_ex(int sock, const char *server_ip, \
		const short server_port, const int timeout, \
		const bool auto_detect)
{
    int result;
    int flags;
    bool needRestore;
    socklen_t len;

#ifdef USE_SELECT
    fd_set rset;
	fd_set wset;
	struct timeval tval;
#else
    struct pollfd pollfds;
#endif

    struct sockaddr_in addr;

    addr.sin_family = PF_INET;
    addr.sin_port = htons(server_port);
    result = inet_aton(server_ip, &addr.sin_addr);
    if (result == 0 )
    {
        return EINVAL;
    }

    if (auto_detect)  //是否自动检测
    {
        flags = fcntl(sock, F_GETFL, 0);
        if (flags < 0)
        {
            return errno != 0 ? errno : EACCES;
        }

        if ((flags & O_NONBLOCK) == 0)  //如果该sock不具备非阻塞
        {
            if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0)  //设置该sock为非阻塞
            {
                return errno != 0 ? errno : EACCES;
            }

            needRestore = true;
        }
        else  //如果该sock具备非阻塞
        {
            needRestore = false;
        }
    }
    else  //如果不需要自动检测
    {
        needRestore = false;
        flags = 0;
    }

    do
    {
        if (connect(sock, (const struct sockaddr*)&addr, \
			sizeof(addr)) < 0)  //非阻塞connect不成功(EINPROGRESS（表示连接建立，建立启动但是尚未完成) 或则 connect失败)
        {
            result = errno != 0 ? errno : EINPROGRESS;
            if (result != EINPROGRESS)  //connect 失败
            {
                break;
            }
        }
        else //connect成功
        {
            result = 0;
            break;
        }
        //到这一步说明connect==-1,表示连接建立，建立启动但是尚未完成

#ifdef USE_SELECT
        FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_SET(sock, &rset);
		FD_SET(sock, &wset);
		tval.tv_sec = timeout;
		tval.tv_usec = 0;

		result = select(sock+1, &rset, &wset, NULL, \
				timeout > 0 ? &tval : NULL);
#else
        pollfds.fd = sock;
        pollfds.events = POLLIN | POLLOUT;
        result = poll(&pollfds, 1, 1000 * timeout);
#endif

        if (result == 0)
        {
            result = ETIMEDOUT;
            break;
        }
        else if (result < 0)
        {
            result = errno != 0 ? errno : EINTR;
            break;
        }

        len = sizeof(result);
        if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &result, &len) < 0)  //getsockopt不成功
        {
            result = errno != 0 ? errno : EACCES;
            break;
        }
    } while (0);

    if (needRestore)  //适用于auto_detect为true,且原来的sock为阻塞的,先设置为非阻塞,用完connect和poll后再设置回阻塞
    {
        fcntl(sock, F_SETFL, flags);
    }

    return result;
}
