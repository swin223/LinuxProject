#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc,char* argv[])
{
    // 获取该进程的pid
    printf("pid is %d\n",getpid());

    // 调用fork函数创建子进程
    printf("start fork!\n");
    pid_t pid = fork();

    // fork系统调用出错
    if(pid < 0)
    {
        perror("fork error\n");
    }
    // 父进程显示调用
    else if(pid > 0)
    {
        printf("------parent------\n");
        printf("i am parent process pid = %d,ppid = %d,fork return = %d\n",getpid(),getppid(),pid);
        printf("------parent--1---\n");
        printf("------parent--2---\n");
        printf("------parent--3---\n");
        printf("------parent--4---\n");
        printf("------parent--5---\n");
        printf("------parent--6---\n");
        printf("------parent--7---\n");
        printf("------parent--8---\n");
        printf("------parent--9---\n");
        printf("------parent--10---\n");
    }
    // 子进程显示调用
    else
    {
        printf("------child------\n");
        printf("i am child process pid = %d,ppid = %d,fork return = %d\n",getpid(),getppid(),pid);
        printf("------child---1--\n");
        printf("------child---2--\n");
        printf("------child---3--\n");
        printf("------child---4--\n");
        printf("------child---5--\n");
        printf("------child---6--\n");
        printf("------child---7--\n");
        printf("------child---8--\n");
        printf("------child---9--\n");
        printf("------child---10--\n");
    }

    printf("------process ending------\n");
    printf("this process pid = %d\n",getpid());

    sleep(15);
    return 0;
}
