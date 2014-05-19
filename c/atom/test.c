/*
 * test.c
 *
 *  Created on: 2014Äê5ÔÂ13ÈÕ
 *      Author: lionel
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atom.h"

int main(int argc, char **argv){

	long n = 987654321;
	char *str = "hello world !";
	char *p;
	int len;

	p = atom_string(str);
	printf("============\n");
	len = atom_length(p);
	printf("str len:%d\n",len);

	p = atom_int(n);
	printf("[%s:%d] >>> \n",__FUNCTION__,__LINE__);
	len = atom_length(p);
	printf("int len:%d\n",len);
	printf("int:%s\n",p);

	return 0;
}
