//测试多线程对同一个fd进行epoll_wait操作

#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stack>
#include <sys/select.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <iostream>
#include <poll.h>
#include <sys/syscall.h> /*此头必须带上*/
static pid_t gettid()
{
	return syscall(SYS_gettid); /*这才是内涵*/
}
const int epollsize = 16;
	int epfd, waitfd;
	struct epoll_event ev, events[epollsize];
/*处理客户请求的线程*/
void* handleRequest1(void *fd)
{
	for(int i = 0; i < 3; i++)
	{
		waitfd = epoll_wait(epfd,events, epollsize, -1);
		
		for(int i = 0; i < waitfd; i++)
		{
			if(events[i].data.fd == STDIN_FILENO)
			{
				printf("777777 = tid:%d\n",gettid);
				
				// pthread_t pid;
				// pthread_create(&pid , NULL , (void *)handleRequest , (void *)&epfd);				
			}
		}
	}
}

void* handleRequest(void *fd)
{
	for(int i = 0; i < 3; i++)
	{			printf("2222 = tid:%d\n",gettid());
		struct pollfd pfd;
		memset(&pfd, 0, sizeof(sizeof(pfd)));
		pfd.fd = *(int*)fd;

		pfd.events = POLLIN | POLLERR;
		poll(&pfd, 1, -1);
		if(pfd.revents)
		{
			printf("77777 = tid:%d, evetn = %d\n",gettid(),pfd.revents);	
			
			// pthread_t pid;
			// pthread_create(&pid , NULL , (void *)handleRequest , (void *)&epfd);				
		}		
	}
}

int main()
{
	/*fd_set set;
	FD_ZERO(&set);
	FD_SET(STDIN_FILENO, &set);
	
	for(;;)
	{
		select(STDIN_FILENO+1,&set, 0, 0, NULL);
		
		if(1)//0 lt,1 et
		{	
			char buf[123];
			read(STDIN_FILENO,buf,sizeof(buf));
		}
		if(FD_ISSET(STDIN_FILENO,&set))
		printf("123\n");
	}*/
	

	epfd = epoll_create(epollsize);
	
	ev.data.fd = STDIN_FILENO;
	ev.events = EPOLLIN|EPOLLET;
	
	epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

	for(;;)
	{
		waitfd = epoll_wait(epfd,events, epollsize, -1);
		
		for(int i = 0; i < waitfd; i++)
		{
			if(events[i].data.fd == STDIN_FILENO)
			{
#if 0				

				
				pthread_t pid;
				pthread_create(&pid , NULL , handleRequest , (void *)&epfd);				
#else
				printf("11111 = tid:%d\n",gettid());
				handleRequest((void *)&epfd);
#endif
			}
		}
	}
	return 1;
}


