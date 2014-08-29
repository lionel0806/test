/*
 * udpsvr1.c
 *
 *  Created on: 2014年6月25日
 *      Author: lionel
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netdb.h>
#include <sys/select.h>

#define SVR_PORT 2345
#define QLEN	 10
#define MSG_LEN  512

int main(int argc, char **argv)
{
	struct sockaddr_in svr_addr, cli_addr;
	int listenfd, udpfd, connfd;
	fd_set rset;
	int nfds;
	char msg[MSG_LEN];
	int nread;
	int pid;
	socklen_t socklen;

	bzero(&svr_addr, sizeof(svr_addr));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	svr_addr.sin_port = htons(SVR_PORT);

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket error !\n");
		return -1;
	}

	bind(listenfd, (struct sockaddr *)&svr_addr, sizeof(svr_addr));
	listen(listenfd, QLEN);

	if ((udpfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("socket error !\n");
		return -1;
	}

	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	svr_addr.sin_port = htons(SVR_PORT);
	bind(udpfd, (struct sockaddr *)&svr_addr, sizeof(svr_addr));

	FD_ZERO(&rset);
	for (;;)
	{
		FD_SET(listenfd, &rset);
		FD_SET(udpfd, &rset);

		nfds = (listenfd > udpfd) ? listenfd : udpfd;
		select(nfds, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset))
		{
			printf("[%s:%d] >>> listenfd\n",__FUNCTION__,__LINE__);
			socklen = sizeof(svr_addr);
			connfd = accept(listenfd, (struct sockaddr *)&svr_addr, &socklen);
			if ((pid = fork()) == 0)
			{
				close(listenfd);
				bzero(msg, MSG_LEN);
				if ((nread = read(connfd, msg, MSG_LEN)) > 0)
				{
					msg[nread] = 0;
					write(connfd, msg, nread);
				}

				exit(0);
			}

			close(connfd);
		}

		if (FD_ISSET(udpfd, &rset))
		{
			printf("[%s:%d] >>> udpfd\n",__FUNCTION__,__LINE__);
			bzero(msg, MSG_LEN);
			if ((nread = recvfrom(udpfd, msg, MSG_LEN, 0, (struct sockaddr *)&cli_addr, &socklen)) > 0)
			{
				msg[nread] = 0;
//				socklen = sizeof(cli_addr);
				sendto(udpfd, msg, nread, 0, (struct sockaddr *)&cli_addr, socklen);
			}
		}
	}
}

