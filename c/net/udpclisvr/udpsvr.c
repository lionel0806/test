/*
 * udpsvr.c
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

#define MAX_MSG_LEN 1024
#define SVR_PORT 2345

void dg_echo(int fd, struct sockaddr *cliaddr, socklen_t clilen)
{
	int n;
	socklen_t len;
	char msg[MAX_MSG_LEN];

	len = clilen;
	for( ; ; )
	{
		bzero(msg, MAX_MSG_LEN);
		if((n = recvfrom(fd, msg, MAX_MSG_LEN, 0, cliaddr, &len)) > 0)
		{
			//printf("[%s:%d] >>> recv msg:%s\n",__FUNCTION__,__LINE__,msg);
			sendto(fd, msg, n, 0, cliaddr, len);
		}
	}
}


int main(int argc, char *argv[])
{
	struct sockaddr_in svr_addr, cli_addr;
	int sockfd;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("connect error !\n");
		return -1;
	}

	bzero(&svr_addr,sizeof(svr_addr));
	bzero(&cli_addr,sizeof(cli_addr));

	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	svr_addr.sin_port = htons(SVR_PORT);

	bind(sockfd, (struct sockaddr *)&svr_addr, sizeof(svr_addr));
	dg_echo(sockfd, (struct sockaddr *)&cli_addr, sizeof(cli_addr));

	return 0;
}
