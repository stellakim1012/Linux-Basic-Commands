#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int n, fd, flag, opt, ef;
	int cnt = 0;
	char buf[1024];
	char eos = '$';
	FILE *fp;

	while((opt = getopt(argc, argv, "En")) != -1)
	{
		switch(opt)
		{
		case 'E':
			ef = 1;
			break;
		case 'n':
			flag = 1;
			break;
		default:
			printf("Try 'mycat --help' for more information. \n");
			exit(EXIT_FAILURE);
		}
	}

	if (argc == 1 && flag==0 && ef == 0) {
		while ((n = read(0, buf, 1024)) > 0 ) {
			write(1, buf, n);
		}
	}

	else if (argc == 2 && ef == 1) {
		while ((n=read(0, buf, 1024)) > 0) {
			for (int i=0; buf[i] != 0; i ++ ) {
				if (buf[i] == '\n') {
					buf[i] = 0;
					break;
				}
			}
			printf("%s%c\n", buf, eos);
			getchar();
		}
	}

	else if(argc >= 2 && flag == 0) {
		int index = 1;
		for (;index<argc;index++) {
			fd = open(argv[index], O_RDONLY);

			if(fd >= 0) {
				while((n = read(fd, buf, 1024)) > 0) {
					if (write(1, buf, n) < n) {
						perror("Write Error!");
						break;
					}
				}
				close(fd);
			}
			else if(fd == -1) {
				printf("can't read file \n");
			}
		}
	}

	else if (argc >=3 && flag ==1) {
		int index = 2;
		for (;index<argc;index++) {
			fp = fopen(argv[index], "r");
			fd = open(argv[index], O_RDONLY);

			if (fd >= 0 ) {
				while(1) {
					fgets(buf, sizeof(buf), fp);
					if(feof(fp))
						break;
					printf("%4d %s", ++cnt, buf);
					}
				fclose(fp);
			}
			else if(fd == -1) {
				printf("can't read file \n");
			}
		}
	}
	return 0;
}