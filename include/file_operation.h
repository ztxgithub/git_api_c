
#ifndef __FILE_OPERATION_H__
#define __FILE_OPERATION_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*getnamefunc)(int socket, struct sockaddr *address, \
		socklen_t *address_len);

#define getSockIpaddr(sock, buff, bufferSize) \
	getIpaddr(getsockname, sock, buff, bufferSize)

#define getPeerIpaddr(sock, buff, bufferSize) \
	getIpaddr(getpeername, sock, buff, bufferSize)

/** write to file
 *  parameters:
 *  	filename: the filename to write
 *  	buff: the buffer to write
 *  	file_size: the file size
 *  return: error no , 0 success, != 0 fail
*/
int writeToFile(const char *filename, const char *buff, const int file_size);

/** set fd flags such as O_NONBLOCK
 *  parameters:
 *  	fd: the fd to set
 *  	adding_flags: the flags to add
 *  return: error no , 0 success, != 0 fail
*/
int fd_add_flags(int fd, int adding_flags);

/** set fd FD_CLOEXEC flags
 *  parameters:
 *  	fd: the fd to set
 *  return: error no , 0 success, != 0 fail
*/
int fd_set_cloexec(int fd);

/** get ip address
 *  parameters:
 *          getname: the function name, should be getpeername or getsockname
 *          sock: the socket
 *          buff: buffer to store the ip address
 *          bufferSize: the buffer size (max bytes)
 *  return: in_addr_t, INADDR_NONE for fail
*/
in_addr_t getIpaddr(getnamefunc getname, int sock, \
		char *buff, const int bufferSize);

#ifdef __cplusplus
}
#endif

#endif