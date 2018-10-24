#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/ip.h>
#include <fcntl.h>
#include <unistd.h>
#include<errno.h>
const int SIZE = 128;
#ifndef CONNECT_SIZE
#define CONNECT_SIZE 256
#endif

#define PORT 7777
#define MAX_LINE 2048
#define LISTENQ 20

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

int main(int argc , char **argv)
{
	int i, listenfd, connfd, sockfd, epfd, nfds;

	ssize_t n, ret;
		
	char buf[MAX_LINE];

	socklen_t clilen;

	struct sockaddr_in servaddr , cliaddr;

	/*声明epoll_event结构体变量，ev用于注册事件，数组用于回传要处理的事件*/
	struct epoll_event ev, events[20];

	/*(1) 得到监听描述符*/
	listenfd = socket(AF_INET , SOCK_STREAM , 0);
	setNonblocking(listenfd);

	/*生成用于处理accept的epoll专用文件描述符*/	
	epfd = epoll_create(CONNECT_SIZE);
	/*设置监听描述符*/
	ev.data.fd = listenfd;
	/*设置处理事件类型*/
	ev.events = EPOLLIN | EPOLLET;
	/*注册事件*/
	epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);		

	/*(2) 绑定套接字*/	
	bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(9999);

	bind(listenfd , (struct sockaddr *)&servaddr , sizeof(servaddr));

	/*(3) 监听*/
	listen(listenfd , LISTENQ);

	/*(4) 进入服务器接收请求死循环*/
	while(1)
	{
		/*等待事件发生*/
		printf("line = %d\n",__LINE__);		
		nfds = epoll_wait(epfd , events , CONNECT_SIZE , -1);
		if(nfds <= 0)
			continue;
	
		printf("nfds = %d\n" , nfds);
		/*处理发生的事件*/
		for(i=0 ; i<nfds ; ++i)
		{
			/*检测到用户链接*/
			if(events[i].data.fd == listenfd)
			{	
				/*接收客户端的请求*/
				clilen = sizeof(cliaddr);

				while((connfd = accept(listenfd,(struct sockaddr*)&cliaddr, &clilen)) > 0)
				{


					printf("accpet a new client: %s:%d\n", inet_ntoa(cliaddr.sin_addr) , cliaddr.sin_port);		
					setNonblocking(connfd);
					
					ev.data.fd = connfd;
					ev.events = EPOLLIN|EPOLLET;
					epoll_ctl(epfd,EPOLL_CTL_ADD, connfd, &ev);
				}
				continue;
			}
			printf("line = %d\n",__LINE__);
			if(events[i].events&EPOLLIN)
			{
				sockfd = events[i].data.fd;
				int n = 0;
				int nread = 0;
				bzero(buf,sizeof(buf));
				while((nread = read(sockfd, buf + n, SIZE)) > 0 );
				{
					n = n + nread;
				}
				if(nread == -1 && errno != EAGAIN)
				{
					perror("read ");
				}
				printf("read = %s\n",buf);
				ev.data.fd = sockfd;
				ev.events = EPOLLOUT | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
			}
			if(events[i].events&EPOLLOUT)
			{
				sockfd = events[i].data.fd	;
				int n = strlen(buf);
				int size = strlen(buf);
				int nwrite = 0;
				while(n > 0)
				{
					nwrite = write(sockfd, buf + size - n , n);
					if(nwrite < n)
					{
						if(nwrite == -1&&errno != EAGAIN)
						{
							perror("write");
						}
						if(errno == EAGAIN)
						{
							sleep(1);
							continue;
						}
						break;
					}
					n = n- nwrite;
				}
				printf("write = %s\n",buf);
				ev.data.fd = sockfd;
				ev.events = EPOLLIN|EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
			}
		}
		
	}
	free(events);
	close(epfd);
	exit(0);
}