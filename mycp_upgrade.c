#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define MAX 1024

int main(int argc, char* argv[])
{
	int fdin, fdout;
	ssize_t nread;
	char buffer[1024];
	struct stat ostat1, ostat2, ostatend;

	if(argc == 1)
	{
		printf("mycp: 파일 명령어 누락\n");
		printf("Try 'mycp --help'for more information\n");
		return 0;
	}
	
	else if(argc == 2)
	{
		printf("mycp %s 다음에 대상 파일 명령이 누락됨\n", argv[1]);
		printf("TRy 'mycp --help' for more information.\n");
		return 0;
	}

	else if(argc == 3)
	{
		stat(argv[1],&ostat1);
		stat(argv[2],&ostat2);

		if(access(argv[1],F_OK)== -1)
		{
			printf("mycp %s 를 설명할 수 없음: 그런 파일이나 디렉터리가 없습니다.\n", argv[1]);
			return 0;
		}

		if (access(argv[1], R_OK) == -1)
		{
			printf("mycp: 읽기를 위해 '%s' 을(를) 열 수 없음: 허가 거부\n", argv[1]);
			return 0;
		}
	
		if(S_ISLNK(ostat1.st_mode)|| S_ISLNK(ostat2.st_mode) || S_ISREG(ostat1.st_mode) && S_ISREG(ostat2.st_mode))
		{
			
			fdin = open(argv[1],O_RDONLY);
			fdout = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC);
			
			while((nread = read(fdin,buffer,1024)) > 0)
			{
				if(write(fdout,buffer,nread) < nread)
				{
					perror("Write error!");
					break;		

				}
			}
			close(fdin);
			close(fdout);
		}

		else if(S_ISREG(ostat1.st_mode) && access(argv[2],F_OK) == -1)
		{
			
			fdin = open(argv[1],O_RDONLY);
			fdout = open(argv[2],O_WRONLY| O_CREAT | O_TRUNC, ostat1.st_mode);
			
			while((nread = read(fdin,buffer, 1024)) >0)
			{
				if(write(fdout,buffer,nread) < nread)
				{
					perror("Write error!");
					break;
				}
			}
			close(fdin);
			close(fdout);
		}

		else if(S_ISDIR(ostat1.st_mode))
		{
			
			printf("mycp: -r not specified; omitting directory '%s'.\n",argv[1]);
			return 0;
		}
		

		else if(S_ISREG(ostat1.st_mode) && S_ISDIR(ostat2.st_mode))
		{
			strcat(argv[2],"/");
			strcat(argv[2],argv[1]);

			stat(argv[2],&ostat2);

			if(access(argv[2],F_OK) == 0 &&  S_ISDIR(ostat2.st_mode))
			{
				printf("파일이 존재하나 디렉터리입니다.\n");
			}
			
			else if(access(argv[2], X_OK) == -1 || access(argv[2], W_OK == -1))
			{
				printf("mycp: %s/%s 를 설명할 수 없음: 허가 거부\n", argv[2], argv[1]);
			}

			else
			{
				fdin = open(argv[1],O_RDONLY);
				fdout = open(argv[2], O_WRONLY|O_CREAT,ostat1.st_mode);

				while((nread = read(fdin, buffer, 1024))>0)
				{
					if(write(fdout,buffer,nread) < nread)
					{
						perror("Write error!");
						break;		
					}
				}
				close(fdin);
				close(fdout);
			}
		}
	}	
		

	else
	{
		stat(argv[argc-1],&ostatend);		
		if(S_ISDIR(ostatend.st_mode))
		{
			char newpath[1024]; 
		
			for(int i = 1; i<argc-1; i++)
			{
				stat(argv[i],&ostat1);
				
				strcpy(newpath,argv[argc-1]);
				strcat(argv[argc-1],"/");
				strcat(argv[argc-1],argv[i]);

				fdin = open(argv[i],O_RDONLY);
				fdout = open(argv[argc-1],O_WRONLY|O_CREAT|O_TRUNC,ostat1.st_mode);

				if(access(argv[i],F_OK)== -1)
				{
					printf("mycp %s 를 설명할 수 없음: 그런 파일이나 디렉터리가 없습니다.\n", argv[i]);
					return 0;
				}

				if (access(argv[i], R_OK) == -1)
				{
					printf("mycp: 읽기를 위해 '%s' 을(를) 열 수 없음: 허가 거부\n", argv[i]);
					return 0;
				}
				while((nread = read(fdin, buffer, 1024)) >0)
				{
					if(write(fdout,buffer,nread) < nread)
					{
						perror("Write error!");
						break;
					}	

				}
				strcpy(argv[argc-1],newpath);
				
				close(fdin);
				close(fdout);	
			}
		}

		else
			printf("mycp: 대상 %s 은 디렉터리가 아님.\n", argv[argc-1]);
			return 0;
	}
}