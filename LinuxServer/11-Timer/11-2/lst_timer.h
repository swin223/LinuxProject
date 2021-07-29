#ifndef LST_TIMER
#define LST_TIMER

#include <time.h>
#include <netinet/in.h>
#include <stdio.h>

#define BUFFER_SIZE 64

class util_timer;    // 定时器类的前向声明

// 用户数据结构
struct client_data
{
    // 客户端socket地址
    sockaddr_in address;
    // 客户端连接的socket文件描述符
    int sockfd;
    // 读缓存
    char buf[BUFFER_SIZE];
    // 定时器
    util_timer* timer;
};

// 定时器类
class util_timer
{
public:
    util_timer() : prev(NULL),next(NULL){}

public:
    time_t expire;                  // 任务超时的时间(绝对时间)
    void (*cb_func)(client_data*);  // 任务回调函数
    // 回调函数处理的客户数据，由定时器的执行者传递给回调函数
    client_data* user_data;
    util_timer* prev;               // 指向前一个定时器
    util_timer* next;               // 指向后一个定时器
};

// 定时器链表
// 它是一个升序、双向的链表，带有头和尾节点
class sort_timer_lst
{
public:
    sort_timer_lst() : head(NULL),tail(NULL) {}

    // 链表被销毁的时候，删除其中所有的定时器
    ~sort_timer_lst()
    {
        util_timer* tmp = head;
        while(tmp)
        {
            head = tmp->next;
            delete tmp;
            tmp = head;
        }
    }

    // 将目标定时器timer添加到链表中
    void add_timer(util_timer* timer)
    {
        // 添加的timer为NULL
        if(!timer)
        {
            return;
        }
        // 当前链表中并没有任何值
        if(!head)
        {
            head = tail = timer;
            return;
        }
        // 如果目标定时器的超时时间小于当前链表中所有定时器的超时时间，则把该定时器插入链表头部
        // 则否就需要调用重载函数add_timer把它插入链表中合适的位置，保证链表的升序特性
        if(timer->expire < head->expire)
        {
            timer->next = head;
            head->prev = timer;
            head = timer;
            return;
        }
        add_timer(timer,head);
    }

    // 当某个定时任务发生变化时，调整对应的定时器在链表中的位置。
    // 这个函数只考虑被调整的定时器的超时时间延长的情况，即该定时器需要往链表的尾部移动
    void adjust_timer(util_timer* timer)
    {
        if(!timer)
        {
            return;
        }
        util_timer* tmp = timer->next;
        // 如果被调整的目标定时器在链表尾部，或者该定时器的新的超时值仍然小于其下一个定时器的超时值，则不用调整
        if(!tmp || (timer->expire < tmp->expire))
        {
            return;
        }
        // 如果目标定时器是链表头节点，取出重新插入链表
        if(timer == head)
        {
            head = head->next;
            head->prev = nullptr;
            timer->next = nullptr;
            add_timer(timer,head);
        }
        // 现在的情况就是普通节点
        // 从链表中取出，插入其原来所在位置之后的链表中
        else
        {
            timer->prev->next = timer->next;
            timer->next->prev = timer->prev;
            add_timer(timer,timer->next);
        }
    }

    // 将目标定时器timer从链表中剔除
    void del_timer(util_timer* timer)
    {
        if(!timer)
        {
            return;
        }
        // 下面条件成立时表示链表中只有一个目标定时器
        if((timer == head) && (timer == tail))
        {
            delete timer;
            head = nullptr;
            tail = nullptr;
            return;
        }
        // 如果链表中至少有两个定时器，且目标定时器是链表头结点
        // 将链表头结点重置为原节点下一个节点，然后删除目标定时器
        if(timer == head)
        {
            head = head->next;
            head->prev = nullptr;
            delete timer;
            return;
        }
        // 如果链表中至少有两个定时器，且目标定时器是链表的尾结点
        // 将链表尾结点重置为原尾节点的前一个节点，然后删除目标定时器
        if(timer == tail)
        {
            tail = tail->prev;
            tail->next = nullptr;
            delete timer;
            return;
        }
        // 如果目标定时器位于链表中间，则把它前后的定时器串联起来，然后删除目标定时器
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        delete timer;
    }

    // SIGALRM信号每次被触发就在其信号处理函数(主函数)中执行一次tick函数，以处理链表上到期的任务
    void tick()
    {
        if(!head)
        {
            return;
        }
        printf("timer tick\n");
        time_t cur = time(NULL);   // 获得系统当前的时间
        util_timer* tmp = head;
        // 从头结点开始一次处理每个定时器，直到遇到一个尚未到期的定时器，这就是定时器的核心逻辑
        while(tmp)
        {
            // 因为每个定时器都是用绝对时间作为超时值，所以我们可以把定时器的超时值和系统当前时间，比较以判断定时器是否到期
            if(cur < tmp->expire)
            {
                break;
            }
            // 调用定时器的回调函数，执行定时任务
            tmp->cb_func(tmp->user_data);
            // 执行完定时器中的定时任务之后，就将它从链表中删除，重置链表头节点
            head = tmp->next;
            if(head)
            {
                head->prev = nullptr;
            }
            delete tmp;
            tmp = head;
        }
    }
    

private:
    // 重载的辅助函数，被public下的add_timer函数和adjust_timer函数调用
    // 该函数表示将目标定时器timer添加到节点lst_head之后的部分链表中
    void add_timer(util_timer* timer,util_timer* lst_head)
    {
        util_timer* prev = lst_head;
        util_timer* tmp = prev->next;
        // 遍历lst_head节点之后的部分链表，直到找到一个超时时间大于目标定时器的超时时间的节点，将其插入到该节点之前
        while(tmp)
        {
            // 找到
            if(timer->expire < tmp->expire)
            {
                prev->next = timer;
                timer->next = tmp;
                tmp->prev = timer;
                timer->prev = prev;
                break;
            }
            prev = tmp;
            tmp = tmp->next;
        }
        // 遍历完lst_head节点之后的部分链表，仍未找到超时时间大于目标定时器的超时时间的节点，则将目标定时器插入链表尾部，并设置为新的尾节点
        if(!tmp)
        {
            prev->next = timer;
            timer->prev = prev;
            timer->next = nullptr;
            tail = timer;
        }
    }

private:
    util_timer* head;
    util_timer* tail;
};

#endif
