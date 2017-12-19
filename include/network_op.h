
#ifndef __NETWORK_OP_H__
#define __NETWORK_OP_H__

#include <net/if.h>
#include "common_define.h"

#define IP_ADDRESS_SIZE (16)

#ifdef __cplusplus
extern "C" {
#endif

/** start a socket server (socket, bind and listen)
 *  parameters:
 *          bind_ipaddr: the ip address to bind
 *          port: the port to bind
 *          err_no: store the error no
 *  return: >= 0 server socket, < 0 fail
*/
int socketServer(const char *bind_ipaddr, const int port, int *err_no);

/** connect to server by non-block mode
 *  parameters:
 *          sock: the socket
 *          server_ip: ip address of the server
 *          server_port: port of the server
 *          timeout: connect timeout in seconds
 *          auto_detect: if detect and adjust the block mode of the socket
 *  return: error no, 0 success, != 0 fail
*/
int connectserverbyip_nb_ex(int sock, const char *server_ip, \
		const short server_port, const int timeout, \
		const bool auto_detect);

/** send data (non-block mode)
 *  parameters:
 *          sock: the socket
 *          data: the buffer to send
 *          size: buffer size
 *          timeout: write timeout
 *  return: error no, 0 success, != 0 fail
*/
int tcpsenddata_nb(int sock, void* data, const int size, const int timeout);

/** get local host ip addresses 获取本机所有的网络接口地址保存到字符串数组中
 *  parameters:
 *          ip_addrs: store the ip addresses
 *          max_count: max ip address (max ip_addrs elements)
 *          count: store the ip address count
 *  return: error no, 0 success, != 0 fail
*/
int getlocaladdrs(char ip_addrs[][IP_ADDRESS_SIZE], \
	const int max_count, int *count);

#ifdef __cplusplus
}
#endif

#endif