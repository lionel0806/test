#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>



#define SUN_PATH "/home/lionel/unixdomain"


int
main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_un addr;
	int len;

	bzero(&addr, sizeof(addr));
	unlink(SUN_PATH);
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, SUN_PATH);

	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		printf("socket() error !");
		return -1;
	}
	
	len = strlen(addr.sun_path) + sizeof(addr.sun_family);
	if(bind(sockfd, (struct sockaddr *)&addr, len) < 0)
	{
		printf("bind( error !\n)");
		return -1;
	}

	for(;;)
	{
		char recv_buf[128];
		bzero(recv_buf, sizeof(recv_buf));
		if(recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr, &len) > 0)
		{
			printf("recv: %s \n", recv_buf);
		}
	}

	return 0;
}
