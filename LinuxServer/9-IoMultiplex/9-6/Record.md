## 记录
#### 输出输出
~~~c
// 服务器端
$ ./chatServer.out 10.0.4.7 8000

// 客户端
$ ./chatClient.out 1.15.157.247 8000

// 客户端和服务器端输入输出
// 第一次(无输入)
connection success  // 客户端输出
comes a new user,now have 1 users  // 服务器端输出
// 第二次(有输入)
a123456789  // 客户端输入
send data process...  // 客户端输出
std input pipe bytes size = 11  // 客户端输出
std output socket bytes size = 11  // 客户端输出
get 11 bytes of client data a123456789  from 4  //  服务器端输出
// 第三次
^C  // 客户端关闭
a client left  //服务器端输出
~~~
#### 代码分析
1. 客户端发起connect连接成功，返回connection success。服务器端注册了监听socket监听事件，
所以poll得到事件后，输出comes a new user,now have 1 users。
2. 客户端标准输入有事件，数据在管道中穿梭，**代码中的32768表示最大可传输数据值**，
实际上传输的数据由splice函数返回。服务器端注册了POLLIN事件所以能够输出。
3. 得到客户端关闭后，输出a client left。

