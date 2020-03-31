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

int main(){
        pid_t pid; int pfd[2]; char buff1[BUFFLEN];
        int rdnum, fd_wca, fd_rca;
	char message[]="ss";
        int offst1=0, res=0, int res2 =0;
        memset(buff1, 0 ,sizeof(buff1));
	if(pipe(pfd)<0){
                perror("pipe error");
                exit(1);
        }

        pid = fork();
        if(pid<0){
                perror("fork error");
                exit(1);
        }else if(pid == 0){
                printf("ClientA child\n");
		close(pfd[1]);
		//close(pfd[0]);
		//exit(EXIT_SUCCESS);
                for(int i=0; i<20; i++){
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
