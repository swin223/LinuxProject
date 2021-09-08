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

int main(int argc,char* argv[])
{
    if(argc <= 2)
    {
        printf("parameter is error!");
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    // 创建服务端socket地址
    int ret = 0;
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);
    
    // 创建UDP socket
    int udpfd = socket(PF_INET,SOCK_DGRAM,0);
    assert(udpfd >= 0);

    // 收发数据
    const char* buf = "yes,i do!\n";
    socklen_t addrlength = sizeof(address);
    sendto(udpfd,buf,strlen(buf),0,(struct sockaddr*)&address,addrlength);
    
    close(udpfd);

    return 0;
}
