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

#define BUF_LEN 128
#define QLEN    10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif


static int
initserver(int type, const struct sockaddr_in *addr, socklen_t alen, int qlen)
{
	int fd;
	int err = 0;

	if((fd = socket(AF_INET, type, 0)) < 0)
	{
		return -1;
	}

	if(bind(fd, (struct sockaddr *)addr, alen) < 0)
	{
		err = errno;
		goto errout;
	}

	if(type == SOCK_STREAM || type == SOCK_SEQPACKET)
	{
		if(listen(fd, qlen) < 0)
		{
			err = errno;
			goto errout;
		}
	}

	return fd;

errout:
	close(fd);
	errno = err;
	return -1;
}


void
server(int sockfd)
{
	int clfd, status;
	FILE *fp;
//	pid_t pid;
	char buf[BUF_LEN];
	struct sockaddr_in cli_addr;
	int len;
	char addr_buf[128];

	for(;;)
	{
		clfd = accept(sockfd, (struct sockaddr *)&cli_addr, &len);
		if(clfd < 0)
		{
			syslog(LOG_ERR, "ruptimed: accept error: %s", strerror(errno));
			exit(1);
		}

		printf("connected client addr: %s, port:%d\n",
			 inet_ntop(AF_INET, (void *)&cli_addr.sin_addr, addr_buf, len), ntohs(cli_addr.sin_port));

		if((fp = popen("/usr/bin/uptime", "r")) == NULL)
		{
			sprintf(buf, "error:%s\n",strerror(errno));
			send(clfd, buf, strlen(buf), 0);
		}
		else
		{
			while(fgets(buf, BUF_LEN,fp) != NULL)
				send(clfd, buf,strlen(buf), 0);
			pclose(fp);
		}

		close(clfd);
	}
}


static void
daemonize(const char * cmd)
{
	int i, fd0, fd1, fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;

	/*
	 *clear file creation mask;
	 */

	umask(0);

	/*
	 * get maxmum number of file descriptors;
	 */

	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
	{
		printf("%s can't get file limit", cmd);
		exit(1);
	}

	/*
	 *become a session leader to lose controlling TTY;
	 */

	if ((pid = fork()) < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if (pid != 0)
		exit(0);

	//printf("child1\n");
	setsid();

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
	{
		perror("sigaction error");
		exit(1);
	}

	if ((pid = fork()) < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if (pid != 0)
		exit(0);
	//printf("child2");

	if (chdir("/") < 0)
		perror("can't change directory to /");

	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < rl.rlim_max; i++)
		close(i);

	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	/*
	 * initialize the log file
	 */
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2)
	{
		syslog(LOG_ERR, "unexpected descriptors %d %d %d", fd0, fd1, fd2);
		exit(1);
	}
}

int
main(int argc, char *argv[])
{
//	struct addrinfo *ailist, *aip;
//	struct addrinfo hint;
	struct sockaddr_in serv_addr;
	int sockfd, err, n;
	char *host;

	if(argc != 1)
	{
		printf("usage: ruptimed\n");
		return 1;
	}

#ifdef _SC_HOST_NAME_MAX
	n = sysconf(_SC_HOST_NAME_MAX);
	if(n < 0)
#endif
		n = HOST_NAME_MAX;
	host = (char *)malloc(n);
	if(host == NULL)
	{
		printf("malloc error\n");
		return -1;
	}

	if (gethostname(host, n) <0)
	{
		printf("gethostname error\n");
		return -1;
	}

	printf("host:%s\n",host);
	//daemonize("ruptimed");

//	hint.ai_flags = AI_CANONNAME;
//	hint.ai_family = AF_INET;
//	hint.ai_socktype = SOCK_STREAM;
//	hint.ai_protocol = 0;
//	hint.ai_addrlen = 0;
//	hint.ai_canonname = NULL;
//	hint.ai_addr = NULL;
//	hint.ai_next = NULL;

//	if((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0)
//	{
//		syslog(LOG_ERR, "ruptimed: getaddrinfo error: %s", gai_strerror(errno));
//		exit(1);
//	}

//	printf()

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(1234);

	if ((sockfd = initserver(SOCK_STREAM, &serv_addr, sizeof(serv_addr), QLEN)) >= 0)
	{
		server(sockfd);
		exit(0);
	}

//	for(aip = ailist; aip; aip = aip->ai_next)
//	{
//		if((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0)
//		{
//			server(sockfd);
//			exit(0);
//		}
//		else
//		{
//			printf("initserver error\n");
//			exit(1);
//		}
//	}

	exit(1);
}


