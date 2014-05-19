/*
 * test.c
 *
 *  Created on: 2014Äê5ÔÂ14ÈÕ
 *      Author: lionel
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <error.h>

int main(int argc, char **argv){

	int a;
	char *p = NULL;
//	a = -1;
//	assert(!"can't happen!");
	printf("p:%p\n", p);
	free(p);
	printf("err info:%s\n", error);
}


