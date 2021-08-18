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
#include <signal.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define USER_LIMIT 5
#define BUFFER_SIZE 1024
#define FD_LIMIT 65535
#define MAX_EVENT_NUMBE 1024
#define PROCESS_LIMIT 65535

// 一个客户连接必要的数据
struct client_data
{
    sockaddr_in address;    // 客户端socket地址
    int connfd;             // socket文件描述符
    pid_t pid;              // 处理这个连接的子进程的PID
    int pipefd[2];          // 和父进程进行通信用的管道
};

static const char* shm_name = "/my_shm";
int sig_pipefd[2];
int epollfd;
int listenfd;
int shmfd;
char* share_mem = 0;
// 客户连接数组。进程用客户连接编号来索引这个数组，即可取得相关的客户连接数据
client_data* users = 0;

