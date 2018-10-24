#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<unistd.h>
#include <arpa/inet.h>
const int PORT = 6666;
const int LISTENQ = 30;
#define check(a) if(a<0) { printf("error = %d\n",__LINE__); exit(1);}
const int MAXSIZE = 128;
int main()
{
	struct sockaddr_in clientaddr;
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientaddr.sin_port = htons(PORT);
	
	int connetfd = socket(AF_INET, SOCK_STREAM, 0);
	
	connect(connetfd, (struct sockaddr* )&clientaddr, sizeof(clientaddr));
	
	char writebuf[MAXSIZE], readbuf[MAXSIZE+1];
	while(fgets(writebuf, MAXSIZE, stdin) > 0)
	{
		printf("1111\n");
		write(connetfd, writebuf, strlen(writebuf));
		printf("1111\n");
		int n = read(connetfd,readbuf, MAXSIZE);
		
		readbuf[n] = 0;
		
		printf("client = %s\n",readbuf);
	}
	
	
}