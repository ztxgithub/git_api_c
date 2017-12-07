
#ifndef __NETWORK_OP_H__
#define __NETWORK_OP_H__

#include <net/if.h>
#include "common_define.h"

#define IP_ADDRESS_SIZE (16)

/** get local host ip addresses 获取本机所有的网络接口地址保存到字符串数组中
 *  parameters:
 *          ip_addrs: store the ip addresses
 *          max_count: max ip address (max ip_addrs elements)
 *          count: store the ip address count
 *  return: error no, 0 success, != 0 fail
*/
int getlocaladdrs(char ip_addrs[][IP_ADDRESS_SIZE], \
	const int max_count, int *count);


#endif