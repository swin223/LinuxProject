#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("this pid  = %d\n",getpid());

    if(fork() == 0)
    {
        // 进入子进程，对该子进程进行进程映像替换
        printf("enter child process\n");

        // 调用execlp()函数，调用ls进程
        if(execlp("tree","-a",NULL) < 0)
        {
            printf("error!");
        }
        // 该进程已经被替换，实际上后续代码都不进行执行！
        printf("this pid  = %d\n",getpid());
    }
   return 0; 
}
