## 正常情况下 - connect有10s的计时
#### 输出
~~~c
// 服务器端
$ ./recvMegNormal.out 10.0.4.7 8000

// 客户端
$ ./SoSndtimeo.out 1.15.157.247 8000

// 客户端输出(立马输出)
connect sucess!
// 服务器端输出(立马输出)
connect sucess!
~~~
#### 分析
1. 尽管有10s的计时器事件，但是一旦能connect就立马connect，不会等待。
2. connect是send类型，表示在10s内没有send的话会出现计时器事件。

## 情况1 - accept在connect发起后延时15s
#### 输出
~~~c
// 服务器端
$ ./recvMegTimeOut.out 10.0.4.7 8000

// 客户端
$ ./SoSndtimeo.out 1.15.157.247 8000

// 客户端输出(立马输出)
connect sucess!
// 服务器端输出(延时15s)
connect sucess!
~~~
#### 分析
1. 服务器端的listenfd具有连接功能，可以完成二次握手，那么connect就能返回。**这一点验证了9.5的Record**
2. 第三次握手需要accept，然后就建立双端通讯了。

## 情况2 - 在listen之前延时15s
#### 输出
~~~c
// 服务器端
$ ./recvBeforeListen.out 10.0.4.7 8000

// 客户端
$ ./SoSndtimeo.out 1.15.157.247 8000

// 客户端输出
error occur when connecting to server
~~~
#### 分析
1. 因为服务器端没有进行监听，没有指定的socket，所以客户端肯定会返回错误。

## 总结
1. connect是发送事件，如果是接受事件的话，对于情况2应该等待15s。