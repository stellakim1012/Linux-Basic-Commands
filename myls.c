#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <sys/ioctl.h>


typedef struct Inform
{
	int inode;
	int size;
	int block;
	int mon;
	int day;
	int hour;
	int min;	
	int hardlink;

	char type;
	char name[100];
	char permission[11];
	char username[30];
	char groupname[30];
}Inform;

	
int myls_a = 0;
int myls_i = 0;
int myls_s = 0;
int myls_F = 0;
int myls_l = 0;
int myls_R = 0;
int myls_r = 0;
int myls_no = 0;

Inform inform[10000];

int sumsize = 0;
int maxlen = 0;
int temp = 0;

void sort()
{
	for(int i =0; i<temp-1; i++)
	{
		for(int j = i+1; j<temp; j++)
                {       
                        if(strcmp(inform[i].name, inform[j].name) > 0)
                        {
                                char tmpsort[100];
				char tmppermission[11];
				char tmpusername[30];
				char tmpgroupname[30];
				int tmp;

                                strcpy(tmpsort,inform[i].name);
                                strcpy(inform[i].name,inform[j].name);
                                strcpy(inform[j].name,tmpsort);

				tmp = inform[i].inode;
				inform[i].inode = inform[j].inode;
				inform[j].inode = tmp;
					
				tmp = inform[i].block;
                                inform[i].block = inform[j].block;
                                inform[j].block = tmp;

				tmp = inform[i].size;
                                inform[i].size = inform[j].size;
                                inform[j].size = tmp;

				tmp = inform[i].inode;
                                inform[i].inode = inform[j].inode;
                                inform[j].inode = tmp;
				
				tmp = inform[i].mon;
                                inform[i].mon = inform[j].mon;
                                inform[j].mon = tmp;

				tmp = inform[i].day;
                                inform[i].day = inform[j].day;
                                inform[j].day = tmp;

				tmp = inform[i].min;
                                inform[i].min = inform[j].min;
                                inform[j].min = tmp;
			
				tmp = inform[i].hour;
                                inform[i].hour = inform[j].hour;
                                inform[j].hour = tmp;

				tmp = inform[i].hardlink;
                                inform[i].hardlink = inform[j].hardlink;
                                inform[j].hardlink = tmp;

				char tp = inform[i].type;
				inform[i].type = inform[j].type;
				inform[j].type = tp;

				strcpy(tmppermission,inform[i].permission);
                                strcpy(inform[i].permission,inform[j].permission);
                                strcpy(inform[j].permission,tmppermission);

				strcpy(tmpusername,inform[i].username);
                                strcpy(inform[i].username,inform[j].username);
                                strcpy(inform[j].username,tmpusername);

				strcpy(tmpgroupname,inform[i].groupname);
                                strcpy(inform[i].groupname,inform[j].groupname);
                                strcpy(inform[j].groupname,tmpgroupname);

                        }
		}
	}		
}

