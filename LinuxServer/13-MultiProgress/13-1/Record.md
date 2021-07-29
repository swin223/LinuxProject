## fork函数调用记录
#### fork的简单调用
~~~c
$ ./fork.out 
pid is 3297
start fork!
------parent------
i am parent process pid = 3297,ppid = 32138,fork return = 3298
------process ending------
this process pid = 3297
------child------
i am child process pid = 3298,ppid = 3297,fork return = 0
------process ending------
this process pid = 3298
~~~
###### 结果分析
1. 开始获得的是运行进程的pid(3297)，且输出start fork时没有进行fork
2. 调用fork之后，分叉运行，各自运行各自的。
3. 父进程运行系列输出，fork返回子进程的pid = 3298
4. 子进程运行系列输出，fork返回pid = 0
- - -

#### fork的简单调用(多行输出)
~~~c
$ ./forkPrintTest.out 
pid is 5386
start fork!
------parent------
i am parent process pid = 5386,ppid = 32138,fork return = 5387
------parent--1---
------parent--2---
------parent--3---
------parent--4---
------parent--5---
------parent--6---
------parent--7---
------parent--8---
------parent--9---
------parent--10---
------process ending------
this process pid = 5386
------child------
i am child process pid = 5387,ppid = 5386,fork return = 0
------child---1--
------child---2--
------child---3--
------child---4--
------child---5--
------child---6--
------child---7--
------child---8--
------child---9--
------child---10--
------process ending------
this process pid = 5387
~~~
###### 结果分析
1. 大概是一个时间片内执行了父进程，之后再执行子进程的，进行输出。
- - -