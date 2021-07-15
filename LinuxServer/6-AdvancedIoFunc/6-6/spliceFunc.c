#define _GNU_SOURCE 1
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int main(int argc,char *argv[])
{
    if(argc <= 2)
    {
        printf("parameter is not enough!");
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    // 创建本地socket地址
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);

    // 创建本地socket
    int sock = socket(PF_INET,SOCK_STREAM,0);
    assert(sock >= 0);

    // 绑定本地socket
    int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
    assert(ret != -1);

    // 监听socket
    ret = listen(sock,5);
    assert(ret != -1);

    // 接收socket
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept(sock,(struct sockaddr*)&client,&client_addrlength);
    if(connfd < 0) printf("error!");
    else
    {
        int pipefd[2];
        ret = pipe(pipefd);  // 创建管道
        assert(ret != -1);
        // 将connfd上流入客户数据定向到管道中
        ret = splice(connfd,NULL,pipefd[1],NULL,32768, SPLICE_F_MORE | SPLICE_F_MOVE);
        assert(ret != -1);
        // 将管道的输出定向到connfd客户连接文件描述符
        ret = splice(pipefd[0],NULL,connfd,NULL,32768, SPLICE_F_MORE | SPLICE_F_MOVE);
        assert(ret != -1);
        close(connfd);
    }

    close(sock);
    return 0;
}
