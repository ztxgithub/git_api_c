
#ifndef __TYPE_CONVERT_H__
#define __TYPE_CONVERT_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** buffer convert to 64 bits int
 *  parameters:
 *  	buff: big-endian 8 bytes buffer
 *  return: 64 bits int value
*/
int64_t buff2long(const char *buff);

/** long (64 bits) convert to buffer (big-endian)
 *  parameters:
 *  	n: 64 bits int value
 *  	buff: the buffer, at least 8 bytes space, no tail \0
 *  return: none
*/
void long2buff(int64_t n, char *buff);

/** 32 bits int convert to buffer (big-endian)
 *  parameters:
 *  	n: 32 bits int value
 *  	buff: the buffer, at least 4 bytes space, no tail \0
 *  return: none
*/
void int2buff(const int n, char *buff);

/** buffer convert to 32 bits int
 *  parameters:
 *  	buff: big-endian 4 bytes buffer
 *  return: 32 bits int value
*/
int buff2int(const char *buff);

#ifdef __cplusplus
}
#endif

#endif