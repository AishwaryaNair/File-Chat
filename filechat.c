#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

volatile sig_atomic_t offsetbytes = 0;

char * timestamp(){
	time_t lttime;
	lttime = time(NULL);
	return(asctime(localtime(&lttime)));
}

void help(){
	printf("Usage: ./proj2 FILE1 FILE2\n");
	printf("FILE1: input file\n");
	printf("FILE2: output file \n");
}

void timeout(int sig){
	//Timeout
}

int main(int argc, char * argv[]){
	struct sigaction ps;
	ps.sa_handler = timeout;
	if(sigaction(SIGALRM, &ps, NULL) < 0){
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	char inpbuf[BUF_SIZE];
	char filebuf[BUF_SIZE];
	if (argc < 3){
	 	help();
	  	exit(EXIT_FAILURE);
	}

	else{
		int outputfile,inputfile;
		size_t readbytes = 0;
		if((outputfile = open(argv[2], O_RDWR | O_CREAT | O_APPEND, 0777)) == -1){
			perror("open");
			exit(EXIT_FAILURE);
		}
		if((inputfile = open(argv[1], O_RDWR, 0777)) == -1){
			perror("open");
			printf("Input file doesnot exist.\n" );
			exit(EXIT_FAILURE);
		}
		printf("Enter within three seconds\n");
		int getlineno;
		inputread:
			alarm(3);
			errno = 0;	
			char * line = NULL;
			size_t len = 0;
			getlineno = read(0, inpbuf, BUF_SIZE);
			alarm(3);
			char * readfromterminal;
			if(getlineno >= 0 && errno == 0){
				inpbuf[getlineno-1] = 0;
				inpbuf[getlineno] = '\0';
				char * printtime = timestamp();
				printtime[strlen(printtime)-1] = 0;
				char * writebuffer;
				if((writebuffer = malloc(strlen(inpbuf)+ 1 + strlen(printtime) + 2)) != NULL){
					writebuffer[0] = '\0';
					strcat(writebuffer, printtime);
					strcat(writebuffer,",");
					strcat(writebuffer,inpbuf);
					strcat(writebuffer,"\n");
				}
				else{
					perror("malloc");
					exit(EXIT_FAILURE);
				}
				//printf("%s\n",writebuffer );
				if(write (outputfile,writebuffer,strlen(writebuffer)) != strlen(writebuffer)){
					perror("write");
					exit(EXIT_FAILURE);
				}
				free(writebuffer);
				memset(inpbuf,0,BUF_SIZE);
				goto inputread;
				}
				if(errno == EINTR){
					errno = 0;
					goto fileread1;
				}
				if(getlineno < 0){
					errno = 0;
					goto fileread1;
				}

		fileread1:
			alarm(3);
			if(lseek(inputfile,offsetbytes,SEEK_SET) == -1){
				perror("lseek");
				exit(EXIT_FAILURE);
			}

			readbytes = read(inputfile,filebuf, BUF_SIZE);
			offsetbytes = offsetbytes + readbytes;
			if(readbytes != EOF){
				filebuf[readbytes-1] = 0;
				filebuf[readbytes] = '\0';
				if(readbytes != 0){
					printf("%s\n", filebuf);
				}
				memset(filebuf,0,BUF_SIZE);
				goto inputread;
			}
			else{
			goto inputread;
			}
			if(errno == EINTR){
				goto inputread;
			}

		if(close(outputfile) == -1){
			perror("close");
			exit(EXIT_FAILURE);
		}
		if(close(inputfile) == -1){
			perror("close");
			exit(EXIT_FAILURE);
		}
	}
}