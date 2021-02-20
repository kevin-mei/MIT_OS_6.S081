
// open.c: create a file, write to it.

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

/*
  $ open
  $ cat output.txt
  FD是一个小整数
  FD是由内核维护的每个进程表里的索引
  [用户/内核]
  不同的进程具有不同的FD名称空间
    即FD 1 通常对不同的进程意味着不同的东西  1是标准输出
  注意：这些示例会忽略错误-别草率！
    可以通过“man 2 open”查找系统调用参数/返回的有关信息

*当一个程序调用open()这个系统调用时会发生什么？
  看起来像一个函数调用，但这实际上是一个特殊的指令
  硬件保存了一些用户寄存器
  硬件增加特权级别
  硬件跳到内核中的已知“入口点”
  现在在内核中运行C代码
  内核中调用系统调用实现
    open()在文件系统中查找名称
    它可能会等待磁盘
    它更新内核数据结构（缓存，FD表）
  恢复用户寄存器
  降低特权级别
  跳回到程序中的调用点，该调用点将继续
  我们稍后会在课程中看到更多细节
*/
int
main()
{
  int fd = open("output.txt", O_WRONLY|O_CREATE);
  write(fd, "ooo\n", 4);

  exit();
}
