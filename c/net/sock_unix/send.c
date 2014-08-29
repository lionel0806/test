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

	memset(&addr, 0, sizeof(addr));

	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, SUN_PATH);

	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		printf("socket error !\n");
		return -1;
	}

	while(1)
	{
		static int counter = 0;
		char buf[128];
		int len;

		counter++;
		sprintf(buf, "The counter is %d", counter);
		len = strlen(addr.sun_path) + sizeof(addr.sun_family);

		if(sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&addr, len) < 0)
		{
			printf("sendto error !\n");
		}			 
		printf("send: %s\n", buf);
		sleep(1);
	}
	
	return 0; 
}
