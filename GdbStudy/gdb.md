## 简介
GDB是GNU工具链中的调试软件，能够用于调试C、C++、java、go等程序编写的各种程序。

## 资料来源
https://zhuanlan.zhihu.com/p/74897601
http://kuring.me/post/cgdb/
https://www.jianshu.com/p/998dd862253a

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




~~~cpp
// 直接打印指针变量值
(gdb) p d 
$15 = (int *) 0x555555756260
~~~
* 若要想打印指针指向的内容，需要解引用，若是指针指向的数组则只能打印出第一个值
~~~cpp
// 指针指向的数组则只能打印出第一个值
(gdb) p *d
$16 = 0
~~~
* 指针指向的数组要打印多个值，需要@后面加上长度/变量值
~~~cpp
// 指针指向的数组要打印多个值，需要@后面加上长度/变量值
(gdb) p *d@a
$17 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
(gdb) p *d@10
$18 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
~~~
* 可定义一个Unix环境变量，来多次输出值
~~~cpp
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
~~~
* 可以使用下标去进行打印
~~~cpp
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
<font color='red'> 此部分没有实际用到过，暂定。 </font>

#### 自动显示变量内容
* 在运行到断点处时，我们希望程序继续运行会一直自动显示某个变量的值，使用display命令
~~~cpp
(gdb) b 19
(gdb) run
(gdb) display *d@10
// 输出 1: *d@10 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
~~~
* 想要查看哪些变量设置了display
~~~cpp
(gdb) info display
/* 输出
Auto-display expressions now in effect:
Num Enb Expression
1:   n  *d@10
*/
~~~
* 暂时不需要自动查看变量值的时候，可以去使能，想要时继续开启
~~~cpp
(gdb) disable display 编号  // 去编号使能
(gdb) disable display       // 去除所有编号使能
(gdb) enable display 编号   // 使能编号
(gdb) enable display        // 使能所遇编号
~~~
* 相处想要自动查看的变量时，可以使用delete清理
~~~cpp
(gdb) delete display 编号   // 清除编号
(gdb) delete display        // 清除所有编号
~~~
#### 查看寄存器内容
<font color='red'> 此部分没有实际用到过，暂定。 </font>

