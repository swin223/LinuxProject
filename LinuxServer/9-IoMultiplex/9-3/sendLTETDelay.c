#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if(argc <= 3)
    {
        printf("parameters is error!");
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    const char *message = argv[3];
    const char *addMessage = "yesyesyesyesyesyesyes";
    
    // 远程socket地址
    struct sockaddr_in server_address;
    bzero(&server_address,sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&server_address.sin_addr);
    server_address.sin_port = htons(port);

    // 创建本地socket
    int sockfd = socket(PF_INET,SOCK_STREAM,0);
    assert(sockfd >= 0);

    // connect远程socket地址
    if(connect(sockfd,(struct sockaddr*)&server_address,sizeof(server_address)) < 0)
    {
        printf("failed!");
    }
    else
    {
        send(sockfd,message,strlen(message),0);
        // 睡眠3s后再继续发送
        sleep(3);
        send(sockfd,addMessage,strlen(addMessage),0);
    }

    close(sockfd);
    return 0;
}
