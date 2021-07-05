## 一些Linux中的操作记录
#### 在Linux中两个主机间传输一个文件
- - -
###### 复制文件到另一台Linux主机
~~~cpp
// scp 文件名 用户名@计算机IP或者计算机名称:远程路径

// 实操 ： 将虚拟机下的Linux中的文件发送到远程服务器上
scp /home/swin/.ssh/id_rsa.pub swin@1.15.157.247:/home/swin/MyGit
~~~
- - -