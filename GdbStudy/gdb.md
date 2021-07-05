## 简介
GDB是GNU工具链中的调试软件，能够用于调试C、C++、java、go等程序编写的各种程序。

## 资料来源
https://zhuanlan.zhihu.com/p/74897601

## 启动调试
#### 前提
启动调试的前提是程序是否带有调试信息。一般我们在编译的时候要加`-g`参数。
#### 查看是否带有调试信息
- - -
###### 方案1
使用gdb文件查看
* 如果没有调试信息，会提示Reading symbols from nodebug...(no debugging symbols found)...done
* 如果有调试信息，会提示Reading symbols from debug...done

~~~cpp
// 使用g++不带调试参数编译
g++ test.cpp -o nodebug
./nodebug
gdb nodebug
// 此时终端最后一行会输出 Reading symbols from nodebug...(no debugging symbols found)...done.

// 使用g++带调试参数编译
g++ -g test.cpp -o debug
./debug
gdb debug
// 此时终端最后一行会输出 Reading symbols from debug...done.
~~~
- - -
###### 方案2
使用readelf查看可执行文件的段信息
* 如果该可执行文件带有调试信息，段信息会比没有带调试信息的可执行文件多出5个，且名字上都有debug字样。
~~~cpp
// -S 表示显示节头信息
readelf -S nodebug | grep debug
// 显示无信息

readelf -S debug | grep debug
/* 会显示
  [26] .debug_aranges    PROGBITS         0000000000000000  00001039
  [27] .debug_info       PROGBITS         0000000000000000  00001069
  [28] .debug_abbrev     PROGBITS         0000000000000000  00003916
  [29] .debug_line       PROGBITS         0000000000000000  00003edb
  [30] .debug_str        PROGBITS         0000000000000000  000042c2
*/
~~~
- - -
###### 方案3
使用file命令，Linux中file可以查看文件的类型信息。
* 如果显示有debug_info，说明是带有调试信息的。
~~~cpp
// 使用file命令来实现cpp文件的类型信息
file test.cpp
// 输出 test.cpp: C++ source, ASCII text

// 使用file命令查看nodebug和debug两个可执行文件的类型信息
file nodebug
// 输出 ...6104c453d8e8b23d8, not stripped
file debug
// 输出 ...5342d934, with debug_info, not stripped
~~~
- - -
#### 以调试的方式运行程序
程序未启动，可有多种方式启动调试。
无论有无参数，首先使用gdb打开可执行文件
* 若无参数 直接run即可
* 若有参数 run的时候加参数
- - -
###### 调试无参数程序
启动时无需带参数，直接run即可
~~~cpp
gdb debug
(gdb) run
/* 输出
Starting program: /home/swin/gdbStudy/debug 
process id = 1
process id = 2
process id = 3
process id = 4
process id = 5
50
*/
~~~
- - -
###### 调试带参数程序
启动时需带参数，在run的后面加上指定的参数
~~~cpp
gdb debugPara
(gdb) run hello swin
/* 输出
Starting program: /home/swin/gdbStudy/debugPara hello swin
extern message
hello
swin
process id = 1
process id = 2
process id = 3
process id = 4
process id = 5
50
*/
~~~
- - -
###### 调试core文件
<font color='red'> 此部分没有实际用到过，暂定。 </font>
- - -
#### 调试已运行程序
<font color='red'> 此部分没有实际用到过，暂定。 </font>

## 断点设置
#### 为什么要设置断点
在代码中设置断点，程序运行到指定的断点的时候将会"暂停",此时我们可以看一些中间变量内容和堆栈情况，
以帮助我们进行调试程序。
#### 查看已设置的断点
列出所有已设置的断点，每个断点都会有一个标号。
~~~cpp
(gdb) info breakpoints
/* 输出
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000000000000fae in main() at breakPoint.cpp:36
2       breakpoint     keep y   0x000000000000109e in main() at breakPoint.cpp:39
3       breakpoint     keep y   0x0000000000000d8f in printMsg(int) at breakPoint.cpp:13
*/
~~~
#### 断点设置
断点设置的方式有很多种，分别应用于不同的场景。
- - -
###### 根据行号设置断点
将断点打到指定位置
* break可以简写为b
~~~cpp
(gdb) b 行号
(gdb) b name.cpp:行号
~~~
- - -
###### 根据函数名设置断点
可以将断点打到该函数下的第一行代码位置
~~~cpp
(gdb) b 函数名
~~~
- - -
###### 根据条件设置断点
程序在某行中，某个变量出现指定的值时，程序断住。
* 可以使用condition指令来修改断点产生的条件
~~~cpp
(gdb) b name.cpp:行号 if 变量名==指定值

// 使用condition来修改断点产生的条件
(gdb) condition 断点号(条件设置的) 变量名==指定值

// 演示
(gdb) b breakPoint.cpp:10 if a==5
/* 会产生断点号
5       breakpoint     keep y   0x0000000000000d58 in printMsg(int) at breakPoint.cpp:10
        stop only if a==5
*/
(gdb) condition 5 a==6
/* 会产生断点号
5       breakpoint     keep y   0x0000000000000d58 in printMsg(int) at breakPoint.cpp:10
        stop only if a==6
*/
~~~
- - -
###### 根据规则设置断点
<font color='red'> 此部分没有实际用到过，暂定。 </font>
- - -