## 单步调试
#### 前言
本文所说的单步调试，不是纯粹意义上的单步调试，而是按照自己的要求来执行代码。
#### 查看程序代码
* 可以使用`list`指令来查看源代码，简写成`l`
~~~cpp
(gdb) l
/* 输出
8
9       int count(int num)
10      {
11          int i = 0;
12          if(0 > num) return 0;
13          while(i < num){
14              printf("%d\n",i);
15              ++i;
16          }
17          return i;
*/
(gdb) l
/* 输出
18      }
19
20      int main(void)
21      {
22          int a = 3;
23          int b = 7;
24          printf("it will calc a + b\n");
25          int c = add(a,b);
26          printf("%d + %d = %d\n",a,b,c);
27          count(c);
*/
(gdb) l
/* 输出
28          return 0;
29      }
30
*/
(gdb) 
~~~
#### 单步执行 - next
* 可以使用`next`指令来在程序断住时候继续执行下一条语句，简写成`n`
* 如果在`n`后面跟上数字num时，相当于执行该语句num次，就达到了执行n行的效果了
~~~cpp
(gdb) l
/* 输出
8
9       int count(int num)
10      {
11          int i = 0;
12          if(0 > num) return 0;
13          while(i < num){
14              printf("%d\n",i);
15              ++i;
16          }
17          return i;
*/
(gdb) l
/* 输出
18      }
19
20      int main(void)
21      {
22          int a = 3;
23          int b = 7;
24          printf("it will calc a + b\n");
25          int c = add(a,b);
26          printf("%d + %d = %d\n",a,b,c);
27          count(c);
*/
(gdb) l
/* 输出
28          return 0;
29      }
30
*/
(gdb) b 22
// 输出 Breakpoint 1 at 0x6f2: file stepDebug.c, line 22.
(gdb) run
/* 输出
Starting program: /home/swin/MyGit/GdbStudy/4-StepDebug/stepDebug.out 

Breakpoint 1, main () at stepDebug.c:22
22          int a = 3;
*/
(gdb) display c
// 输出 1: c = 0
(gdb) n
/* 输出
23          int b = 7;
1: c = 0
*/
(gdb) n 2
/* 输出
it will calc a + b
25          int c = add(a,b);
1: c = 0
*/
(gdb) n
/* 输出
26          printf("%d + %d = %d\n",a,b,c);
1: c = 10
*/
~~~
#### 单步进入 - step
* 如果想要步入函数内部，可以使用`step`指令来实现，简写成`s`
* `s`命令会尝试进入函数，如果没有函数可以进入效果同`n`，后面可加执行步数num
* 需要跳出函数执行/函数无源码需要跳出，使用`finish`，会完成该函数的调用并返回相应结果
~~~cpp
(gdb) l
/* 输出
8
9       int count(int num)
10      {
11          int i = 0;
12          if(0 > num) return 0;
13          while(i < num){
14              printf("%d\n",i);
15              ++i;
16          }
17          return i;
*/
(gdb) l
/* 输出
18      }
19
20      int main(void)
21      {
22          int a = 3;
23          int b = 7;
24          printf("it will calc a + b\n");
25          int c = add(a,b);
26          printf("%d + %d = %d\n",a,b,c);
27          count(c);
*/
(gdb) l
/* 输出
28          return 0;
29      }
30
*/
(gdb) b 24
// 输出 Breakpoint 1 at 0x700: file stepDebug.c, line 24.
(gdb) run
/* 输出
Starting program: /home/swin/MyGit/GdbStudy/4-StepDebug/stepDebug.out 

Breakpoint 1, main () at stepDebug.c:24
24          printf("it will calc a + b\n");
*/
(gdb) s
/* 输出
_IO_puts (str=0x5555555547d8 "it will calc a + b") at ioputs.c:33
33      ioputs.c: No such file or directory.
*/
(gdb) finish
/* 输出
Run till exit from #0  _IO_puts (str=0x5555555547d8 "it will calc a + b") at ioputs.c:33
it will calc a + b
main () at stepDebug.c:25
25          int c = add(a,b);
Value returned is $1 = 19
*/
(gdb) n
// 输出 26          printf("%d + %d = %d\n",a,b,c);
(gdb) p c
// 输出 $2 = 10
(gdb) n
/* 输出
3 + 7 = 10
27          count(c);
*/
(gdb) s
/* 输出
count (num=10) at stepDebug.c:11
11          int i = 0;
*/
(gdb) display i
// 输出 1: i = 10
(gdb) s 4
/* 输出 
__printf (format=0x5555555547d4 "%d\n") at printf.c:28
28      printf.c: No such file or directory.
*/
(gdb) finish
/* 输出
Run till exit from #0  __printf (format=0x5555555547d4 "%d\n") at printf.c:28
0
count (num=10) at stepDebug.c:15
15              ++i;
1: i = 0
Value returned is $3 = 2
*/
(gdb) n
/* 输出
13          while(i < num){
1: i = 1
*/
(gdb) finish
/* 输出
Run till exit from #0  count (num=10) at stepDebug.c:13
1
2
3
4
5
6
7
8
9
main () at stepDebug.c:28
28          return 0;
Value returned is $4 = 10
*/
(gdb) n
// 输出 29      }
(gdb) n
/* 输出
__libc_start_main (main=0x5555555546ea <main>, argc=1, argv=0x7fffffffe488, init=<optimized out>, fini=<optimized out>, rtld_fini=<optimized out>, 
    stack_end=0x7fffffffe478) at ../csu/libc-start.c:344
344     ../csu/libc-start.c: No such file or directory.
*/
(gdb) 
~~~
#### 继续执行到下一个断点 - continue
* 若想要执行到下一个断点，可以使用`continue`命令,简写为`c`,或使用`fg`
* 若想要执行到接下去的N个断点，可以使用命令`c 数字`
~~~cpp
(gdb) l
/* 输出
8
9       int count(int num)
10      {
11          int i = 0;
12          if(0 > num) return 0;
13          while(i < num){
14              printf("%d\n",i);
15              ++i;
16          }
17          return i;
*/
(gdb) l
/* 输出
18      }
19
20      int main(void)
21      {
22          int a = 3;
23          int b = 7;
24          printf("it will calc a + b\n");
25          int c = add(a,b);
26          printf("%d + %d = %d\n",a,b,c);
27          count(c);
*/
(gdb) l
/* 输出
28          return 0;
29      }
30
*/
(gdb) b 15
// 输出 Breakpoint 1 at 0x6d9: file stepDebug.c, line 15.
(gdb) run
/* 输出
Starting program: /home/swin/MyGit/GdbStudy/4-StepDebug/stepDebug.out 
it will calc a + b
3 + 7 = 10
0

Breakpoint 1, count (num=10) at stepDebug.c:15
15              ++i;
*/
(gdb) c
/* 输出
Continuing.
1

Breakpoint 1, count (num=10) at stepDebug.c:15
15              ++i;
*/
(gdb) c
/* 输出
Continuing.
2

Breakpoint 1, count (num=10) at stepDebug.c:15
15              ++i;
*/
(gdb) c
/* 输出
Continuing.
3

Breakpoint 1, count (num=10) at stepDebug.c:15
15              ++i;
*/
(gdb) c 4
/* 输出
Will ignore next 3 crossings of breakpoint 1.  Continuing.
4
5
6
7

Breakpoint 1, count (num=10) at stepDebug.c:15
15              ++i;
*/
(gdb) fg
/* 输出
Continuing.
8

Breakpoint 1, count (num=10) at stepDebug.c:15
15              ++i;
*/
(gdb) 
~~~
#### 继续运行到指定位置 - until
* 在断点处程序暂停，若想临时运行到指定行，可以使用`until`指令，简称`u`，效果是利用临时断点功能，执行到该处
~~~cpp
(gdb) l
/* 输出
8
9       int count(int num)
10      {
11          int i = 0;
12          if(0 > num) return 0;
13          while(i < num){
14              printf("%d\n",i);
15              ++i;
16          }
17          return i;
*/
(gdb) l
/* 输出
18      }
19
20      int main(void)
21      {
22          int a = 3;
23          int b = 7;
24          printf("it will calc a + b\n");
25          int c = add(a,b);
26          printf("%d + %d = %d\n",a,b,c);
27          count(c);
*/
(gdb) l
/* 输出
28          return 0;
29      }
30
*/
(gdb) b 23
// 输出 Breakpoint 1 at 0x6f9: file stepDebug.c, line 23.
(gdb) run
/* 输出
Starting program: /home/swin/MyGit/GdbStudy/4-StepDebug/stepDebug.out 

Breakpoint 1, main () at stepDebug.c:23
23          int b = 7;
*/
(gdb) u 27
/* 输出
it will calc a + b
3 + 7 = 10
main () at stepDebug.c:27
27          count(c);
*/
(gdb) 
~~~
#### 跳过执行 - skip
* 使用`skip`可以在`step`命令时跳过一些不想关注的函数
~~~cpp
// skip函数

