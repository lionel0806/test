/*
 * udpcli.c
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
#include <arpa/inet.h>

#define MAX_MSG_LEN 1024
#define SVR_PORT 2345

void dg_cli(int fd, struct sockaddr *svraddr, socklen_t svrlen)
{
	int n;
	char sendmsg[MAX_MSG_LEN], recvmsg[MAX_MSG_LEN + 1];
	struct sockaddr_in svr_addr;
	socklen_t len;
	char addr[32];

	for( ; ; )
	{
		bzero(sendmsg, MAX_MSG_LEN);
		if(fgets(sendmsg, MAX_MSG_LEN, (FILE *)stdin))
		{
			//printf("[%s:%d] >>> %s\n",__FUNCTION__,__LINE__, sendmsg);
			sendto(fd, sendmsg, strlen(sendmsg), 0, svraddr, svrlen);
			n = recvfrom(fd, recvmsg, MAX_MSG_LEN, 0, (struct sockaddr *)&svr_addr, &len);
			recvmsg[n] = 0;
			fputs(recvmsg, stdout);

			sprintf(recvmsg, "recv from %s:%d\n",inet_ntop(AF_INET, &svr_addr.sin_addr, addr, len), ntohs(svr_addr.sin_port));
			fputs(recvmsg, stdout);
		}
	}
}


int main(int argc, char *argv[])
{
	struct sockaddr_in svr_addr;
	int sockfd;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("connect error !\n");
		return -1;
	}

	bzero(&svr_addr,sizeof(svr_addr));

	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port = htons(SVR_PORT);
	inet_pton(AF_INET, argv[1], &svr_addr.sin_addr);

	dg_cli(sockfd, (struct sockaddr *)&svr_addr, sizeof(svr_addr));
	return 0;
}
