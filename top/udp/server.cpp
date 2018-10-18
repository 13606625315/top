#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include<string.h>
#include<netinet/ip.h>

const int port = 6000;
const int masgLen = 128;

int main()
{
	int sockfd;
	struct sockaddr_in server, client;
	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket failed! \n");
		exit(1);
	}
	
	if(bind(sockfd, (struct sockaddr *)&server, sizeof(sockaddr)))
	{
		perror("bind failed!\n");
		exit(1);
	}
	
	int n = 0;
	char masg[masgLen];
	socklen_t len = sizeof(sockaddr);
	for(;;)
	{
		printf(" start server \n");
		if((n = recvfrom(sockfd, masg, masgLen, 0 ,(struct sockaddr*)&client, &len)) < 0)
		{
			perror("recv failed!\n");
			exit(1);
		}
		printf(" start server \n");
		printf("server = %s", masg);
		
		if((n = sendto(sockfd, masg ,n, 0,(struct sockaddr*)&client, sizeof(sockaddr))) < 0)
		{
			perror("send failed\n");
			exit(1);
		}
		
		
	}
}