(gdb) l
/* 输出
8
9       int count(int num)
10      {
11          int i = 0;
12          if(0 > num) return 0;
13          while(i < num){
14              printf("%d\n",i);
15              ++i;
16          }
17          return i;
*/
(gdb) l
/* 输出
18      }
19
20      int main(void)
21      {
22          int a = 3;
23          int b = 7;
24          printf("it will calc a + b\n");
25          int c = add(a,b);
26          printf("%d + %d = %d\n",a,b,c);
27          count(c);
*/
(gdb) l
/* 输出
28          return 0;
29      }
30
*/
(gdb) b 27
// 输出 Breakpoint 1 at 0x73a: file stepDebug.c, line 27.
(gdb) skip function count
// 输出 Function count will be skipped when stepping.
(gdb) info skip
/* 输出
Num   Enb Glob File                 RE Function
1     y      n <none>                n count
*/
(gdb) run
/* 输出
Starting program: /home/swin/MyGit/GdbStudy/4-StepDebug/stepDebug.out 
it will calc a + b
3 + 7 = 10

Breakpoint 1, main () at stepDebug.c:27
27          count(c);
*/
(gdb) s
/* 输出
0
1
2
3
4
5
6
7
8
9
28          return 0;
*/
(gdb) s
// 输出 29      }
(gdb)
~~~
* 使用`skip`可以在`step`命令时跳过一些不想关注的某个文件中的函数
~~~cpp
// skip 文件
(gdb) skip file stepDebug.c
// 输出 File stepDebug.c will be skipped when stepping.
(gdb) b 27
// 输出 Breakpoint 1 at 0x73a: file stepDebug.c, line 27.
(gdb) run
/* 输出
Starting program: /home/swin/MyGit/GdbStudy/4-StepDebug/stepDebug.out 
it will calc a + b
3 + 7 = 10

Breakpoint 1, main () at stepDebug.c:27
27          count(c);
*/
(gdb) s
/* 输出
0
1
2
3
4
5
6
7
8
9
28          return 0;
*/
(gdb) 
~~~
* `skip`同`break`、`display`，都是有info信息的
~~~cpp
(gdb) info skip            // 查看skip信息
(gdb) skip enable skip值   // 使能某个skip标志
(gdb) skip enable          // skip标志全部使能
(gdb) skip disable skip值  // 去使能某个skip标志
(gdb) skip disable         // 去使能属于skip标志
(gdb) skip delete skip值   // 删除某个skip标志
(gdb) skip delete          // 删除所有skip标志
~~~

