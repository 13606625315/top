#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include <unistd.h>
const int PORT = 7777;
const int LEN = 128;
int main()
{
	int sockfd;
    if((sockfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
    {
        perror("socket error");
        exit(1);
    }//if
	struct sockaddr_in addr;
	bzero(&addr , sizeof(addr));
    bzero(&addr , sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
	    if(inet_pton(AF_INET , "127.0.0.1" , &addr.sin_addr) < 0)
    {
        printf("inet_pton error for \n");
        exit(1);
    }//if
    if(connect(sockfd , (struct sockaddr *)&addr , sizeof(addr)) < 0)
    {
        perror("connect error");
        exit(1);
    }//if
	for(;;)
	{
		int n = 0;
		char buf[LEN];
		if(fgets(buf,LEN,stdin))
		{
			printf("buf = %s\n",buf);
			write(sockfd, buf, strlen(buf));

			int n = read(sockfd,buf, LEN);
			
			buf[n] = 0;
			
			printf("client = %s\n",buf);
			
		}
	}
	
	return 0;
}