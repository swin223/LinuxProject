#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
{
    if(argc <= 2)
    {
        printf("parameters is not enough!");
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    // 创建服务器端socket地址
    struct sockaddr_in server_address;
    bzero(&server_address,sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&server_address.sin_addr);
    server_address.sin_port = htons(port);

    // 创建本地socket - 客户端socket
    int sock = socket(PF_INET,SOCK_STREAM,0);
    assert(sock >= 0);

    // 在connect之前设置相应的缓存区大小
    int sendbuf = atoi(argv[3]);
    int len = sizeof(sendbuf);
    // 先设置TCP缓存区大小，然后进行读取
    setsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sendbuf,sizeof(sendbuf));
    getsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sendbuf,(socklen_t *)&len);
    printf("the tcp send buffer size after setting is %d\n",sendbuf);

    // 进行与服务器端连接
    if(connect(sock,(struct sockaddr *)&server_address,sizeof(server_address)) != -1)
    {
        char buffer[BUFFER_SIZE];
        memset(buffer,'a',BUFFER_SIZE);
        send(sock,buffer,BUFFER_SIZE,0);
    }

    close(sock);
    return 0;
}

