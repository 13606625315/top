#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
const int port = 6000;
const int buflen = 128;


int main()
{
	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(port);
	
	
	if((inet_pton(AF_INET,"127.0.0.1", &client.sin_addr)) <= 0)
	{
		perror("error\n");
	}
	client.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	int sockfd;
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
		perror("sock failed\n");
		exit(1);
	}
	
	char sendbuf[buflen], recvbuf[buflen + 1];
	while(fgets(sendbuf, buflen, stdin))
	{
		printf("sned = %s\n",sendbuf);
		if((sendto(sockfd, sendbuf, strlen(sendbuf),0,(struct sockaddr*)&client, sizeof(sockaddr))) < 0)
		{
			perror("send failed\n");
			exit(1);
		}
		
		int n = 0;
		socklen_t len = sizeof(sockaddr);
		if((n = recvfrom(sockfd, recvbuf, buflen, 0,(struct sockaddr*)&client, &len))< 0)
		{
			perror("recv failed\n");
			exit(1);
		}

		recvbuf[n] = 0;
		
		printf("client2 = %s,n=%d\n", recvbuf,n);
		
	}

	return 0;
}