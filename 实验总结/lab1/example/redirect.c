
// redirect.c: run a command with output redirected

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

/*
UNIX约定：fd 0是“标准输入”，1是“标准输出”
示例：redirect.c，重定向命令的输出
  shell做什么用
    $ echo hello> out
  答案：fork，在子进程中中更改FD 1，然后调用exec
  $ redirect
  $ cat output.txt
  注意：open()总是选择最低的未使用FD；由于close（1）而为1。
  可以将I/O设置与程序逻辑分开是很整洁的
    诸如echo之类的程序无需了解重定向
      他们只是从FD 0 “标准输入” 读取，然后写入FD 1 “标准输出”
    只有shell必须知道重定向
  FD帮助这项工作
    间接级别-FD 1而不是特定文件
    内核维护FD表在fork()和exec()之间
  fork / exec分离有助于完成这项工作
    使shell有机会在执行前更改FD

*为什么这些I/O和流程抽象？
  为什么不别的呢？
  为什么在内核中使用FS？为什么不让用户程序以自己的方式使用磁盘？
  为什么采用FD间接级别？为什么不将文件名传递给write()？
  为什么要使用字节流而不是磁盘块或格式化的记录？
  为什么fork()和exec()是分开的？为什么不将它们结合起来？
  UNIX设计效果很好，但是我们还会看到其他设计！
*/

int
main()
{
  int pid;

  pid = fork();
  if(pid == 0){
    close(1);
    open("output.txt", O_WRONLY|O_CREATE);

    char *argv[] = { "echo", "this", "is", "redirected", "echo", 0 };
    exec("echo", argv);
    printf("exec failed!\n");
    exit();
  } else {
    wait();
  }

  exit();
}
