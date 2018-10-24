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

void* recv_msg(void* arg)
{
	int sock = *(int*)arg;
	char buf[MAX_LINE];
	int n;
	while(1)
	{
		memset(buf , 0 , MAX_LINE);	
		n = read(sock, buf, MAX_LINE);
		if(n == 0)
		{
			printf("connet out\n");
			return;
		}
		printf("recv = %s, n =%d\n",buf,n);
	}
}

int main()
{
	int sock;
	sock = socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	seraddr.sin_port = htons(PORT);
	
	connect(sock, (struct sockaddr*)&seraddr, sizeof(seraddr));
	
	pthread_t threadfd;
	pthread_create(&threadfd, NULL, recv_msg, &sock );
	
	char buf[MAX_LINE];
	while(fgets(buf, MAX_LINE, stdin))
	{
		int size = strlen(buf);
		write(sock,buf, size);
		//printf("w");
	}
}