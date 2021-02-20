
// pipe1.c: communication over a pipe

#include "kernel/types.h"
#include "user/user.h"

/*
示例：pipe1.c，通过管道进行通信
  shell如何实现
    $ ls | grep x
  $ pipe1
  FD可以做一个指向“管道”的引用,就像对文件一样引用
  系统调用pipe()创建一对FD
    从第一个FD读取
    从第二个FD写入
  内核内部维护管道缓冲区
    [u/k图]
    write()追加到缓冲区
    read()读取，它会阻塞直到有数据
*/

int
main()
{
  int fds[2];
  char buf[100];
  int n;

  // create a pipe, with two FDs in fds[0], fds[1].
  pipe(fds);
  
  write(fds[1], "hello\n", 6);
  n = read(fds[0], buf, sizeof(buf));

  write(1, buf, n); //UNIX约定：fd 0是“标准输入”，1是“标准输出” 给标准输出中写入hello

  exit();
}
