## 第一种情况 - 服务器用LT方式接收
#### 过程
~~~c
// 服务器端
$ ./epollLt.out 10.0.4.7 8000

// 客户端
$ ./sendLTET.out 1.15.157.247 8000 abcdefghijklmnopqrstuvwxyz

// 服务器端接收
enter it function and number = 1
enter socket accept
enter it function and number = 1
event trigger once
get 9 bytes of content: abcdefghi
enter it function and number = 1
event trigger once
get 9 bytes of content: jklmnopqr
enter it function and number = 1
event trigger once
get 8 bytes of content: stuvwxyz
enter it function and number = 1
event trigger once
exit
~~~
#### 解析
1. 由于服务器注册了监听socket的读事件，因此会有[连接事件]。
2. 当客户端发起连接后，服务器端监听socket[连接事件]触发，lt函数执行，本次仅是accept。
3. 当客户端发送长串数据后，服务器端连接socket[读事件]触发，第一次触发，触发结束后继续epoll_wait。
4. 由于socket仍有[读事件]，再次触发，第二次触发，触发结束后继续epoll_wait。
5. 由于socket还有[读事件]，再次触发，第三次触发，触发结束后继续epoll_wait。
6. 客户端发送数据完毕，断开连接，触发[读事件]，revc返回0，关闭服务器的connfd。
#### 总结
针对LT触发方式，对于到来的事件，应用程序可以处理(accept、recv)一部分，或者不处理，
在程序下一次调用epoll_wait的时，还会告诉应用程序有此事件，**直到该事件被处理为止**。
- - -

## 第二种情况 - 服务器用ET方式接收
#### 过程
~~~c
// 服务器端
$ ./epollEt.out 10.0.4.7 8000

// 客户端
$ ./sendLTET.out 1.15.157.247 8000 abcdefghijklmnopqrstuvwxyz

// 服务器端接收
enter et function and number = 1
enter socket accept
enter et function and number = 1
event trigger once
get 9 bytes of content: abcdefghi
get 9 bytes of content: jklmnopqr
get 8 bytes of content: stuvwxyz
read later
enter et function and number = 1
event trigger once
exit
~~~
#### 解析
1. 由于服务器注册了监听socket的读事件，因此会有[连接事件]。
2. 当客户端发起连接后，服务器端监听socket[连接事件]触发，et函数执行，本次仅是accept。
3. 当客户端发送长串数据后，服务器端连接socket[读事件]触发，代码要求一次性要输出完。
4. 客户端发送数据完毕，断开连接，触发[读事件]，revc返回0，关闭服务器的connfd。
#### 总结
和接下来的情况一起总结
- - -

## 第三种情况 - 服务器用ET方式接收，仅仅接收一次
#### 过程
~~~c
// 服务器端
$ ./epollEtSingle.out 10.0.4.7 8000

// 客户端
$ ./sendLTET.out 1.15.157.247 8000 abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789

// 服务器端接收
enter et function and number = 1
enter socket accept
enter et function and number = 1
event trigger once
get 9 bytes of content: abcdefghi
enter et function and number = 1
event trigger once
get 9 bytes of content: jklmnopqr

// 如果是使用第二种情况接收
enter et function and number = 1
enter socket accept
enter et function and number = 1
event trigger once
get 9 bytes of content: abcdefghi
get 9 bytes of content: jklmnopqr
get 9 bytes of content: stuvwxyz1
get 9 bytes of content: 234567891
get 9 bytes of content: 234567891
get 9 bytes of content: 234567891
get 9 bytes of content: 234567891
get 8 bytes of content: 23456789
enter et function and number = 1
event trigger once
exit
~~~
#### 解析
1. 由于服务器注册了监听socket的读事件，因此会有[连接事件]。
2. 当客户端发起连接后，服务器端监听socket[连接事件]触发，et函数执行，本次仅是accept。
3. 当客户端发送长串数据后，服务器端连接socket[读事件]触发，读取部分数据。
4. 3步骤中读取完部分数据后，不管你有没有读全，这个事件算是被处理过了，就遗忘了，**接下去的事件列表中就无了**。
#### 总结
和接下来的情况一起总结
- - -

## 第四种情况 - 服务器用ET方式接收，仅仅接收一次，延时N秒后继续接收
#### 过程
~~~c
// 服务器端
$ ./epollEtSingle.out 10.0.4.7 8000

// 客户端
$ ./sendLTET.out 1.15.157.247 8000 abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789

// 服务器端接收
swin@VM-4-7-ubuntu:~/LinuxServer/9-IoMultiplex/9-3$ ./epollEtSingle.out 10.0.4.7 8000
enter et function and number = 1
enter socket accept
enter et function and number = 1
event trigger once
get 9 bytes of content: abcdefg12
enter et function and number = 1
event trigger once
get 9 bytes of content: 3456789ye
enter et function and number = 1
event trigger once
get 9 bytes of content: syesyesye
~~~
#### 总结
1. 对于ET触发方式，系统仅仅会通知1-2次，在1-2次中进行相应的处理，但是该socket的缓存区不进行清空。
2. 但是如果后续还有数据到来，，还是会触发[读事件]，还是会从缓存区中从前往后再触发1-2次读操作。
3. 比起LT，ET有且仅会触发1-2次，LT是只要存在数据，次次能触发。
- - -


