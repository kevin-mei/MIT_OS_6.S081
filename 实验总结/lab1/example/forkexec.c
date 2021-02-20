
// forkexec.c: fork then exec

#include "kernel/types.h"
#include "user/user.h"

/*
forkexec.c，fork()一个新进程，exec()一个程序
  $ forkexec
  这是一个常见的UNIX习惯用法：
    fork()子进程
    exec()在子进程里使用exec()执行子命令
    父进程的 wait() 函数for child to finish会等待子进程完成
  Shell会对您键入的每个命令执行fork / exec / wait
    在wait()之后，shell序将显示下一个提示
    如果没有wait()，直接返回（shell会跳过wait()）
  注意：fork()复制，接下来的exec()会丢弃内存
    这看起来似乎很浪费
    不会在在接下来的实验中会透明地优化掉副本
*/
int
main()
{
  int pid;

  pid = fork();
  if(pid == 0){ // 只在子进程里执行exec
    char *argv[] = { "echo", "this", "is", "the", "echo", "command", 0 };
    exec("echo", argv);
    printf("exec failed!\n");
    exit();
  } else {
    printf("parent waiting\n");
    wait();
    printf("the child exited\n");
  }

  exit();
}
