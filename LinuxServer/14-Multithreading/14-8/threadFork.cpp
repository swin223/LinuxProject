#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

pthread_mutex_t mutex;

// 子线程运行函数
// 先获取互斥锁mutex，暂停5s之后释放
void* another(void* arg)
{
    printf("in child thread,lock the mutex\n");
    pthread_mutex_lock(&mutex);
    sleep(5);
    pthread_mutex_unlock(&mutex);
    printf("thread end\n");
}

void prepare()
{
    pthread_mutex_lock(&mutex);
}

void infork()
{
    pthread_mutex_unlock(&mutex);
}

int main()
{
    pthread_mutex_init(&mutex,NULL);
    pthread_t id;
    pthread_create(&id,NULL,another,NULL);

    // 父进程中主线程暂停1s，确保在执行fork操作之前，子线程已经开始运行并且获得了互斥变量mutex
    sleep(1);
    pthread_atfork(prepare,infork,infork);
    int pid = fork();
    if(pid < 0)
    {
        pthread_join(id,NULL);
        pthread_mutex_destroy(&mutex);
        return 1;
    }
    else if(pid == 0)
    {
        printf("I am in the child,wat to get the lock\n");
        // 子进程继承了互斥锁mutex的状态，此时该互斥锁处于锁住状态
        // 这是父进程中子线程引起的
        // 因此下面这句话加锁操作会一直阻塞，尽管从逻辑上来说它是不应该阻塞的
        pthread_mutex_lock(&mutex);
        printf("I can not run to here,oop...\n");
        pthread_mutex_unlock(&mutex);
        exit(0);
    }
    else
    {
        wait(NULL);
    }

    printf("wait thread join\n");
    pthread_join(id,NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}
