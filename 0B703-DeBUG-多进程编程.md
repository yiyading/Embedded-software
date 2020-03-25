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
 ### 3、pipe/fifo实验
编写如下程序：<br>
①服务器端程序：<br>
服务器程序从客户端A、客户端B的FIFO接收消息，然后先通过pipe发送给子进程打印出来，然后将从客户端A接收到的消息发送给客户端B，将从客户端B接收到的消息发送给客户端A。<br>
```c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<memory.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>
#define BUFFLEN 1000

int main()
{
	pid_t pid;
	int pfd[2];
	char buff1[BUFFLEN];
	char buff2[BUFFLEN];
	//char message[]="communication is successful!";
	int rdnum, wtnum, fd_wca, fd_wcb, fd_rca, fd_rcb;
	int offst1, offst2=0;
	int i=0;
	int j = 2;
	int res=0;
	offst1 = 0;
	memset(buff1, 0 ,sizeof(buff1));
	memset(buff2, 0 ,sizeof(buff2));
	if(pipe(pfd)<0)
	{
		perror("pipe error");
		exit(1);
	}
	pid = fork();
	if(pid<0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid == 0)
	{
		close(pfd[1]);
		while(1)
		{
			usleep(50*1000);
			rdnum = read(pfd[0], buff1, 100);
			printf("Server Child: received %d bytes from the Server Parent: %s \n", rdnum, buff1);
		}
		exit(0);
	}
	else
	{
		if (access("./WT_CLNTA", F_OK) == -1)
		{
			res = mkfifo("./WT_CLNTA", 0777);
			if (res != 0)
			{
				fprintf(stderr, "Could not create fifo:w2a\n");
				exit(EXIT_FAILURE);
			}
		}
		if (access("./RDF_CLNTA", F_OK) == -1)
                {
                        res = mkfifo("./RDF_CLNTA", 0777);
                        if (res != 0)
                        {
                                fprintf(stderr, "Could not create fifo r2a\n");
                                exit(EXIT_FAILURE);
                        }
                }

		if (access("./WT_CLNTB", F_OK) == -1)
                {
                        res = mkfifo("./WT_CLNTB", 0777);
                        if (res != 0)
                        {
                                fprintf(stderr, "Could not create fifo w2b\n");
                                exit(EXIT_FAILURE);
                        }
                }

		if (access("./RDF_CLNTB", F_OK) == -1)
                {
                        res = mkfifo("./RDF_CLNTB", 0777);
                        if (res != 0)
                        {
                                fprintf(stderr, "Could not create fifo r2b\n");
                                exit(EXIT_FAILURE);
                        }
                }

		close(pfd[0]);
		fd_wca = open("./WT_CLNTA", O_WRONLY);
		fd_wcb = open("./WT_CLNTB", O_WRONLY);
		fd_rca = open("./RDF_CLNTA", O_RDONLY);
		fd_rcb = open("./RDF_CLNTB", O_RDONLY);
		printf("Server: fd_wca= %d, fd_rca= %d \n", fd_wca, fd_rca);
		while(j>0)
		{
			if( (rdnum = read( fd_rca, buff1 + offst1, BUFFLEN)) > 0 )
			{
				printf("Server: received %d bytes from ClientA\n", rdnum);
				write(pfd[1], buff1 + offst1 , rdnum);
				offst1 += rdnum;
			}
			if( (rdnum = read( fd_rcb, buff2 + offst2, BUFFLEN)) > 0 )
			{
				printf("Server: received %d bytes from ClientB\n", rdnum);
				write(pfd[1], buff2 + offst2, rdnum);
				offst2 += rdnum;
			}
			if(  i== 1)
			{
				//printf("server has run 2 times\n");
				//printf("offst1 2 = %d\n", offst1);
				i = 0;
				if( offst1 != 0)
				{
					if( (wtnum = write(fd_wca, buff1, offst1)) != 0 )
						printf("Server: write %d bytes to ClientA\n", wtnum);
					memset(buff1, 0 ,sizeof(buff1));
					offst1  = 0; 

				}
				if( offst2 != 0 )
				{
					write(fd_wcb, buff2, offst2);
					memset(buff2, 0 ,sizeof(buff2));
					offst2 = 0;
				}
			}

			usleep(100*1000);
			i ++;
			j --;
		}
		close(pfd[0]);
		close(fd_wcb);
		close(fd_rcb);
		close(fd_wca);
		close(fd_rca);
		kill(pid, SIGKILL);
		exit(0);
	}
}
```
②客户端1程序：<br>
客户端程序每次将从服务器端接收到的数据通过pipe发送给子进程，然后将"A "或"B "发送给服务器端。<br>
```c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<memory.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<wait.h>
#include<signal.h>
#include<wait.h>
#define BUFFLEN 1000

int main()
{
        pid_t pid;
        int pfd[2];
        char buff1[BUFFLEN];
        int rdnum, fd_wca, fd_rca;
	char message[]="ss";
        int offst1=0;
        int res=0;
	int res2 =0;
        memset(buff1, 0 ,sizeof(buff1));
	if(pipe(pfd)<0)
        {
                perror("pipe error");
                exit(1);
        }
        pid = fork();
        if(pid<0)
        {
                perror("fork error");
                exit(1);
        }
	else if(pid == 0)
        {
                printf("ClientA child\n");
		close(pfd[1]);
		//close(pfd[0]);
		//exit(EXIT_SUCCESS);
                for(int i=0; i<20; i++)
                {
			printf("************clienta child running\n");
                        rdnum = read(pfd[0], buff1, 100);
			if(rdnum != 0)
                        	printf("ClietnA: read %d bytes from the Server: %s \n", rdnum, buff1);
			//printf("*******");
			usleep(100*1000);
                }
		close(pfd[0]);
                exit(EXIT_SUCCESS);
        }
	 else
        {	
		close(pfd[0]);
		printf("ClientA parent\n");
		//write(pfd[1], "ssss", 4);
		res = access("./WT_CLNTA", F_OK); 
                if (res == -1)
		{
			kill(pid, SIGKILL);
			exit(0);
		}
		res2 = access("./RDF_CLNTA", F_OK);
		if ( res2 == -1)
		{
			kill(pid, SIGKILL);
			exit(0);
		}
		printf("--------------------");
                //fd_wca = open("./WT_CLNTA", O_RDONLY|O_NONBLOCK);
                fd_rca = open("./RDF_CLNTA",O_WRONLY|O_NONBLOCK);
		fd_wca = open("./WT_CLNTA", O_RDONLY|O_NONBLOCK);
		printf("fd_wca = %d, fd_rca = %d \n",fd_wca,fd_rca);

		write(fd_rca, message, 2);
		for(int i =0; i < 2; i++ )
		{
			for(int j = 0; j<2; j++)
			{
				usleep(100*1000);
				if( (rdnum = read( fd_wca, buff1 + offst1, BUFFLEN)) > 0 )
				{
					printf("ClientA: receive %d bytes\n", rdnum);
					write(pfd[1], buff1+ offst1, rdnum);
					offst1 += rdnum;
					//usleep(100*1000);
				}
				message[0] = 'A' + j;
				message[1] = ' ';
				write(fd_rca, message, 2);
				printf("ClientA: sent %s to Server\n", message);
			}
			if(offst1 != 0)
			{
				write(fd_rca, buff1, offst1);
                                memset(buff1, 0 ,sizeof(buff1));
                                offst1  = 0;
			}
		}
		//close(pfd[1]);
               	close(fd_wca);
                close(fd_rca);
		wait(NULL);
		close(pfd[1]);
		//kill(pid, SIGKILL);
		exit(EXIT_SUCCESS);

	}

}
```
③客户端2程序与客户端1程序类似，省略；<br><br>
实验结果如下：<br>
可以看到，客户端成功发送信息"A "和"B "给服务器端，服务器端的子进程打印出了接收到的数据；服务器端成功发送信息给客户端，客户端的子进程打印出了接收到的数据；
![pipe/fifo result](https://github.com/yiyading/Embedded-software/blob/master/xh_322/img/sc1.png)<br>
![pipe/fifo result](https://github.com/yiyading/Embedded-software/blob/master/xh_322/img/sc2.png)<br>
## 四、实验结论
（1）vfork()保证子进程先运行，在它调用 exec（进程替换） 或 exit（退出进程）之后父进程才可能被调度运行。如果子进程没有调用 exec, exit, 程序则会导致死锁，程序是有问题的程序，没有意义。<br>
(2) 对于FIFO，需要open去打开FIFO的读端或是写端的描述符。<br>
* 如果open的时候没有指定O_NONBLOCK标志，且open的是读端时：如果不存在此FIFO的已经打开的写端时，open会一直阻塞到有FIFO的写端打开；如果已经存在此FIFO的打开的写端时，open会直接成功返回。<br>
* 如果open的时候没有指定O_NONBLOCK标志，且open的是写端时：如果不存在此FIFO的已经打开的读端时，open会一直阻塞到有FIFO的读端打开；如果已经存在此FIFO的打开的读端时，open会直接成功返回。<br>
(3) 从FIFO或者空管道读写<br>
* read时，读端fd没有指定O_NONBLOCK标志: 如果存在此FIFO或管道的已经打开的写端时，阻塞到FIFO或管道中有数据或者FIFO或管道的已经打开的写端全部被关闭为止。如果不存在此FIFO或管道的已经打开的写端时，read返回0；<br>
* write时, 同read差不多，就不详述了。
