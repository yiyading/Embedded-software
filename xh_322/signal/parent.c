#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<wait.h>

int main()
{
	pid_t pid;
	pid = fork();
	char* args[] = {"child", NULL};
	int ret;

	if(pid<0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid == 0)
	{
		printf("This is the child process:\n");
		printf("pid= %d, ppid= %d\n", getpid(), getppid());
		execv("./child", args);
	}
	else if(pid > 0){
		sleep(5);
		printf("This is the parent process:\n");
		printf("pid= %d, ppid= %d\n", getpid(), getppid());
		if ( (ret=kill(pid, SIGILL)) == 0 )
			printf("parent: sent SIGILL to child\n");
		sleep(1);
		if ( (ret=kill(pid, SIGALRM)) == 0 )
			printf("parent: sent SIGALRM to child\n");
		sleep(1);
		if ( (ret=kill(pid, SIGRTMIN)) == 0 )
                        printf("parent: sent SIGRTMIN to child\n");
                sleep(1);
		if( (ret=kill(pid, SIGKILL)) == 0 )
			printf("parent: sent SIGKILL to child\n");
//		waitpid(pid, NULL, 0);
		exit(EXIT_SUCCESS);
	}


}
