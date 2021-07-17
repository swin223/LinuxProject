## 三次握手中的connect和accept返回
#### 讲解
服务器开启listen，客户端发起connect向服务器发送request，**在服务器端返回ACK前客户端connect阻塞**。
服务器发送ACK，客户端接收ACK，connect就返回。**在服务器发送ACK时未收到客户端第三次ACK前服务器端的accept阻塞。**
这时候的连接只是客户端可以向服务器端进行发送数据。仅当收到第三次ACK时候，accept返回，双方可以进行通讯。
#### 网址讲解参考
https://blog.csdn.net/lvyuan30276/article/details/50458572  
https://blog.csdn.net/yangyiwei524386/article/details/7377120
- - -

## 应用实例
#### 实际情况
~~~c
// 服务器端
$ ./recvMegOob.out 10.0.4.7 8000

// 客户端
$ ./nonBlockConnect.out 1.15.157.247 8000

// 服务器端输出
connect sucess!

// 客户端输出
connection ready after select with the socket : 3
~~~
#### 实例讲解
1. 由于是非阻塞连接，因此没有立即连接，而是利用select进行监听写事件后进行连接。
2. 在select监听时间段内，一旦有事件就会立即返回，这在我运行程序时秒连接有联系。
- - -