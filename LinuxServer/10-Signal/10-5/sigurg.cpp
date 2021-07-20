#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#define BUF_SIZE 1024

static int connfd;

// SIGURG信号处理函数
void sig_urg(int sig)
{
    // 保证可重入
    int save_errno = errno;
    char buffer[BUF_SIZE];
    memset(buffer,'\0',BUF_SIZE);
    // 接收带外数据
    int ret = recv(connfd,buffer,BUF_SIZE - 1,MSG_OOB);
    printf("got %d bytes of oob data '%s'\n",ret,buffer);
    errno = save_errno;
}

void addsig(int sig,void (*sig_handler)(int))
{
    struct sigaction sa;
    memset(&sa,'\0',sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig,&sa,NULL) != -1);
}

int main(int argc,char* argv[])
{
    if( argc <= 2 )
    {
        printf( "parameter is error !");
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );

    // 创建本地socket地址
    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );

    // 创建本地socket
    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 0 );

    // 绑定本地socket
    int ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    assert(ret != -1);

    // 监听本地socket
    ret = listen( listenfd, 5 );
    assert( ret != -1 );

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    connfd = accept(listenfd,(struct sockaddr*)&client,&client_addrlength);
    if(connfd < 0)
    {
        printf("errno is %d\n",errno);
    }
    else
    {
        addsig(SIGURG,sig_urg);
        // connfd会接收到Normal和Oob数据，接收到Oob数据时触发SIGURG信号
        // 触发SIGURG信号时需要通知相应的进程，所以需要设定
        // 在使用SIGURG信号之前，我们必须设置socket的宿主进程或进程组
        fcntl(connfd,F_SETOWN,getpid());

        char buffer[BUF_SIZE];
        // 循环接收普通数据
        while(1)
        {
            memset(buffer,'\0',BUF_SIZE);
            // recv是会进行阻塞的，如果没有数据可以接受的话
            ret = recv(connfd,buffer,BUF_SIZE - 1,0);
            if(ret <= 0)
            {
                break;
            }
            printf("got %d bytes of normal data '%s'\n",ret,buffer);
        }
        close(connfd);
    }

    close(listenfd);
    return 0;
}
