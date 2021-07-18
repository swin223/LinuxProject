#define _GNU_SOURCE 1
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
#include <poll.h>

#define USER_LIMIT 5   // 最大用户数量
#define BUFFER_SIZE 64 // 读缓存区的大小
#define FD_LIMIT 65535 // 文件描述符数量限制

// 客户数据
// 客户端socket地址 写到数据端数据的位置 从客户端读入的数据
struct client_data
{
    sockaddr_in address;
    char* write_buf;
    char buf[BUFFER_SIZE];
};

// 将文件描述符设置为非阻塞的
int setnonblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

int main(int argc, char* argv[])
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

    // 创建users数组，分配FD_Limit个client_data对象
    // 每个可能的socket连接都可以获得这样一个对象，socket的值可以直接索引socket连接对应的data对象
    // 这是socket和客户数据关联的简单高效的方式
    client_data* users = new client_data[FD_LIMIT];
    // 尽管我们分配足够多的client_data对象，但是为了提高poll性能，必须有必要限制用户数量
    pollfd fds[USER_LIMIT + 1];

    // 初始化
    int user_counter = 0;
    for(int i = 1;i <= USER_LIMIT;++i)
    {
        fds[i].fd = -1;
        fds[i].events = 0;
    }
    fds[0].fd = listenfd;
    fds[0].events = POLLIN | POLLERR;
    fds[0].revents = 0;

    while(1)
    {
        // 使用poll来监听事件的发生
        ret = poll(fds,user_counter + 1,-1);
        if(ret < 0)
        {
            printf("poll failure\n");
            break;
        }

        // 调用poll时为了得到结果需要轮询
        for(int i = 0;i < user_counter + 1;++i)
        {
            if((fds[i].fd == listenfd)&&(fds[i].revents & POLLIN))
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                int connfd = accept(listenfd,(struct sockaddr*)&client_address,&client_addrlength);
                if(connfd < 0)
                {
                    printf("errno if %d\n",errno);
                    continue;
                }

                // 如果请求太多，则关闭新到的连接
                if(user_counter >= USER_LIMIT)
                {
                    const char* info = "too many users\n";
                    printf("%s",info);                    // 本地打印一条消息
                    send(connfd,info,strlen(info),0);     // 向客户端发送一条消息
                    close(connfd);
                    continue;
                }

                // 接下来就是正常的请求处理流程
                // 对于新连接，需要同时修改fds和users数组
                user_counter++;
                users[connfd].address = client_address;
                setnonblocking(connfd);
                fds[user_counter].fd = connfd;
                fds[user_counter].events = POLLIN | POLLRDHUP | POLLERR;
                fds[user_counter].revents = 0;
                printf("comes a new user,now have %d users\n",user_counter);
            }
            else if(fds[i].revents & POLLERR)
            {




            }
            else if(fds[i].revents & POLLRDHUP)
            {



            }
            else if(fds[i].revents & POLLIN)
            {


            }




        }










    }




































}
