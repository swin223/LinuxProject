#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// 定义联合体，以启动semctl系统调用
union semun
{
    int val;
    struct semid_ds* buf;
    unsigned short int* array;
    struct seminfo* __buf;
};

// PV操作
// op = -1表示执行P操作，op = 1表示执行V操作
void pv(int sem_id,int op)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = op;
    sem_b.sem_flg = SEM_UNDO;
    semop(sem_id,&sem_b,1);
}

int main(int argc,char* argv[])
{
    // 创建新的信号量集
    // 0666表示所有用户都是可读可写的
    int sem_id = semget(IPC_PRIVATE,1,0666);

    // 调用semctl来设置信号量的值
    union semun sem_un;
    sem_un.val = 1;
    semctl(sem_id,0,SETVAL,sem_un);

    // 创建子进程
    pid_t id = fork();
    // 创建子进程失败
    if(id < 0)
    {
        return 1;
    }
    // 子进程
    else if(id == 0)
    {
        printf("child try to get binary sem\n");
        // 在父子进程间共享IPC_PRIVATE信号量的关键就在于二者都可以操作该信号量的标识符sem_id
        pv(sem_id,-1);
        printf("child get the sem and would release it after 5 sec\n");
        sleep(5);
        pv(sem_id,1);
        exit(0);
    }
    else
    {
        printf("parent try to get binary sem\n");
        pv(sem_id,-1);
        printf("parent get the sem and would release it after 5 sec\n");
        sleep(5);
        pv(sem_id,1);
    }

    // 处理僵尸进程
    // 此时子进程处于运行结束，但是父进程没有读取其退出信息 -- 僵尸进程
    waitpid(id,NULL,0);
    printf("end!\n");
    // 立即删除信号量集
    semctl(sem_id,0,IPC_RMID,sem_un);
    return 0;
}
