## 编译时遇到的问题
pthread 库不是 Linux 系统默认的库，连接时需要使用静态库 libpthread.a
~~~c
// 在编译中要加 -lpthread参数
// -l (L的小写)：表示需要链接库的名称，注意不是库文件名称，比如库文件为 libtest.so，那么库名称为test
g++ -g main.c -o main.out -lpthread
~~~
- - -

## 第一种情况 正常代码
#### 接收情况
~~~c
// 服务器端
$ ./epollOneShot.out 10.0.4.7 8000

// 客户端
// 客户端会有延迟，会发2次(中间间隔10s)
$ ./sendLTETDelay.out 1.15.157.248 8000 abcdefg123456789

// 服务器端接收
// 分4次接收，每次接收中途有延迟
// 第一次
accept a new socket
enter read event(init)
enter thread function
start new thread to receive data on fd:5
get content：abcdefg123456789
// 第二次 - 因为要等待5s
read later
end thread receiving data on fd:5
// 第三次 - 第二次发送数据
enter read event(init)
enter thread function
start new thread to receive data on fd:5
get content：yesyesyesyesyesyesyes
// 第四次 - 在等待5秒的过程中对端已经关闭，将时间写到socket中
foreiner closed the connection
end thread receiving data on fd:5
~~~
#### 接收分析
1. 客户端发起connect连接后，服务器端listenfd有[读事件]，accept一个新的socket。
2. 客户端发送了第一次数据，服务器端进入线程函数得到数据。
3. 等待5秒处理数据，在while(1)下继续读数据，发现读完后退出。
4. 客户端第二次发送数据，再次触发读数据等。
5. 在等待5s的时候，对端关闭，此时不再重新进入线程，直接读。
- - -

## 第二种情况 没有reset_oneshot
#### 接收情况
~~~c
// 服务器端
$ ./epollOneShotNoReset.out 10.0.4.7 8000

// 客户端
// 客户端会有延迟，会发2次(中间间隔10s)
$ ./sendLTETDelay.out 1.15.157.248 8000 abcdefg123456789

// 服务器端接收
// 分2次接收，每次接收中途有延迟
// 第一次
accept a new socket
enter read event(init)
enter thread function
start new thread to receive data on fd:5
get content：abcdefg123456789
// 第二次
read later
end thread receiving data on fd:5
~~~
#### 接收分析
1. 客户端发起connect连接后，服务器端listenfd有[读事件]，accept一个新的socket。
2. 客户端发送了第一次数据，服务器端进入线程函数得到数据。
3. 等待5秒处理数据，在while(1)下继续读数据，发现读完后退出。
4. 因为没有重新设置oneshot，**oneshot模式在没有重新设置情况下对该类事件只会触发一次**
- - -

## 第二种情况的拓展 设置监听socket为oneshot
#### 接收情况
~~~c
// 服务器端
$ ./epollOneShotListen.out 10.0.4.7 8000

// 客户端
// 客户端会有延迟，会发2次(中间间隔10s)
$ ./sendLTETDelay.out 1.15.157.248 8000 abcdefg123456789
$ ./sendLTETDelay.out 1.15.157.248 8000 abcdefg123456789

// 服务器端接收
// 分4次接收，每次接收中途有延迟
// 第一次
accept a new socket
enter read event(init)
enter thread function
start new thread to receive data on fd:5
get content：abcdefg123456789
// 第二次 - 因为要等待5s
read later
end thread receiving data on fd:5
// 第三次 - 第二次发送数据
enter read event(init)
enter thread function
start new thread to receive data on fd:5
get content：yesyesyesyesyesyesyes
// 第四次 - 在等待5秒的过程中对端已经关闭，将时间写到socket中
foreiner closed the connection
end thread receiving data on fd:5
~~~
#### 接收分析
1. 其他的同第一种情况
2. 监听listen socket由于被设置了oneshot而没有被reset导致其只能接收一个socket，不能接收更多的了。
- - -

## 第三种情况 在处理时间段内仍有数据传入
#### 接收情况
~~~c
// 服务器端
$ ./epollOneShotContinue.out 10.0.4.7 8000

// 客户端
// 客户端会有延迟，会发2次(中间间隔10s)
$ ./sendLTETDelay.out 1.15.157.248 8000 abcdefg123456789

// 服务器端接收
// 分3次接收，每次接收中途有延迟
// 第一次
accept a new socket
enter read event(init)
enter thread function
start new thread to receive data on fd:5
get content：abcdefg123456789
// 第二次 - 中间有15s数据处理时间
get content：yesyesyesyesyesyesyes
// 第三次 - 中间有15s数据处理时间
foreiner closed the connection
end thread receiving data on fd:5
~~~
#### 接收分析
1. 客户端发起connect连接后，服务器端listenfd有[读事件]，accept一个新的socket。
2. 客户端发送了第一次数据，服务器端进入线程函数得到数据。
3. 在数据处理阶段，socket缓存区仍有数据进入，继续处理。
4. 处理数据时，又等待了15s，发现对端已经关闭，直接关闭连接。
- - -

## 总结
1. 在设置一个事件为OneShot的时候意味着此sockfd的该类事件在接下来**没有重设置之前**最多触发一种事件，
且**最多触发一次**。且触发的事件在处理后若没有及时resetOneShot，那么之后就不会继续处理fd的该类事件了。
包括监听socket，如果被设置为OneShot之后没有reset，就只会连接一次，以后忽略该类型事件。
所以要及时重新设置。
2. 在处理数据，比如读取socket中数据的时候，刚好该socket有数据流入，那么就继续读取。
3. oneshot专门用于线程的处理。
- - -



