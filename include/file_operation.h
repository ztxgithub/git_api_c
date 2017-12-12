
#ifndef __FILE_OPERATION_H__
#define __FILE_OPERATION_H__

#include <stdint.h>

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

#endif