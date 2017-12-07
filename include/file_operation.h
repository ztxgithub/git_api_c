
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

#endif