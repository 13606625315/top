#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/epoll.h>
#include<socket.h>
#include<arpa/inet.h>
#include<netinet/ip.h>
void setNonblocking(int sockfd)
{
	int opts;
    opts=fcntl(sockfd,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        return;
    }//if

    opts = opts|O_NONBLOCK;
    if(fcntl(sockfd,F_SETFL,opts)<0)
    {
 		perror("fcntl(sock,SETFL,opts)");
        return;
    }//if
}
int main()
{
	int listenfd;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	setNonblocking(listenfd);
	
	struct sock_addr_in client;	
	struct sockaddr_in addr;
	bzero(&client,sizeof(client));
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(listenfd, (struct sock_addr* )& addr, sizeof(addr)) < 0)
	{
		perror("bind");
		exit(1);
	}	
	
	if(listen(listenfd, 10) < 0)
	{
		perror("listen");
		exit(1);
	}
	
	int epfd, waitfd;
	struct epoll_event ev, events[10];
	
	epfd = epoll_create(10);
	ev.data.fd = listenfd;
	ev.events = EPOLLIN|EPOLLET;
	epoll_ctr(epfd,EPOLL_CTR_ADD, listenfd, &ev);
	
	for(;;)
	{
	  waitfd = epoll_wait(epfd, events, 10, -1);
	  
	  for(int i = 0; i < waitfd; i++)
	  {
		if(events[i] == listenfd)
		{
			int clientfd;

			while(clinetfd = accept(listen,(struct sock_addr*)&client, sizeof(client)) < 0)
			{
				
			}
			
			
		}
	  }
		
	}
}