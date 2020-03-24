#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<memory.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<wait.h>
#include<signal.h>
#define BUFFLEN 1000

int main()
{
        pid_t pid;
        int pfd[2];
        char buff1[BUFFLEN];
        int rdnum, fd_wcb, fd_rcb;
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
                printf("ClientB child\n");
		close(pfd[1]);
		//close(pfd[0]);
		//exit(EXIT_SUCCESS);
                for(int i=0; i<20; i++)
                {
			printf("*********clientb child running\n");
                        rdnum = read(pfd[0], buff1, 100);
			if(rdnum != 0)
                        	printf("ClietnB: read %d bytes from the Server: %s \n", rdnum, buff1);
			//printf("*******");
			usleep(50*1000);
                }
		close(pfd[0]);
                exit(0);
        }
	 else
        {	
		close(pfd[0]);
		printf("ClientB parent\n");
		//write(pfd[1], "ssss", 4);
		res = access("./WT_CLNTB", F_OK); 
                if (res == -1)
		{
			kill(pid, SIGKILL);
			exit(0);
		}
		res2 = access("./RDF_CLNTB", F_OK);
		if ( res2 == -1)
		{
			kill(pid, SIGKILL);
			exit(0);
		}
		printf("--------------------\n");
                //fd_wcb = open("./WT_CLNTB", O_RDONLY|O_NONBLOCK);
		fd_rcb = open("./RDF_CLNTB",O_WRONLY|O_NONBLOCK);
		fd_wcb = open("./WT_CLNTB", O_RDONLY|O_NONBLOCK);
		printf("ClientB: fd_rcb = %d, fd_wcb = %d ", fd_rcb, fd_wcb);
		write(fd_rcb, message, 2);
		for(int i =0; i < 2; i++ )
		{
			for(int j = 0; j<2; j++)
			{
				usleep(100*1000);
				if( (rdnum = read( fd_wcb, buff1 + offst1, BUFFLEN)) > 0 )
				{
					write(pfd[1], buff1+ offst1, rdnum);
					offst1 += rdnum;
					//usleep(100*1000);
				}
				message[0] = 'A' + j;
				message[1] = ' ';
				write(fd_rcb, message, 2);
				printf("ClientB: sent %s to Server\n", message);
			}
			if(offst1 != 0)
			{
				write(fd_rcb, buff1, offst1);
                                memset(buff1, 0 ,sizeof(buff1));
                                offst1  = 0;
			}
		}
		//close(pfd[1]);
               	close(fd_wcb);
                close(fd_rcb);
		wait(NULL);
		close(pfd[1]);
		//kill(pid, SIGKILL);
		exit(EXIT_SUCCESS);

	}


}
