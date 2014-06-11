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


#define MAX_ADDR_LEN 256
#define BUF_LEN      128
#define MAX_SLEEP_SEC 128
static int
connect_retry(int sockfd, struct sockaddr *sap, socklen_t len)
{
	int nsec;

	if(!sap)
		return -1;

	for (nsec = 0; nsec < MAX_SLEEP_SEC; nsec << 1)
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


static void
print_uptime(int sockfd)
{
	int n;
	char buf[BUF_LEN];

	while((n = recv(sockfd, buf, BUF_LEN, 0)) > 0)
	{
		write(stderr, buf, BUF_LEN);
	}

	if(n < 0)
		printf("recv error\n");
}


int
main(int argc, char *argv[])
{
	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	int sockfd, err;

	if(argc != 2)
	{
		printf("usage: \n %s hostname\n", argv[0]);
		return -1;
	}

	hint.ai_flags = 0;
	hint.ai_family = 0;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;

	if((err = getaddrinfo(argv[1], "ruptime", &hint, &ailist)) != 0)
	{
		printf("getaddrinfo error: %s \n",gai_strerror(err));
		return -2;
	}

	for(aip = ailist; aip; aip = aip->ai_next)
	{
		if((sockfd = socket(aip->ai_family, SOCK_STREAM, 0)) < 0)
		{
			err = errno;
		}
		

		if(connect_retry(sockfd, aip->ai_addr, aip->ai_addrlen) < 0)
		{
			err = errno;	
		}
		else
		{
			print_uptime(sockfd);
			return 0;
		}
	}

	fprintf(stderr, "can't connect to %s: %s\n", argv[1], strerror(err));
	return -3;
}



