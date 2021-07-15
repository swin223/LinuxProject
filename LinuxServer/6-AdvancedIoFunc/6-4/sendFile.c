#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

int main(int argc,char *argv[])
{
    if(argc <= 3)
    {
        printf("parameter is not enough!");
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    const char* file_name = argv[3];

    // 获取文件相关信息
    int filefd = open(file_name,O_RDONLY);
    assert(filefd > 0);
    struct stat stat_buf;
    fstat(filefd,&stat_buf);

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
        // 直接从文件发送到socket
        sendfile(connfd,filefd,NULL,stat_buf.st_size);
        close(connfd);
    }

    close(sock);
    return 0;
}















