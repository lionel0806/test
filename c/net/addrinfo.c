#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

static void
print_family(struct addrinfo *aip)
{
	if(!aip)
		return;
	printf(" family ");
	switch(aip->ai_family)
	{
		case AF_INET:
			printf("inet");
			break;
		case AF_INET6:
			printf("inet6");
			break;
		case AF_UNIX:
			printf("unix");
			break;
		case AF_UNSPEC:
			printf("unspecified");
			break;
		default:
			printf("unknown");
	}	
}


static void
print_type(struct addrinfo *aip)
{
	if(!aip)
		return;

	printf(" type ");

	switch(aip->ai_socktype)
	{
		case SOCK_STREAM:
			printf("stream");
			break;
		case SOCK_DGRAM:
			printf("datagram");
			break;
		case SOCK_SEQPACKET:
			printf("seqpacket");
			break;
		case SOCK_RAW:
			printf("raw");
			break;
		default:
			printf("unknown (type:%d)", aip->ai_socktype);
	}
}


static void 
print_protocol(struct addrinfo *aip)
{
	if(!aip)
		return;

	printf("protocol");

	switch(aip->ai_protocol)
	{
		case 0:
			printf("default");
			break;
		case IPPROTO_TCP:
			printf("tcp");
			break;
		case IPPROTO_UDP:
			printf("udp");
			break;
		case IPPROTO_RAW:
			printf("raw");
			break;
		default:
			printf("unknown (protocol:%d)", aip->ai_protocol);
	}
}


static void
print_flags(struct addrinfo *aip)
{
	if(!aip)
		return;
	printf(" flags ");
	if(aip->ai_flags == 0)
	{
		printf(" 0 ");
	}
	else
	{
		if(aip->ai_flags & AI_PASSIVE)
			printf(" passive ");
		else if(aip->ai_flags & AI_CANONNAME)
			printf("canonname");
#if defined(AI_NUMERICSERV)
		else if(aip->ai_flags & AI_NUMERICSERV)
			printf("numericserv");
#endif
#if defined(AI_V4MAPPED)
		else if(aip->ai_flags & AI_V4MAPPED)
			printf("v4mapped");
#endif
#if defined(AI_ALL)
		else if (aip->ai_flags & AI_ALL)
			printf("all");
#endif
	}
}



int
main(int argc, char *argv[])
{
	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	struct sockaddr_in *sinp;
	const char *addr;
	int err;
	char abuf[INET_ADDRSTRLEN];

	if(argc != 3)
	{
		printf("usage: %s nodename service\n", argv[0]);
		return 1;
	}


	hint.ai_flags = AI_CANONNAME;
	hint.ai_family = 0;
	hint.ai_socktype = 0;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if((err = getaddrinfo(argv[1], argv[2], &hint, &ailist)) != 0)
	{
		printf("getaddrinfo error :%s", gai_strerror(err));
		return 1;
	}	

	for(aip = ailist; aip; aip = aip->ai_next)
	{
		print_flags(aip);
		print_family(aip);
		print_type(aip);
		print_protocol(aip);
		printf("\n\thost %s", aip->ai_canonname?aip->ai_canonname:"--");

		if(aip->ai_family == AF_INET)		
		{
			sinp = (struct sockaddr_in *)aip->ai_addr;
			addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);
			printf(" address %s", addr?addr:"unknown");
			printf(" port %d", ntohs(sinp->sin_port));
		}

		printf("\n");
	}
	
	return 0; 
}
