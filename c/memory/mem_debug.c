/*
 * mem_debug.c
 *
 *  Created on: 2014Äê5ÔÂ16ÈÕ
 *      Author: lionel
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mem_debug.h"


#define FUNC_NAME_LEN_MAX 256
#define HASH(p, t) (((unsigned long)(p) >> 3) & (sizeof(t)/sizeof((t)[0]) - 1))

#define PRINT_ERR(fmt...)     do{printf("\033[41m");printf(fmt);printf("\033[0m");}while(0)


static struct node{
	struct node *link;
	const void *ptr;
	long size;
	char func_name[FUNC_NAME_LEN_MAX];
	int line;
}*htable[512] = {NULL};


static void _add_node(struct node *node) {

	struct node **np = &htable[HASH(node->ptr, htable)];

	if (node) {
		/*insert first node of hash link*/
		node->link = *np;
		*np = node;
	}
}


static void _del_node(const void *ptr) {

	struct node *prev;
	struct node *np = htable[HASH(ptr, htable)];

	if (np && np->ptr == ptr) {
		htable[HASH(ptr, htable)] = np->link;
		free(np);
		return;
	}

	prev = np;
	while (np) {
		if (np->ptr == ptr) {
			prev->link = np->link;
			free(np);
			break;
		}

		prev = np;
		np = prev->link;
	}
}


static struct node *_find_node(const void *ptr) {

	struct node *np = htable[HASH(ptr, htable)];

	while (np && np->ptr != ptr)
		np = np->link;
	return np;
}


void *Malloc(size_t size, char *func_name, int line) {
	void *ptr;
	struct node *np;

	assert(size > 0);
	assert(func_name);

	ptr = malloc(size);
	if (!ptr) {
		PRINT_ERR("%s:%d no enough memory !\n", func_name, line);
		return NULL;
	}

	if ((np = malloc(sizeof(*np)))) {
		np->ptr = ptr;
		np->size = size;
		np->line = line;
		snprintf(np->func_name, FUNC_NAME_LEN_MAX, "%s", func_name);

		_add_node(np);
	}

	return ptr;
}

void Free(void *ptr, char *func_name, int line) {
	assert(ptr);

	if (_find_node(ptr) == NULL) {
		PRINT_ERR("[%s:%d] You free this memory more than one time !\n", func_name,
				line);
		return;
	}

	_del_node(ptr);

	printf("%s:%d free memory OK! \n", func_name, line);
	free(ptr);
}


void print_alloc_log(void){
	struct node *p;
	
	int i, nsize;

	nsize  = sizeof(htable)/sizeof(htable[0]);
	for(i = 0;i < nsize; i++){
		for(p = htable[i]; p; p = p->link){
			printf("[%s:%d], size:%ld\n",p->func_name, p->line, p->size);
		}
	}
}
