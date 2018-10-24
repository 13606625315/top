#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#define PORT 7777

int main(int ar, char** av)
{
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(atoi(av[1]));
	
	connect(sockfd, (sockaddr*)&addr, sizeof(addr));
	
	char buf[128];
	while(fgets(buf, 128, stdin))
	{
		printf("buf1 = %s\n",buf);
		int size = strlen(buf);
		int n = write(sockfd, buf ,size);
		printf("buf2 = %s\n",buf);
		n = read(sockfd,buf,128);
		buf[n] = 0;
		printf("buf3 = %s\n",buf);
	}
}