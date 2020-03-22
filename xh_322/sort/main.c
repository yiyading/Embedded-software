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
	if((pid = vfork()) == -1){
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
		barrel(a, len,  b, 1);
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
