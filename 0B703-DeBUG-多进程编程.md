# 0B703-DeBUG-多进程编程.md
## 一、实验目的
1、学会创建多进程程序，理解fork()，vfork() 创建的子进程的特点。<br>
2、学会使用有名管道和无名管道实现进程间通信。<br>
3、学会使用信号机制实现进程间通信。<br>
## 二、实验内容
1. 编写fork()，vfork()多进程程序，通过输出结果比较所创建的子进程的不同之处。<br>
2. 编写fork()多进程程序，进程间模拟信号通信，包括信号发送、捕捉与处理。<br>
3. 编写fork()多进程程序，进程间实现有名管道和无名管道通信。具体要求如下：<br>
编写1个服务器程序server.c和2个客户端程序client1.c,client2.c ，服务器和客户端之间采用FIFO管道通信；<br>
服务器和客户端各自创建自己的子进程，父进程负责完成FIFO管道通信，子进程负责输出接收到的信息，父子进程间采用无名管道通信。<br>
## 三、实验过程与结果
### 1、fork()、vfork()实验
编写如下所示的程序

```c
#include "./barrel.h"
#include "./char2int.h"
#include<stdio.h>
#include<wait.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
	int if_input = 1;
	int i, k, len;
	int a[] = {2,0,6,7,4,6,4,11,56,89,100,1,1};
	char scan_num[100];
	int c[50]={0};

	//scanf("%[^/n]", scan_num);
	if( if_input ==1 )		
	{
		fgets(scan_num, 100, stdin);
        	k= char2int(scan_num, c);
		len = k;
	}
	else	
		len = sizeof(a)/sizeof(int);
	int b[ len ];
	int status, ascending = 0;
	pid_t pid, cpid;
	if((pid = fork()) == -1){
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(if_input == 1)
		if(pid == 0) //child process :descending sort
		{
			printf("This is the child process\n");
			printf("pid= %d, ppid= %d\n",getpid(), getppid());
			barrel(c, len,b, ascending);
			ascending = 1;
		}
		else
		{
			printf("This is the parent process\n");
			printf("pid= %d, ppid= %d\n",getpid(), getppid());
			barrel(c, len,b, ascending);
			ascending = 1;
		}
	else 
		barrel(a, len,  b, 1)                      ;
	for(i =0; i< (sizeof(b)/sizeof(int)); i ++)
	{
		printf("%d, ", b[i]);
		if(i == (sizeof(b)/sizeof(int))-1)
			printf("\n");
	}
	if(pid != 0)
	{
		cpid = wait(&status);
		printf("The child process has finishied, cpid= %d\n",cpid);
	}
	exit(EXIT_SUCCESS);
	return 0;
}
```
程序中使用到了一个函数barrel()，是用于排序的函数，其第三个参数表示是降序排序还是升序排序，ascending为1时表示升序排序，ascending为0时表示降序排序。程序运行开始时，ascending=0，为降序排序，如果子进程和父进程共享内存，则在一个进程运行完成后会将ascending置1，程序运行结果是一个进程升序排序，一个进程降序排序。如果不共享内存，则两个进程都为降序排序。<br>
下图为使用fork()函数创建子进程的程序运行结果：<br>
![使用fork()创建子进程实验结果](https://github.com/yiyading/Embedded-software/blob/master/xh_322/img/fork().png)<br>
下图为使用vfork()函数创建子进程的程序运行结果：<br>
![使用vfork()创建子进程实验结果](https://github.com/yiyading/Embedded-software/blob/master/xh_322/img/vfork().png)<br>
可以看到，结果为一个进程升序排序，一个进程降序排序。<br>
### 2、signal实验
编写如下程序：<br>
①父进程程序如下，父进程用kill()向子进程发送信号<br>
```c
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
	else{
		sleep(1);
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
		waitpid(pid, NULL, 0);
		exit(EXIT_SUCCESS);
	}


}
```
①子进程程序如下，子进程用pause()捕获信号信号并处理<br>
```c
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
```
下图为的程序运行结果：<br>
![signal实验结果](https://github.com/yiyading/Embedded-software/blob/master/xh_322/img/signal.png)<br>
可以看到，子进程正确接受到了父进程发出的信号并用handler函数进行了处理，打印出了接收到的信号，并在最后由于接受到了父进程发出的SIGKILL信号而退出。<br>

