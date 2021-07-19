#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
//추가로 필요한 헤더 파일들을 명시한다.
 
struct CMD{
    char *name;//명령 이름
    char *desc;//명령어 설명
    int (*cmd)(int argc, char *argv[]); //명령 실행 함수에 대한 포인터
};
 
int cmd_cd(int argc, char *argv[]);
int cmd_pwd(int argc, char *argv[]);
int cmd_exit(int argc, char *argv[]);
int cmd_help(int argc, char *argv[]);
int cmd_echo(int argc, char *argv[]);
int cmdProcessing(void);
 
struct CMD builtin[] = { //내장 명령 정보
    {"cd", "작업 디렉터리 바꾸기", cmd_cd },
    {"pwd", "현재 작업 디렉터리는?", cmd_pwd },
    {"exit", "셸 실행을 종료합니다", cmd_exit},
    {"help", "도움말 보여 주기", cmd_help},
    {"echo", "텍스트 줄을 출력합니다", cmd_echo}
};
const int builtins = 5; //내장 명령의 수
 
void int_handler(int signum){ printf("\n");}
 
int main(void)
{
    static struct sigaction n, o;
    int isExit = 0;
 
    n.sa_handler = int_handler;
    sigemptyset(&n.sa_mask);
    sigaction(SIGINT, &n, &o);
    
    while (!isExit)
        isExit = cmdProcessing();   // 명령 처리 루틴 호출
    fputs ("My Shell 을 종료합니다...\n", stdout);
    return 0;
}
 
#define STR_LEN 1024// 최대 명령 입력 길이
#define MAX_TOKENS 128 // 공백으로 분리되는 명령어 문자열 최대 수
 
int cmdProcessing (void)
{
    char cmdLine[STR_LEN]; //입력 명령 전체를 저장하는 배열
    char outfile[STR_LEN] = "/bin/";
    char *cmdTokens[MAX_TOKENS];//입력 명령을 공백으로 분리하여 저장하는 배열
    char delim[] = " \t\n\r";//토큰 구분자 - strtok에서 사용
    char *token;//토큰 (분리된 입력 문자열)
    int tokenNum; //입력 명렁에 저장된 토큰 수
 
    //기타 필요한 변수 선언...
    pid_t pid;
    int status = 0;
    int i = 0;
 
    int exitCode = 0;// 종료 코드 (default = 0)
    fputs("[mysh v0.1] $ ", stdout);// 프롬프트 출력
    fgets(cmdLine, STR_LEN, stdin);// 한 줄의 명령 입력
   
    token = strtok(cmdLine, delim); //입력 명령의 문자열 하나 분리
    tokenNum = 0;
    while(token){
        cmdTokens[tokenNum++] = token;//분리된 문자열을 배열에 저장
        token = strtok(NULL, delim); //연속하여 입력 명령의 문자열 하나 분리
    }
    cmdTokens[tokenNum] = NULL;
    if(tokenNum == 0)
        return exitCode;
    for(int i =0; i < builtins; ++i)
    {//내장 명령인지 검사하여 명령이 있으면 해당 함수 호출
        if(strcmp(cmdTokens[0], builtin[i].name)==0)
            return builtin[i].cmd(tokenNum, cmdTokens);
    }
    
    // 내장 명령이 아닌 경우 자식 프로세스를 생성하여 명령 프로그램을 호출함
    pid = fork(); //자식 프로세서 생성.

    if(pid == 0) // Child
    {
        strcat(outfile, cmdTokens[0]);
        execv(outfile, cmdTokens);
        exit(0);
    }
    else if(pid>0) // Parent
    {
        wait(&status);
    }
    else // Fail
    {
        fputs("fail\n",stdout);
    }
    memset(cmdLine,0,sizeof(char)*STR_LEN);
    return exitCode;
}
 
int cmd_cd(int argc, char *argv[])
{
    /*cd 명령 처리*/
    struct stat temp;
    
    if(argc == 1)
    {
        chdir(getenv("HOME"));
    }
    else if(strcmp(argv[1],"~")==0)
    {
            chdir(getenv("HOME"));
    }
    else
    {
        if(stat(argv[1], &temp) == -1) //bring info
        {
            printf("%s: 그런 파일이나 디렉터리가 없습니다\n", argv[1]);
        }
        else if(S_ISDIR(temp.st_mode))//디렉터리 인지 확인
        {
            chdir(argv[1]); 
        }
        else
        {
            printf("%s: 디렉터리가 아닙니다\n", argv[1]);
        }
    }
    
    return 0;
}
 
int cmd_pwd(int argc, char *argv[])
{
    /*pwd 명령 처리*/
    char buffer[STR_LEN];
    getcwd(buffer, STR_LEN);
    printf("%s\n", buffer);
    return 0;
}
 
int cmd_exit(int argc, char *argv[])
{
    return 1;
}
 
int cmd_help(int argc, char *argv[])
{
    /*help 명령 처리*/
    if(argc == 1)
    {
        for(int i =0; i < builtins; i++)
        {
            printf("%s : %s\n", builtin[i].name, builtin[i].desc);
        }
    }

    else // help cd etc..
    {
        for(int i =0; i < builtins; i++)
        {
            if(strcmp(builtin[i].name ,argv[1]) == 0)
            {
                printf("%s : %s\n", builtin[i].name, builtin[i].desc);
                return 0;
            }
        }
        fputs("존재하지 않는 명령어입니다\n",stdout);
    }
    return 0;
}
int cmd_echo(int argc, char *argv[])
{
    /*echo 명령 처리*/
    for(int i =1; i < argc-1; i++)
    {
        printf("%s ",argv[i]);
    }
    printf("%s\n", argv[argc-1]);
    return 0;
}