## 源码查看
#### 前言
调试过程中难免要查看相应的源码，如果打开另一个窗口未免显得过于麻烦，本节介绍在GDB模式下对源码进行查看和编辑
#### 直接打印源码
* `list`命令就是直接打印源码，简称为`l`
* 可以多次输出`l`来打印接下来部分的源码(只会打印main入口)
* 也可以使用`l +`和`l -`来打印上次的部分源码后部分/前部分输出
~~~cpp
(gdb) l
/* 输出
1       #include <stdio.h>
2       #include "test.h"
3
4       int main(void)
5       {
6           printf("it will print from 5 to 1\n");
7           printNum(5);
8           printf("print end\n");
9
10          printf("it will print from 1 to 5\n");
*/
(gdb) l
/* 输出
11          printNum1(5);
12          printf("print end\n");
13          return 0;
14      }
15
*/
(gdb) l -
/* 输出
1       #include <stdio.h>
2       #include "test.h"
3
4       int main(void)
5       {
6           printf("it will print from 5 to 1\n");
7           printNum(5);
8           printf("print end\n");
9
10          printf("it will print from 1 to 5\n");
*/
(gdb) l +
/* 输出
11          printNum1(5);
12          printf("print end\n");
13          return 0;
14      }
15
*/
(gdb) 
~~~
#### 列出指定行附近的源码
* `l 行号`可以列出以指定行为中心附近的源码
~~~cpp
(gdb) l 10
/* 输出
5       {
6           printf("it will print from 5 to 1\n");
7           printNum(5);
8           printf("print end\n");
9
10          printf("it will print from 1 to 5\n");
11          printNum1(5);
12          printf("print end\n");
13          return 0;
14      }
*/
(gdb) 
~~~
#### 列出指定函数附近的源码
* `l 函数名`可以列出指定函数附近的源码
~~~cpp
(gdb) l printNum
/* 输出
1       #include "test.h"
2
3       void printNum(int n)
4       {
5           if(n < 0) return;
6           while(n > 0)
7           {
8               printf("%d\n",n);
9               n--;
10          }
*/
(gdb) 
~~~
#### 设置源码一次性列出的行数
* 可以使用`listsize`属性来修改每次列出的行数
~~~cpp
(gdb) set listsize 30
(gdb) show listsize
// 输出 Number of source lines gdb will list by default is 30.
(gdb) l
/* 输出
1       #include <stdio.h>
2       #include "test.h"
3
4       int main(void)
5       {
6           printf("it will print from 5 to 1\n");
7           printNum(5);
8           printf("print end\n");
9
10          printf("it will print from 1 to 5\n");
11          printNum1(5);
12          printf("print end\n");
13          return 0;
14      }
15
*/
(gdb) 
~~~
* 可以设置`listsize`为`0`或`unlimited`,这样源码列出就无限制
~~~cpp
(gdb) set listsize 0
(gdb) l
/* 输出
1       #include <stdio.h>
2       #include "test.h"
3
4       int main(void)
5       {
6           printf("it will print from 5 to 1\n");
7           printNum(5);
8           printf("print end\n");
9
10          printf("it will print from 1 to 5\n");
11          printNum1(5);
12          printf("print end\n");
13          return 0;
14      }
15
*/
(gdb)
~~~
#### 列出指定行之间的源码
* `list 起始行,终止行`可以输出起始行到终止行之间的源码
* `list 起始行,`或者`list ,终止行`可以输出以起始行为开头/终止行为结尾，指定行数的源码
~~~cpp
(gdb) l 5,10
/* 输出
5       {
6           printf("it will print from 5 to 1\n");
7           printNum(5);
8           printf("print end\n");
9
10          printf("it will print from 1 to 5\n");
*/
(gdb) 

(gdb) l 5,
/* 输出
5       {
6           printf("it will print from 5 to 1\n");
7           printNum(5);
8           printf("print end\n");
9
10          printf("it will print from 1 to 5\n");
11          printNum1(5);
12          printf("print end\n");
13          return 0;
14      }
*/
(gdb) 

