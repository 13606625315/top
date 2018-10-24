#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stack>
#include <sys/select.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <iostream>

const int epollsize = 16;

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
	
	int epfd, waitfd;
	struct epoll_event ev, events[epollsize];
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
				printf("1231231\n");
			}
		}
	}
	return 1;
}


