/*
 * common.c
 *
 *  Created on: 2014年6月23日
 *      Author: lionel
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "common.h"

int
writen(int fd, const void *vptr, unsigned int n)
{
	unsigned int nwrite, nleft;
	const char  *ptr;

	ptr = vptr;
	nleft = n;

	while(nleft > 0)
	{
		if ((nwrite = write(fd, ptr, nleft)) <= 0)
		{
			if(nwrite < 0 && nwrite == EINTR)
			{
				nwrite = 0;
			}
			else
			{
				return -1;
			}
		}

		nleft -= nwrite;
		ptr += nwrite;
	}

	return n;
}
