#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if(argc <= 2)
    {
        printf("parameters is not enough!");
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

    // 创建本地监听socket
    int sock = socket(PF_INET,SOCK_STREAM,0);
    assert(sock >= 0);

    // 在listen之前 - 创建监听socket时候就能修改相应值
    int recvbuf = atoi(argv[3]);
    int len = sizeof(recvbuf);
    // 先设置监听socket的缓存区大小，然后进行读取
    setsockopt(sock,SOL_SOCKET,SO_RCVBUF,&recvbuf,sizeof(recvbuf));
    getsockopt(sock,SOL_SOCKET,SO_RCVBUF,&recvbuf,(socklen_t*)&len);
    printf("the tcp receive buffer size after setting is %d\n",recvbuf);

    // 进行地址绑定
    int ret = bind(sock, (struct sockaddr*)&address,sizeof(address));
    assert(ret != -1);

    // 进行监听
    ret = listen(sock,5);
    assert(ret != -1);

    // 进行连接
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept(sock,(struct sockaddr *)&client,&client_addrlength);
    if(connfd < 0)
    {
        printf("error!");
    }
    else
    {
        char buffer[BUFFER_SIZE];
        memset(buffer,'\0',BUFFER_SIZE);
        // 直到接受到数据为止
        while(recv(connfd,buffer,BUFFER_SIZE - 1,0) > 0){}
        close(connfd);
        printf("receive message is %s\n",buffer);
    }

    close(sock);
    return 0;
}

