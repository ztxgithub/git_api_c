#include <ifaddrs.h>
#include "network_op.h"
#include <stdio.h>
#include <string.h>

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