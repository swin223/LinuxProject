## 通讯过程中的输出
#### 第一次设置
- - -
###### 服务器端
~~~cpp
// 设置接收缓存区的大小为50
./modRecv.out 10.0.4.7 8000 50
the tcp receive buffer size after setting is 2304
~~~
###### 客户端
~~~cpp
// 设置发送缓存区的大小为2000
./modSend.out 1.15.157.247 8000 2000
the tcp send buffer size after setting is 4608
~~~
###### 服务器端接收
~~~cpp
receive message is aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
~~~
- - -
#### 第二次设置
###### 服务器端
~~~cpp
// 设置接收缓存区的大小为2500
./modRecv.out 10.0.4.7 8000 2500
the tcp receive buffer size after setting is 5000
~~~
###### 客户端
~~~cpp
// 设置发送缓存区的大小为5000
./modSend.out 1.15.157.247 8000 5000
the tcp send buffer size after setting is 10000
~~~
- - -
#### 总结
* 设置发送缓存区大小和接收缓存区大小的时候，如果设置得太小，系统会启动默认值。
* 设置发送缓存区大小和接收缓存区大小的时候，如果设置的值超过默认最小值且为x时，系统会将其设置为2x值。
