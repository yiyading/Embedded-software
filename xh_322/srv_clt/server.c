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
	pid_t pid; int pfd[2]; char buff1[BUFFLEN]; char buff2[BUFFLEN];
	//char message[]="communication is successful!";
	int rdnum, wtnum, fd_wca, fd_wcb, fd_rca, fd_rcb;
	int offst1=0, offst2=0;
	int i=0, j=2, res=0;
	memset(buff1, 0 ,sizeof(buff1)); 
	memset(buff2, 0 ,sizeof(buff2));
	if(pipe(pfd)<0)	{
		perror("pipe error");
		exit(1);
	}

	pid = fork();
	if(pid<0){
		perror("fork error");
		exit(1);
	}else if(pid == 0){
		close(pfd[1]);
		while(1){
			usleep(50*1000);
			rdnum = read(pfd[0], buff1, 100);
			printf("Server Child: received %d bytes from the Server Parent: %s \n", rdnum, buff1);
		}
		exit(0);
	}else{
		// 创建FIFO文件
		if (access("./WT_CLNTA", F_OK) == -1){
			res = mkfifo("./WT_CLNTA", 0777);
			if (res != 0){
				fprintf(stderr, "Could not create fifo:w2a\n");
				exit(EXIT_FAILURE);
			}
		}

		if (access("./RDF_CLNTA", F_OK) == -1){
                        res = mkfifo("./RDF_CLNTA", 0777);
                        if (res != 0){
                                fprintf(stderr, "Could not create fifo r2a\n");
                                exit(EXIT_FAILURE);
                        }
                }

		if (access("./WT_CLNTB", F_OK) == -1){
                        res = mkfifo("./WT_CLNTB", 0777);
                        if (res != 0){
                                fprintf(stderr, "Could not create fifo w2b\n");
                                exit(EXIT_FAILURE);
                        }
                }

		if (access("./RDF_CLNTB", F_OK) == -1){
                        res = mkfifo("./RDF_CLNTB", 0777);
                        if (res != 0){
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
		while(j>0){
			if((rdnum=read(fd_rca, buff1+offst1, BUFFLEN)) > 0){
				printf("Server: received %d bytes from ClientA\n", rdnum);
				write(pfd[1], buff1 + offst1 , rdnum);
				offst1 += rdnum;
			}
			if((rdnum=read( fd_rcb, buff2 + offst2, BUFFLEN)) > 0){
				printf("Server: received %d bytes from ClientB\n", rdnum);
				write(pfd[1], buff2 + offst2, rdnum);
				offst2 += rdnum;
			}
			if(i==1){
				printf("server has run 2 times\n");
				printf("offst1 2 = %d\n", offst1);
				i = 0;
				if( offst1 != 0){
					if( (wtnum = write(fd_wca, buff1, offst1)) != 0 )
						printf("Server: write %d bytes to ClientA\n", wtnum);
					memset(buff1, 0 ,sizeof(buff1));
					offst1  = 0; 
				}
				if( offst2 != 0 ){
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

