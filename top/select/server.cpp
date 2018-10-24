#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/ip.h>
//#include<arpa>
#include<fstream>

#include <unistd.h>

const int PORT = 7777;
const int SIZE = 64;
const int LEN = 128;
int main()
{
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT);
	int serfd = socket(AF_INET, SOCK_STREAM, 0);
	
	bind(serfd, (struct sockaddr *)& serveraddr, sizeof(serveraddr));
	
	listen(serfd, 100);
	
	fd_set readset;
	FD_ZERO(&readset);
	FD_SET(serfd, &readset);
	
	int connetfd[SIZE], acceptfd;
	int i,size = 0;
	for( i = 0;i < SIZE;i++)
	{
		connetfd[i] = -1;
	}
	int ready = 0;
	int max = serfd + 1;
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);
	bzero(&clientaddr,sizeof(clientaddr));
	for(;;)
	{
		printf("max = %d\n",max);
		fd_set settmp = readset;
		ready = select(max, &settmp, 0 ,0,NULL);
		
		printf("ready = %d\n",ready);
		if(FD_ISSET(serfd, &settmp))
		{
			printf("ready 11= %d\n",ready);
			acceptfd = accept(serfd, (struct sockaddr*)&clientaddr, &len);
			if(acceptfd < 0)
			{
				perror("accept error.\n");
				exit(1);
			}//if	
			for( i = 0; i < SIZE; i++)
			{
				if(connetfd[i] == -1)
				{
					connetfd[i] = acceptfd;
					break;
				}
			}
			
			FD_SET(acceptfd, &readset);
			
			max = max>acceptfd?max:acceptfd + 1;
			size = size>i?size:i;
			printf(" 111 %d %d \n",max , size);
			if(--ready < 0)
			{
				continue;
			}
		}
		
		for(i = 0;i<=size;i++)
		{
			if(connetfd[i] == -1)
			{
				continue;
			}
			printf("123123 = %d\n",connetfd[i]);
			if(FD_ISSET(connetfd[i],&settmp))
			{
				int n = 0;
				char buf[LEN];
				n = read(connetfd[i], buf, LEN);
				if(n<=0)
				{
					printf(" 1111 = %d \n",n);
					close(connetfd[i]);
					FD_CLR(connetfd[i] , &readset);
					connetfd[i] = -1;
				}//if
				else
				{
					printf(" 222 %d %d \n",2 , 2);
					write(connetfd[i], buf, n);
				}
			}
			if(--ready <= 0)
			{
				break;
			}
		}
	}
}