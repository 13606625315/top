#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<unistd.h>
const int PORT = 7777;
const int LISTENQ = 30;
#define check(a) if(a<0) { printf("error = %d\n",__LINE__); exit(1);}
const int MAXSIZE = 128;

int main()
{
	struct sockaddr_in serveraddr;
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	check(sockfd);
	
	int ret = bind(sockfd,(struct sockaddr*)&serveraddr, sizeof(serveraddr));
	check(ret);
	
	ret = listen(sockfd, LISTENQ);
	check(ret);
	
	struct sockaddr_in clientaddr;
	for(;;)
	{
		socklen_t clientlen = sizeof(clientaddr);
		int connetfd = accept(sockfd, (struct sockaddr*)&clientaddr, &clientlen);
		printf("accept\n");	
		check(connetfd);
		if(fork() == 0)
		{
			close(sockfd);
			ssize_t n = 0;
			char buf[MAXSIZE];
			while((n = read(connetfd, buf, MAXSIZE)) > 0)
			{
				printf("read = %s\n",buf);
				write(connetfd, buf, n);
			}
			exit(0);
		}
		close(connetfd);
	}
	
}