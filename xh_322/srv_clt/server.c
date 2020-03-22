#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<memory.h>
#include<sys/types.h>
#include<sys/stat.h>

int main()
{
	pid_t pid;
	int pfd[2];
	char buff1[100];
	char message[]="communication is successful!";
	int rdnum,wtnum;
	int res=0;
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
		close(pfd[1]);
		while(1)
		{
			usleep(100*1000);
			rdnum = read(pfd[0], buff1, 100);
			printf("read %d bytes from the pipe: %s \n", rdnum, buff1);
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
		while(1)
		{
			wtnum = write(pfd[1], "pipe-", 5);
			printf("write %d bytes to the pipe\n", wtnum);
			sleep(1);
		}
		exit(0);
	}
}

