#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024
static int pipefd[2];

// 设置文件fd为非阻塞
int setnonblocking( int fd )
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

// 向epoll事件表中添加事件
void addfd( int epollfd, int fd )
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );
}

// 信号处理函数
void sig_handler( int sig )
{
    // 保证函数的可重入性
    // 保存全局变量errno之后进行恢复
    int save_errno = errno;
    int msg = sig;
    // Linux中的信号值不超过255
    send( pipefd[1], ( char* )&msg, 1, 0 );
    errno = save_errno;
}

// 设置信号处理的处理函数
void addsig(int sig)
{
    struct sigaction sa;
    memset(&sa,'\0',sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig,&sa,NULL) != -1);
}


int main( int argc, char* argv[] )
{
    if( argc <= 2 )
    {
        printf( "parameter is error !");
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );

    // 创建本地socket地址
    int ret = 0;
    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );

    // 创建本地socket
    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 0 );

    // 绑定本地socket
    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    if( ret == -1 )
    {
        printf( "errno is %d\n", errno );
        return 1;
    }

    // 监听本地socket
    ret = listen( listenfd, 5 );
    assert( ret != -1 );

    // 初始化epoll事件表
    epoll_event events[ MAX_EVENT_NUMBER ];
    int epollfd = epoll_create( 5 );
    assert( epollfd != -1 );

    // 注册listenfd上的可读事件
    addfd( epollfd, listenfd );

    // 使用socketpair创建管道
    ret = socketpair(PF_UNIX,SOCK_STREAM,0,pipefd);    
    assert(ret != -1);
    setnonblocking(pipefd[1]);

    // 注册pipefd[0]上的可读事件
    addfd(epollfd,pipefd[0]);

    // 设置一些信号的处理函数
    addsig(SIGHUP);    // 终端挂起
    addsig(SIGCHLD);   // 子进程状态发生变化(退出或暂停)
    addsig(SIGTERM);   // 终止进程，例如kill命令
    addsig(SIGINT);    // 键盘输入中断进程
    bool stop_server = false;

    while(!stop_server)
    {
        int number = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        if((number < 0) && (errno != EINTR))
        {
            printf("epoll failure\n");
        }

        for(int i = 0;i < number;i++)
        {
            int sockfd = events[i].data.fd;
            // 如果就绪文件描述符是listenfd，处理新的连接
            if(sockfd == listenfd)
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                int connfd = accept(listenfd,(struct sockaddr*)&client_address,&client_addrlength);
                addfd(epollfd,connfd);
            }
            // 如果就绪的文件描述符是pipefd[0]，则处理信号
            else if((sockfd == pipefd[0]) && (events[i].events & EPOLLIN))
            {
                int sig;
                char signals[1024];
                ret = recv(pipefd[0],signals,sizeof(signals),0);
                if(ret == -1)
                {
                    continue;
                }
                else if(ret == 0)
                {
                    continue;
                }
                else
                {
                    // 由于每个信号占1字节，所以按字节来逐个接受信号
                    // 以SIGTERM为例，说明如何安全地终止服务器主循环
                    for(int i = 0;i < ret;++i)
                    {
                        switch(signals[i])
                        {
                            case SIGCHLD:
                            case SIGHUP:
                            {
                                continue;
                            }
                            case SIGTERM:
                            case SIGINT:
                            {
                                stop_server = true;
                            }
                        }
                    }
                }
            }
            else
            {
                printf("something happened!");
            }
        }
    }

    // 关闭socket
    printf("close fds\n");
    close(listenfd);
    close(pipefd[1]);
    close(pipefd[0]);
    return 0;
}
