
// exec.c: replace a process with an executable file

#include "kernel/types.h"
#include "user/user.h"

/*
$ echo hello
  echo是echo.c编译后的，程序存储在文件中：指令和初始存储器
    由编译器和链接器创建
  因此有一个名为echo的文件，其中包含说明
  $执行
  exec()用可执行文件替换当前进程,进程覆盖有点意思
    丢弃指令和数据存储器
    从文件加载指令和内存
    保留文件描述符
  exec(文件名，参数数组)
    参数数组保存命令行参数；exec传递给main()
*/

int
main()
{
  char *argv[] = { "echo", "hello", 0 };

  exec("echo", argv);

  printf("exec failed!\n");

  exit();
}
