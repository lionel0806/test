/*
 * test.c
 *
 *  Created on: 2014Äê5ÔÂ16ÈÕ
 *      Author: lionel
 */

#include <stdio.h>
#include <stdlib.h>

#include "mem_debug.h"


int main(int argc, char **argv){

	char *buf;

	buf = MALLOC(1024);
	if(!buf)
	{
		printf("[%s:%d] >>> malloc error!\n",__FUNCTION__,__LINE__);
		return 1;
	}


	FREE(buf);
	FREE(buf);
	FREE(buf);	

	return 0;
}
