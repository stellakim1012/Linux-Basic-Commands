#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char* argv[])
{
	char buf[1024];
	int fdin, fdout;
	ssize_t n;
	struct stat first, second;

	if (argc == 1) {
		printf("mycp: 파일 명령어 누락\n");
	}

	else if(argc == 2) {
		printf("mycp %s 다음에 대상 파일 명령어 누락됨 \n", argv[1]);
	}

	else if (argc >= 3) {
		stat(argv[1], &first);
		stat(argv[2], &second);

		if (S_ISLNK(first.st_mode) || S_ISLNK(second.st_mode) || S_ISREG(first.st_mode) && S_ISREG(second.st_mode)) {
			fdin = open(argv[1], O_RDONLY);
			fdout = creat(argv[2], 0644);
			while ((n = read(fdin, buf, 1024)) > 0) {
				if(write(fdout, buf, n) < n) {
					close(fdin);
					close(fdout);
				}
			}
		}

		else if(S_ISREG(first.st_mode) && access(argv[2], F_OK) == -1) {
			fdin = open(argv[1],O_RDONLY);
			fdout = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, first.st_mode);
			
			while ((n = read(fdin, buf, 1024)) > 0) {
				if (write(fdout, buf, n) < n) {
					perror("Write Error!");
					close(fdin);
					close(fdout);
					break;
				}
			}
		}

		else if(access(argv[1], F_OK) == -1)
			printf("복사할 파일이 존재하지 않습니다. \n");

	}
	close(fdin);
	close(fdout);
	return 0;
}