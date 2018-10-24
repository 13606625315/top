#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

const int MAX_LINE = 2048;
const int PORT = 6001;
const int BACKLOG = 10;
const int LISTENQ = 6666;
const int MAX_CONNECT = 20;

void *revc_masg(void * arg)
{
	int sockfd = *(int*)arg;
	char buf[MAX_LINE];

	while(1)
	{	memset(buf , 0 , MAX_LINE);
		int n = recv(sockfd, buf, MAX_CONNECT - 1,0);
		if(n == 0)
		{
			printf("connet out\n");
			return;
		}		
		buf[n] = 0;
		printf("buf = %s\n",buf);
	}
}

int main()
{
	int listenfd,connetfd;
	listenfd = socket(AF_INET,SOCK_STREAM , 0);
	
	struct sockaddr_in addr, clientaddr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);
	
	bind(listenfd, (struct sockaddr*)&addr, sizeof(addr));
	
	listen(listenfd, 20);
	
	socklen_t len = sizeof(clientaddr);
	connetfd = accept(listenfd, (struct sockaddr*)&clientaddr, &len);
	printf("accpet a new client: %s:%d\n", inet_ntoa(clientaddr.sin_addr) , clientaddr.sin_port);		
	pthread_t threadfd;
	pthread_create(&threadfd,NULL, revc_masg, &connetfd);
	
	char buf[MAX_LINE];
	while(fgets(buf, MAX_LINE, stdin ))
	{
		int size = strlen(buf);
		write(connetfd, buf, size);
	}
	
}