###### 设置临时断点
假设某个断点只想生效一次，可以产生的一个临时断点，该断点之后就不复存在了
~~~cpp
// 产生临时断点
(gdb) tbreak name.cpp:行号

/* 与其他断点不同的是 disp是del,表示只用于一次，其他都是keep表示再次运行时仍然程序暂停
6       breakpoint     keep y   0x00000000000010c6 in main() at breakPoint.cpp:40
7       breakpoint     del  y   0x000000000000109e in main() at breakPoint.cpp:39
*/
~~~
- - -
###### 跳过多次设置断点
虽然在某处设置了断点，但是想跳过前N次。
* 若某个地方已知前N次不可能出错，那么可以用这个指令来设置
~~~cpp
(gdb) ignore 断点号 想忽略的次数N

/* 之前
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000000000000fae in main() at breakPoint.cpp:36
*/
(gdb) ignore 1 1
/* 之后
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000000000000fae in main() at breakPoint.cpp:36
        ignore next 1 hits
*/
~~~
- - -
###### 根据表达式值变化产生断点
<font color='red'> 此部分没有实际用到过，暂定。 </font>
- - -

#### 禁用或启动断点
某些断点暂时不想使用,但是又不想删除，可以暂时进行禁用，待想使用时进行启用。
~~~cpp
/* 禁用/启动时Enb会发生改变
Num     Type           Disp Enb Address            What
2       breakpoint     keep y   0x000000000000109e in main() at breakPoint.cpp:39

Num     Type           Disp Enb Address            What
2       breakpoint     keep n   0x000000000000109e in main() at breakPoint.cpp:39
*/

(gdb) disable          // 禁用一切
(gdb) disable 断点号   // 禁用指定断点
(gdb) enable           // 启动一切
(gdb) enable 断点号    // 启动指定断点
~~~
#### 断点清除
断点清除主要使用达到的命令是clear和break
* 实际上一般最好指定cpp文件中的断点，以免出错
~~~cpp
// 参照代码中断点删除
clear 函数名           // 删除函数名处的断点
clear name.cpp:函数名  // 删除cpp文件中函数处的断点
clear 行号             // 删除指定行号的断点
clear name.cpp:行号    // 删除cpp文件中指定行的断点

// 根据断点号中的断点删除
delete 断点号          // 删除指定断点号
delete                 // 删除所有断点
~~~

## 变量查看
#### 为什么要变量查看
使用GDB调试工具最关键的一点就是中间变量的查看,看运行结果是否符合预期。
#### 打印基本类型变量/数组/字符数组
* print指令可以简写成p,用于打印变量内容
* 注意输出中间变量是一定是在断点处停住才能输出中间变量
* 只有断点之前的代码才属于运行完
* 有的时候多个函数/文件中会有同一变量名，可以指定对应的函数名/文件名来加以区分
~~~cpp
/* 断点打在第九行
6           int a = 10;
7           int b[] = {1,2,3,4};
8           char c[] = "Hello,Swin";
(b)9           int *d = (int *)malloc(a*sizeof(int));
*/

// 指令与输出
(gdb) p a
$1 = 10
(gdb) p b
$2 = {1, 2, 3, 4}
(gdb) p c
$3 = "Hello,Swin"
(gdb) p d
$4 = (int *) 0x55555555487d <__libc_csu_init+77>

// 区分在不用函数/文件下的变量
(gdb) p '文件名/函数名'::变量名
~~~
#### 打印指针指向内容
* 直接打印指针变量值，打印出来的是指针地址
* 若要想打印指针指向的内容，需要解引用，若是指针指向的数组则只能打印出第一个值
* 指针指向的数组要打印多个值，需要@后面加上长度/变量值
* 可定义一个Unix环境变量，来多次输出值
* 可以使用下标去进行打印
~~~cpp
// 直接打印指针变量值
(gdb) p d 
$15 = (int *) 0x555555756260
// 指针指向的数组则只能打印出第一个值
(gdb) p *d
$16 = 0
// 指针指向的数组要打印多个值，需要@后面加上长度/变量值
(gdb) p *d@a
$17 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
(gdb) p *d@10
$18 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
// 定义一个Unix环境变量，来多次输出值
(gdb) set $index = 0
(gdb) p b[$index++]
$19 = 1
(gdb) p b[$index++]
$20 = 2
(gdb) p b[$index++]
$21 = 3
(gdb) p b[$index++]
$22 = 4
(gdb) p b[$index++] // 超出数组范围了
$23 = 1431652400
// 可以使用下标去进行打印
(gdb) p b[0]
$31 = 1
(gdb) p b[1]
$32 = 2
(gdb) p b[2]
$33 = 3
(gdb) p b[3]
$34 = 4
~~~
#### 按照特定格式打印变量
<font color='red'> 此部分没有实际用到过，暂定。 </font>

#### 查看内存内容