void listfunc(char* new)
{
	struct stat finfo;
	struct tm* timeget;
	struct dirent *dentry;
	
	DIR *dir;
		
	if((dir = opendir(new)) == NULL)
	{
		printf("경로가 존재하지 않습니다.");
	}

	chdir(new);

	while(dentry = readdir(dir))
	{
		lstat(dentry->d_name,&finfo);
		if(myls_a == 0 && dentry->d_name[0] == '.')
			continue;
		else
			strcpy(inform[temp].name,dentry->d_name);

		if(myls_s == 1)
		{
			inform[temp].block = finfo.st_blocks / 2;
			sumsize += inform[temp].block;
		}

		else
			sumsize += finfo.st_blocks / 2;

		if(myls_i == 1)
			inform[temp].inode = dentry->d_ino;
		
		if(myls_F == 1)
		{
			if(S_ISDIR(finfo.st_mode))
				inform[temp].type = '/';
			else if(S_ISSOCK(finfo.st_mode))
				inform[temp].type = '=';
			else if(S_ISFIFO(finfo.st_mode))
				inform[temp].type = '|';
			else if(S_ISLNK(finfo.st_mode))
				inform[temp].type = '@';
			else if(finfo.st_mode == 33261)
				inform[temp].type = '*';
			else
				inform[temp].type = ' ';
		}

		if(myls_l == 1)
		{
			if((S_IFDIR & finfo.st_mode) == S_IFDIR)
				inform[temp].permission[0]='d';
			else if((S_IFLNK & finfo.st_mode) == S_IFLNK)
				inform[temp].permission[0]='l';
			else if((S_IFSOCK & finfo.st_mode) == S_IFSOCK)
				inform[temp].permission[0]='s';
			else if((S_IFBLK & finfo.st_mode) == S_IFBLK)
				inform[temp].permission[0]='b';
			else if((S_IFCHR & finfo.st_mode) == S_IFCHR)
				inform[temp].permission[0]='c';
			else
				inform[temp].permission[0]='-';

			if((S_IRUSR & finfo.st_mode) == S_IRUSR)
				inform[temp].permission[1]='r';
			else
				inform[temp].permission[1]='-';

			if((S_IWUSR & finfo.st_mode) == S_IWUSR)
                                inform[temp].permission[2]='w';
                        else 
                                inform[temp].permission[2]='-';

			if((S_IXUSR & finfo.st_mode) == S_IXUSR)
                                inform[temp].permission[3]='x';
                        else 
                                inform[temp].permission[3]='-';

                        if((S_IRGRP & finfo.st_mode) == S_IRGRP)
                                inform[temp].permission[4]='r';
                        else 
                                inform[temp].permission[4]='-';

                        if((S_IWGRP & finfo.st_mode) == S_IWGRP)
                                inform[temp].permission[5]='w';
                        else 
                                inform[temp].permission[5]='-';

                        if((S_IXGRP & finfo.st_mode) == S_IXGRP)
                                inform[temp].permission[6]='x';
                        else 
                                inform[temp].permission[6]='-';

                        if((S_IROTH & finfo.st_mode) == S_IROTH)
                                inform[temp].permission[7]='r';
                        else 
                                inform[temp].permission[7]='-';

                        if((S_IWOTH & finfo.st_mode) == S_IWOTH)
                                inform[temp].permission[8]='w';
                        else 
                                inform[temp].permission[8]='-';

                        if((S_IXOTH & finfo.st_mode) == S_IXOTH)
                                inform[temp].permission[9]='x';
                        else 
                                inform[temp].permission[9]='-';

			inform[temp].hardlink = finfo.st_nlink;
			inform[temp].size = finfo.st_size;

			timeget = localtime(&finfo.st_mtime);

			inform[temp].mon= timeget->tm_mon+1;
			inform[temp].day= timeget->tm_mday;
			inform[temp].hour = timeget->tm_hour;
			inform[temp].min = timeget->tm_min;
			
			struct passwd *pwd = getpwuid(finfo.st_uid);
			struct group *grp = getgrgid(finfo.st_gid);

			strcpy(inform[temp].username, pwd->pw_name);
			strcpy(inform[temp].groupname, grp->gr_name);
		}
		temp++;
			
		if(myls_a == 0 && myls_R == 1 && S_ISDIR(finfo.st_mode))
		{
			printf("%s:\n", new);
			listfunc(dentry->d_name);
							
		}

		if(maxlen < strlen(dentry->d_name))
			maxlen = strlen(dentry->d_name);
	}
	closedir(dir);
	chdir("..");
}

