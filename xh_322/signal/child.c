#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void handler(int signum)
{
	if(signum == SIGALRM ) printf("child: SIGALRM is captured\n");
	if(signum == SIGILL ) printf("child: SIGAILL is captured\n");
	if(signum == SIGTTOU ) printf("child: SIGTTOU is captured\n");
	if(signum == SIGRTMIN ) printf("child: SIGRTMIN is captured\n");
}
int main()
{
	int flag;
	signal(SIGALRM, handler);
	signal(SIGILL, handler);
	signal(SIGTTOU, handler);
	signal(SIGRTMIN, handler);
	//printf("child process is running");
	alarm(3);
	while(1)
	{
		pause();
	}

}
