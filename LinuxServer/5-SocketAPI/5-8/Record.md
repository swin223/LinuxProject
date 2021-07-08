## 通讯过程中遇到的问题
#### 服务器端
* 这里的服务器指的是远端服务器
* 服务器的bind只能绑定本地的ip，使用ifconfig找到本地ip后进行绑定，远程就可以连进来
~~~cpp
// 需要首先使用ifconfig查看本地的网络端口
ifconfig
/* 输出的信息
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 10.0.4.7  netmask 255.255.252.0  broadcast 10.0.7.255
        inet6 fe80::5054:ff:fe87:918e  prefixlen 64  scopeid 0x20<link>
        ether 52:54:00:87:91:8e  txqueuelen 1000  (Ethernet)
        RX packets 18385113  bytes 2908936280 (2.9 GB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 18192352  bytes 3399771622 (3.3 GB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 788592  bytes 169138574 (169.1 MB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 788592  bytes 169138574 (169.1 MB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
*/

./recvMegOob.out 10.0.4.7 8000
~~~
#### 客户端
* 这里的客户端指的是本地的虚拟机下的Linux
* 客户端connect可以连接远程/本地的ip地址
~~~cpp
./sendMsgOob.out 1.15.157.247 8000
~~~

#### 服务器端的接收情况
~~~cpp
got 3 bytes of normal data '123'
got -1 bytes of oob data ''
got 6 bytes of normal data 'abc123'
~~~