void print()
{
	struct winsize size;
	ioctl(STDOUT_FILENO,TIOCGWINSZ, &size);
	int line = 0;
	int cnt = 1;

		if(myls_l == 0 && myls_r == 0)
		{
			if(myls_i == 1 ||myls_s == 1 || myls_F == 1) cnt++;
			
			if(size.ws_col > 120) line= 3/cnt+3;
			else if(size.ws_col > 90) line= 3/cnt+2;
			else if(size.ws_col > 60) line= 3/cnt+1;
			else if(size.ws_col > 30) line= 3/cnt;
			else line = 2;
			
			for(int i =0; i<temp; i++)
	                {
				if(i != 0 && i % line == 0)
					printf("\n");
				else if (i != 0)
					printf("  ");

                       		if(myls_i == 1)
                               		printf("%d  ", inform[i].inode);

                	        if(myls_s == 1)
        	                        printf("%d  ", inform[i].block);

 	                        printf("%s", inform[i].name);

				if(myls_F == 1)
               	                	printf("%c  ", inform[i].type);
				
				for(int k = 0; k<maxlen-strlen(inform[i].name); k++)
				{
					if((i+1)%line == 0)
						continue;
					else					
						printf(" ");
				}
                        	if(i == temp-1)
					printf("\n");
			}

		}

		else if(myls_l == 0 && myls_r == 1)
		{
			if(myls_i == 1 ||myls_s == 1 || myls_F == 1) cnt++;
			
			if(size.ws_col > 120) line= 3/cnt+3;
			else if(size.ws_col > 90) line= 3/cnt+2;
			else if(size.ws_col > 60) line= 3/cnt+1;
			else if(size.ws_col > 30) line= 3/cnt;
			else line = 2;
			
			int j=0;
			for(int i =temp-1; i>=0; i--)
	                {
				if(j!=0 && j % line == 0)
					printf("\n");
				else if (j != 0)
					printf("  ");
				j++;
                       		if(myls_i == 1)
                               		printf("%d  ", inform[i].inode);


                	        if(myls_s == 1)
        	                        printf("%d  ", inform[i].block);

 	                        printf("%s", inform[i].name);

                	        if( myls_F == 1)
               	                 	printf("%c  ", inform[i].type);

				for(int k = 0; k<maxlen-strlen(inform[i].name); k++)
				{
					if((j)%line == 0)
						continue;
					else					
						printf(" ");
				}

                        	if(i == 0)
					printf("\n");
			}
		}		

		else if(myls_r ==1)
		{
			printf("합계: %d\n",sumsize);
			for(int i =temp-1; i>=0; i--)
	                {

                       		if(myls_i == 1)
                               		printf("%d  ", inform[i].inode);

				if(myls_s == 1 )
        	                        printf("%d  ", inform[i].block);

                   	        if(myls_l ==1)
                       		 {
               	           	        printf("%s  ", inform[i].permission);
               	 	                printf("%d  ", inform[i].hardlink);
               	         	        printf("%s  ", inform[i].username);
               	                 	printf("%s  ", inform[i].groupname);
         	          		printf("%d  ", inform[i].size);
                        	        printf("%d월 ", inform[i].mon);
                	                printf("%d일 ", inform[i].day);
        	               		printf("%d", inform[i].hour);
					printf(":%d  ", inform[i].min);

                        	}

				printf("%s", inform[i].name);
                	        if(myls_F== 1)
               	                	printf("%c  ", inform[i].type);

                        	printf("\n");
			}

		}

		else if(myls_r == 0)
		{
			printf("합계: %d\n",sumsize);
			for(int i =0; i<temp; i++)
			{
			
				if(myls_i == 1)
					printf("%d  ", inform[i].inode);
		
				if(myls_s == 1 )
        	                        printf("%d  ", inform[i].block);

				if(myls_l ==1)
				{
					printf("%s  ", inform[i].permission);
					printf("%d  ", inform[i].hardlink);
					printf("%s  ", inform[i].username);
					printf("%s  ", inform[i].groupname);
					printf("%d  ", inform[i].size);
					printf("%d월 ", inform[i].mon);
					printf("%d일 ", inform[i].day);
					printf("%d", inform[i].hour);
					printf(":%d  ", inform[i].min);
				}

				printf("%s", inform[i].name);

				if(myls_F== 1)
					printf("%c  ", inform[i].type);

				printf("\n");
			}
		}
}

int main(int argc, char* argv[])
{
	
	if(argc < 2)
	{	
		listfunc(".");
		sort();
		print();
	}

	else if(argc>=2)
	{
	   if(*argv[1] == '-'){	
		int opt;
		while((opt = getopt(argc,argv,"asliFRr")) != -1)
		{
			switch(opt)
			{
				case 'a':
					myls_a = 1;
					break;
				case 's':
					myls_s = 1;
					break;
				case 'l':
					myls_l = 1;
					break;
				case 'i':
					myls_i = 1;
					break;
				case 'F':
					myls_F = 1;
					break;
				case 'R':
					myls_R = 1;
					break;
				case 'r':
					myls_r = 1;
					break;
				default: 
					myls_no = 1;
					break;	
			}
		}
		
		if(myls_no == 1)
			return 0;

		if(argc == 2 && *argv[1] == '-')
		{
			listfunc(".");
			sort();
			print();
		}
			

		for (int i =2; i<argc; i++)
			{
				listfunc(argv[i]);

				if( i == argc-1)
				{
					sort();
					print();
				}		
			}
	  }

	else{
			for (int i =1; i<argc; i++)
			{
				printf("%s:\n", argv[i]);
				listfunc(argv[i]);
				sort();
				print();
				printf("\n");		
			}
		}
	}
}