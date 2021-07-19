#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void alarm_handler(int n){}
 
void mysleep(int n){
    static struct sigaction a, b;
 
    a.sa_handler = alarm_handler;
    sigemptyset(&a.sa_mask);
 
    sigaction(SIGALRM,  &a, &b);
    alarm(n);
    pause();
 
    sigaction(SIGALRM, &b, NULL);
}
 
int main(int argc, char *argv[])
{
    int i = 0;
    while(i < 10){
        printf("Loop %d\n", i++);
        mysleep(1);
    }
    return 0;
}