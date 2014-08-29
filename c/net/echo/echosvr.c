/*
 * echosvr.c
 *
 *  Created on: 2014年6月22日
 *      Author: lionel
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <syslog.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"

#define MAX_LINE_LEN 512
#define QLEN    10


//int read_a_line(int fd, char *buf)
//{
//	char *ptr;
//	int n, count = 0;
//
//	ptr = buf;
//	while ((n = read(fd, ptr, MAX_LINE_LEN)) > 0)
//	{
//		ptr += n;
//		count += n;
//
//		if(count >= MAX_LINE_LEN)
//	}
//
//	buf[n] = 0;
//
//	if(n < 0)
//	{
//		printf("read error! \n");
//		return 0;
//	}
//
//	return count;
//}


void str_echo(int fd)
{
	int n;
	char buf[MAX_LINE_LEN + 1];

	printf("[%s:%d] >>> \n",__FUNCTION__,__LINE__);
again:
	while((n = read(fd, buf, MAX_LINE_LEN)) > 0)
	{
		printf("svr read buf: %s\n", buf);
		writen(fd, buf, strlen(buf));
	}

	if(n < 0 && errno == EINTR)
	{
		goto again;
	}
	else if(n < 0)
		printf("read error !\n");
}


int main(int argc, char *argv[])
{
	struct sockaddr_in svraddr;
	struct sockaddr_in cliaddr;
	int sockfd, connfd;
	pid_t pid;
	int status;
	socklen_t socklen;


	if(argc != 1)
	{
		printf("usage: %s \n", argv[0]);
		return -1;
	}


	bzero(&svraddr, sizeof(svraddr));
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(SVR_PORT);
	svraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket error !\n");
		return -2;
	}

	if(bind(sockfd, (struct sockaddr *)&svraddr, sizeof(svraddr)) != 0)
	{
		printf("bind error !\n");
		return -3;
	}

	if (listen(sockfd, QLEN) < 0)
	{
		printf("listen error !\n");
		return -4;
	}

	printf("[%s:%d] >>> \n",__FUNCTION__,__LINE__);
	for (;;)
	{
		socklen = sizeof(cliaddr);
		if ((connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &socklen)) >= 0)
		{
			printf("[%s:%d] >>> \n",__FUNCTION__,__LINE__);
#if 1
			pid = fork();
			if (pid == 0)
			{
				close(sockfd);
				str_echo(connfd);
				exit(0);
			}

			close(connfd);
#endif
		}
	}

//	wait(&status);
	exit(0);
}
