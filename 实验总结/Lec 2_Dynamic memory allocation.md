6.S081 / 6.828 2019课程2：内存分配

## 一. 课程前准备：
1. 阅读大佬的c语言书5.1-5.6节，6.4节，8.7节
2. 阅读给的示例代码malloc.c 


## 二. 知识点
- gcc 编译报错：
  `error: 'for' loop initial declarations are only allowed in C99 mode`
  这是因为gcc基于c89标准，换成C99标准就可以在for循环内定义i变量了：
  `gcc src.c -std=c99 -o src`
```CPP
// compile on Linux: 下面的是都是字母O，-O0,—O1,-O2,—O3,代表编译器的4个优化级别，默认是O1
$ gcc -O2 -o malloc malloc.c 
// 发现报错error: 'for' loop initial declarations ...,修改编译语句：
$ gcc -O2 -std=c99 -o malloc malloc.c 
// 发现还报错：
/*malloc.c: In function ‘bd_init’:
malloc.c:253:18: error: ‘MAP_ANONYMOUS’ undeclared (first use in this function)
    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    MAP_ANON和MAP_ANONYMOUS在C99标准中是无效，需要更换为gnu99,再来：
*/
$ gcc -O2 -std=gnu99 -o malloc malloc.c 
// 完美，就剩下warning，不关心，开始run: 
$ ./malloc
/* 输出如下：usec 是微秒，同样是分配10000空间的内存，region是最快的
elapsed time K&R is    131755 usec
elapsed time region is 277 usec
elapsed time buddy is  2057 usec
*/
```
接下来分析下这几种内存方法分配的区别
K&Rmalloc


系统程式设计
  为应用程序构建服务
    不特定于应用程序工作负载
  直接使用基础操作系统
  例子：
    UNIX实用程序，K / V服务器，ssh，bigint库等。

挑战：
  低级编程环境
    例如，数字是64位（不是无限的整数）
    例如，分配/释放内存（非类型化对象）
  并发
    允许并行处理请求
  崩溃
  表现
    为应用程序提供硬件性能

动态内存分配是一项基本的系统服务
  低级编程示例
    指针操作，类型转换等
    使用底层操作系统请求内存块
  必须支持广泛的工作负载
  表现很重要
    在构建应用程序时，内存分配器可能会成为瓶颈
    您可能会在职业生涯中的某个时候看一下内存分配器

背景：程序布局
  文本
  数据（静态存储）
  堆栈（堆栈存储）
  堆（动态内存分配）
    使用sbrk（）或mmap（）增长堆
  [文字| 数据 堆-> ... <-堆栈]
  0地址空间的顶部


数据段中分配的内存
  静态的，总是在那里

内存分配在堆栈上
  在功能条目上分配
  在功能退出时释放
  
堆的内存分配器
  界面
    p = malloc（int sz）
    免费（p）
  本讲座中的几种实现
    所有单线程

堆分配器的目标
  快速的malloc和免费
  内存开销小
  想要使用所有内存
    避免碎片化

简单的实现（例如，xv6的kalloc）
  当前堆作为块列表，按地址排序
  200字节堆
    [ 自由 ]
    0 200
  malloc（100）-> 0
  malloc（50）-> 100
  堆图片：
    [A | B | 自由 ]
    0100150200

API挑战
  免费（100）
    B的尺寸是多少？
  需要在某个地方存储B的大小
  在每个块的标题前放置一些元数据
  K＆R：结构头：包含大小和下一个指针
    [HA | H免费| H免费]
    0 16 100 + 16 150 + 32 200
  （假设：64位指针和大小）
    
碎片挑战
  malloc（75）->失败！
    空闲列表中没有块足够大
  我们需要合并

K＆R malloc（请参阅xv6的shell使用的user / umalloc.c）
  对malloc的首次拟合
    其他政策：最适合
  免费合并
  内存开销：如果每个malloc都是16字节，则为50％
  效率：
    取决于工作量
    例如，如果块按顺序返回：空闲列表有一个元素
    例如，如果块未按顺序返回：也许搜索长列表

旁：C
  流行于系统编程语言
    许多内核都是用C编写的（例如，Linux，BSD，MacOS，Windows）
    许多用C编写的系统软件（K / V服务器，ssh等）
  允许退出类型系统
    一块内存可以说是T1类型
    相同的内存也可以用作T2类型
    类型转换（标题*）
  指针=地址
    允许引用内存，设备等。
  K＆R malloc中的示例：
    char * p（指向char对象的指针）
    标题* p; p + 1指向哪里？
    无效* p（任何类型的指针）
  危险的编程语言
    容易出现错误
      无类型安全
      并发性不好
  下一个讲座：关于C和gdb的TA讲座 
  
区域分配器（请参阅malloc.c）
  分配器更快，但碎片化
  部分堆的区域
    非常快的malloc
    非常快的免费
    低内存开销
  严重的支离破碎
    释放区域，直到不使用整个区域
  应用程序必须适合地区使用
  比K＆R更有效，并且仍然通用吗？

好友分配器（请参阅malloc.c）
  其中一个实验室的主题
  数据结构：
    尺寸等级：2 ^ 0、2 ^ 1、2 ^ 2，... 2 ^ k
      大小0：最小大小（例如16个字节）
        ＃块= 2 ^ k *最小大小
	最小大小应足够大，以保留两个指针作为空闲列表
      大小1：2 ^ 1的块
        0级的两个好友合并为1个大小为1的块
      ... 等等 ...
      大小k：完整堆
        ＃块：1
    每种尺寸的免费清单
      没有该大小的块
    每个块在每种尺寸下的位向量
      alloc：是否分配了块？
      split：块拆分了吗？
    [绘制图]
  内部碎片
    malloc（17）分配32个字节
  操作：
    malloc：分配列表中的第一个空闲块
      也许在分割一个更大的块之后
    自由：
      确定块的大小
      如果好友免费，则合并
      将块放到尺寸等级的免费清单上
  为什么好？
    malloc / free很快
    成本：数据结构的大小
      可能的几种优化

其他顺序分配器：
  dlmalloc
  平板分配器

其他目标
  内存开销小
    例如伙伴分配器的元数据很大
  良好的内存位置
  核心增加，扩展性好
    并发malloc / free

参考

https://en.wikipedia.org/wiki/Buddy_memory_allocation
http://gee.cs.oswego.edu/dl/html/malloc.html
http://www.usenix.org/publications/library/proceedings/bos94/full_papers/bonwick.ps
http://people.freebsd.org/~jasone/jemalloc/bsdcan2006/jemalloc.pdf
http://www.cs.umass.edu/~emery/pubs/berger-asplos2000.pdf
http://bitsquid.blogspot.com/2015/08/allocation-adventures-3-buddy-allocator.html

唐纳德·纳斯（1997）。基本算法。计算机的艺术
编程。1（第二版）。马萨诸塞州雷丁：
艾迪生·韦斯利（Addison-Wesley）。第435、455页。ISBN 0-201-89683-4。