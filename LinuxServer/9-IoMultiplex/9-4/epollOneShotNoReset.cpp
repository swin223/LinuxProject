#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 1024

// 定义一个结构体,用于输入线程中
struct fds
{
    int epollfd;
    int sockfd;
};

// 将文件描述符设置为非阻塞的
int setnonblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

// 将文件描述符fd上的EPOLLIN和EPOLLET事件注册到epollfd指示的epoll内核事件表中
// 参数oneshot指定是否注册fd上的EPOLLONESHOT事件
void addfd(int epollfd,int fd,bool oneshot)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if(oneshot)
    {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    setnonblocking(fd);
}

// 重置fd上的事件
// 尽管fd上EPOLLONESHOT事件被注册，但是操作系统仍然会触发fd上的EPOLL事件
void reset_oneshot(int epollfd,int fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}

// 工作线程
void* worker(void* arg)
{
    printf("enter thread function\n");
    int sockfd = ((fds*) arg)->sockfd;
    int epollfd = ((fds*) arg)->epollfd;
    printf("start new thread to receive data on fd:%d\n",sockfd);
    char buf[BUFFER_SIZE];
    memset(buf,'\0',BUFFER_SIZE);

    // 循环读取sockfd上的数据，直到遇到EAGAIN错误
    while(1)
    {
        int ret = recv(sockfd,buf,BUFFER_SIZE - 1,0);
        if(ret == 0)
        {
            close(sockfd);
            printf("foreiner closed the connection\n");
            break;
        }
        else if(ret < 0)
        {
            if(errno == EAGAIN)
            {
                // 在数据读完以后，记得重置EPOLLONESHOT事件
                // 让其他工作线程有机会继续处理这个socket
                // reset_oneshot(epollfd,sockfd);
                printf("read later\n");
                break;
            }
        }
        else
        {
            printf("get content：%s\n",buf);
            // 休眠5s，模拟数据处理过程
            sleep(5);
        }
    }
    printf("end thread receiving data on fd:%d\n",sockfd);
}

int main(int argc,char* argv[])
{
    if(argc <= 2)
    {
        printf("parameter is error!");
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    // 创建本地socket地址
    int ret = 0;
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);

    // 创建监听socket
    int listenfd = socket(PF_INET,SOCK_STREAM,0);
    assert(listenfd >= 0);

    // 绑定本地地址
    ret = bind(listenfd,(struct sockaddr*)&address,sizeof(address));
    assert(ret != -1);

    // 进行监听
    ret = listen(listenfd,5);
    assert(ret != -1);

    // 调用epoll
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    // 注意，监听socket listenfd上不能注册EPOLLONESHOT事件的
    // 否则应用程序只能处理一个客户连接，因为后续的客户连接请求将不再触发listenfd上的EPOLLIN事件
    addfd(epollfd,listenfd,false);

    while(1)
    {
        int ret = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        if(ret < 0)
        {
            printf("epoll failure\n");
            break;
        }

        // 处理每一个事件
        for(int i = 0;i < ret;++i)
        {
            int sockfd = events[i].data.fd;
            if(sockfd == listenfd)
            {
                printf("accept a new socket\n");
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                int connfd = accept(listenfd,(struct sockaddr*)&client_address,&client_addrlength);
                addfd(epollfd,connfd,true);
            }
            else if(events[i].events & EPOLLIN)
            {
                printf("enter read event(init)\n");
                pthread_t thread;
                fds fds_for_new_worker;
                fds_for_new_worker.epollfd = epollfd;
                fds_for_new_worker.sockfd = sockfd;
                // 新启动一个工作线程为sockfd服务
                pthread_create(&thread,NULL,worker,(void *)&fds_for_new_worker);
            }
            else
            {
                printf("something else happened \n");
            }
        }
    }

    close(listenfd);
    return 0;
}









