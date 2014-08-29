#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <unistd.h>
#include "common.h"


#define MAX_ADDR_LEN 256
#define BUF_LEN      128
#define MAX_SLEEP_SEC 128



static int
connect_retry(int sockfd, struct sockaddr *sap, socklen_t len)
{
	int nsec;

	if(!sap)
		return -1;

	for (nsec = 1; nsec < MAX_SLEEP_SEC; nsec <<= 1)
	{
		if(connect(sockfd, sap, len) == 0)
		{
			return 0;
		}

		if(nsec <= MAX_SLEEP_SEC/2)
		{
			sleep(nsec);
		}
	}

	return -1;
}


void str_cli(int sockfd)
{
	char buf[BUF_LEN + 1];
	int n;
	fd_set rset;
	int maxfdp1;

	printf("[%s:%d] >>> \n",__FUNCTION__,__LINE__);

	FD_ZERO(&rset);
	for(;;)
	{
		FD_SET(fileno(stdin), &rset);
		FD_SET(sockfd, &rset);

		maxfdp1 = (fileno(stdin) > sockfd)? fileno(stdin) : sockfd + 1;
		select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset))
		{
			if ((n = read(sockfd, buf, BUF_LEN)) > 0)
			{
				buf[n] = 0;
				printf(">>> %s\n", buf);
			}
		}

		if (FD_ISSET(fileno(stdin), &rset))
		{
			if (fgets(buf, BUF_LEN, stdin))
			{
				printf(">>> %s\n", buf);
				writen(sockfd, buf, strlen(buf));
			}
		}
	}
}


int
main(int argc, char *argv[])
{
	struct sockaddr_in serv_addr;
	int sockfd, err;

	if(argc != 2)
	{
		printf("usage: \n %s ipaddr\n", argv[0]);
		return -1;
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket error !\n");
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SVR_PORT);
	inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

	printf("[%s:%d] >>> \n",__FUNCTION__,__LINE__);
	if (connect/*_retry*/(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0)
	{
		printf("connect error ! %d\n", errno);
		return -3;
	}
	printf("[%s:%d] >>> \n",__FUNCTION__,__LINE__);


//	write(sockfd, "hello", 5);
	str_cli(sockfd);

//	fprintf(stderr, "can't connect to %s: %s\n", argv[1], strerror(err));
	return 0;
}



