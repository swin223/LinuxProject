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

#define UDP_BUFFER_SIZE 1024

int main(int argc,char* argv[])
{
    if(argc <= 2)
    {
        printf("parameter is error!");
        return 1;
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
    
    // 创建UDP socket
    int udpfd = socket(PF_INET,SOCK_DGRAM,0);
    assert(udpfd >= 0);

    // 绑定本地地址
    ret = bind(udpfd,(struct sockaddr*)&address,sizeof(address));
    assert(ret != -1);

    // 收发数据
    char buf[UDP_BUFFER_SIZE];
    memset(buf,'\0',UDP_BUFFER_SIZE);
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);

    ret = recvfrom(udpfd,buf,UDP_BUFFER_SIZE - 1,0,(struct sockaddr*)&client_address,&client_addrlength);
    printf("got %d bytes of data '%s'\n",ret,buf);

    close(udpfd);

    return 0;
}
