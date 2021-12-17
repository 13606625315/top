#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<arpa/inet.h>

#include <net/if.h>
#include <linux/sockios.h>
const int port = 53;
const int buflen = 128;

#define INTERFAXENAME "wlan0"

int main()
{
	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(port);
	client.sin_addr.s_addr = inet_addr("8.8.8.8");
	
	int sockfd;
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
		perror("sock failed\n");
		exit(1);
	}
	
#if 0	
    struct ifreq interface;
    strncpy(interface.ifr_ifrn.ifrn_name, INTERFAXENAME, sizeof(INTERFAXENAME));
    if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&interface, sizeof(interface))  < 0) {
           perror("SO_BINDTODEVICE failed");
    }
#endif
	
	char sendbuf[buflen], recvbuf[buflen + 1];
	while(fgets(sendbuf, buflen, stdin))
	{
		printf("sned = %s\n",sendbuf);
		int len = sendto(sockfd, sendbuf, strlen(sendbuf),0,(struct sockaddr*)&client, sizeof(sockaddr));
		if(len < 0)
		{
			printf("send failed len = %d\n",len);

		}
		else
		{
			printf("send success len = %d\n",len);
		}
	}

	return 0;
}