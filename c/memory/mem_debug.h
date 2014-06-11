/*
 * mem_debug.h
 *
 *  Created on: 2014Äê5ÔÂ16ÈÕ
 *      Author: lionel
 */

#ifndef MEM_DEBUG_H_
#define MEM_DEBUG_H_

#define MALLOC(size)  (Malloc((size), (char *)__FUNCTION__, __LINE__))
#define FREE(ptr)  (Free((ptr), (char *)__FUNCTION__, __LINE__))

void *Malloc(size_t size, char *func_name, int line);
void Free(void *ptr, char *func_name, int line);

#endif /* MEM_DEBUG_H_ */
