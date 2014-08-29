/*
 * echoservers.c
 *
 *  Created on: 2014年6月16日
 *      Author: lionel
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>


typedef struct{

	int maxfd;
	fd_set read_set;
	fd_set ready_set;
	int nready;
	int maxi;
	int clientfd[FD_SETSIZE];
	rio_t clientrio[FD_SETSIZE];
} pool;


int byte_cnt = 0;


static void
init_pool(int listenfd, pool *p)
{
	int i;

	p->maxi = -1;
	for(i = 0;i < FD_SETSIZE; i++)
		p->clientfd[i] = -1;

	p->maxfd = listenfd;
	FD_ZERO(&p->read_set);
	FD_SET(listenfd, &p->read_set);
}


int
main(int argc, char *argv[])
{
	int listenfd, connfd,port;
	socklen_t clientlen = sizeof(struct sockaddr_in);
	struct sockaddr_in clientaddr;
	static pool pool;

	if(argc != 2)
	{
		printf("usage: %s <port>\n",argv[0]);
		return -1;
	}

	port = atoi(argv[1]);
	listenfd = open_clientfd(port);

	init_pool(listenfd, &pool);


}