(gdb) l ,15
/* 输出
6           printf("it will print from 5 to 1\n");
7           printNum(5);
8           printf("print end\n");
9
10          printf("it will print from 1 to 5\n");
11          printNum1(5);
12          printf("print end\n");
13          return 0;
14      }
15
*/
(gdb) 
~~~
#### 列出指定文件的源码
* 若没有指示会默认列出main.c的源码，如果想要看指定文件的源码可以使用`l location`,
* 其中location可以是`文件名:行号`
~~~cpp
(gdb) l test.c:10
/* 输出
5           if(n < 0) return;
6           while(n > 0)
7           {
8               printf("%d\n",n);
9               n--;
10          }
11      }
12
13      void printNum1(int n)
14      {
~~~
* 其中location可以是`函数名`
~~~cpp
(gdb) l printNum1
/* 输出
9               n--;
10          }
11      }
12
13      void printNum1(int n)
14      {
15          if(n < 0) return;
16          int i = 1;
17          while(i <= n)
18          {
*/
(gdb) 
~~~
* 其中location可以是`文件名:函数名`
~~~cpp
(gdb) l test.c:printNum
/* 输出
1       #include "test.h"
2
3       void printNum(int n)
4       {
5           if(n < 0) return;
6           while(n > 0)
7           {
8               printf("%d\n",n);
9               n--;
10          }
*/
(gdb) 
~~~
* 其中location可以是`文件名:行号,文件名:行号`
~~~cpp
(gdb) l test.c:5,test.c:15
/* 输出
5           if(n < 0) return;
6           while(n > 0)
7           {
8               printf("%d\n",n);
9               n--;
10          }
11      }
12
13      void printNum1(int n)
14      {
15          if(n < 0) return;
*/
(gdb) 
~~~
#### 指定源码路径
###### 源码被移走
<font color='red'> 此部分没有实际用到过，暂定。 </font>

###### 更换源码目录
<font color='red'> 此部分没有实际用到过，暂定。 </font>

#### 编辑源码
* 在某些时候GDB模式下设置了很多东西，不想退出想要直接在GDB模式下修改代码，可以使用下述方式解决。
~~~cpp
// 设置默认编辑器为vim
$ EDITOR=/usr/bin/vim
$ export EDITOR

// 如果你不知道vim在哪里，可以用下述指令进行查询
$ whereis vim
/* 输出
vim: /usr/bin/vim /usr/bin/vim.tiny /usr/bin/vim.basic /usr/bin/vim.gnome /etc/vim /usr/share/vim /usr/share/man/man1/vim.1.gz
*/
$ which vim
// 输出 /usr/bin/vim
~~~
* 可以使用`edit location进行编辑程序`
~~~cpp
(gdb)edit 3        // 编辑当前默认文件第三行
(gdb)edit printNum // 编辑printNum函数
(gdb)edit test.c:5 // 编辑test.c第五行
~~~
* 编辑完后，一定要重新进行编译，否则还是原先的out文件，这里为了在GDB模式下执行shell指令，
一定在前加上关键词shell，来表明这是一条shell指令。这样就不用退出GDB模式来进行编译程序了。
~~~cpp
(gdb)shell gcc -g main.c test.c -o readResouce.out
~~~

## 可视化界面
#### 选择原因
在使用gdb调试代码的时候会因为查看不到代码而烦恼，这是因为gdb的list不好用。可以选择gdb -tui或者cgdb来进行调试
#### gdb tui模式
gdb -tui模式可以解决边调试边list的缺陷，但是它仍有两个缺陷：  
(1) 语法未高亮
(2) 使用ssh登录远程服务器时，源码布局不能自动刷新，需要`clrt + l`来进行刷新
#### cgdb模式
就gdb tui模式下源码布局不能自动刷新的问题，cgdb解决了这个问题
* 安装 `sudo apt-get install cgdb`
* 常用命令
~~~cpp
ESC：        切换焦点到源码模式，在该界面中可以使用vi的常用命令
i：          切换焦点到gdb模式
I:           切换焦点到TTY模式(若打开了TTY模式)
o：          在源码模式中，打开文件对话框，选择要显示的代码文件，按ESC取消
空格：       在源码模式中，在当前行设置一个断点
-/=：        在源码模式中，上移/下移源码界面
shift + t：  打开/关闭TTY窗口(TTY窗口可以显示程序的输出/输入参数)
shift + -/=：在源码模式中，上移/下移TTY界面
~~~
* 注意cgdb不能使用`edit`命令。
