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
    }
    // 子进程显示调用
    else
    {
        printf("------child------\n");
        printf("i am child process pid = %d,ppid = %d,fork return = %d\n",getpid(),getppid(),pid);
    }

    printf("------process ending------\n");
    printf("this process pid = %d\n",getpid());

    sleep(15);
    return 0;
}
