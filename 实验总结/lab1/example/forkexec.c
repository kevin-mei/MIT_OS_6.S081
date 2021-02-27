
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

    这里只是巧合。父进程的输出有可能与子进程的输出交织在一起，就像我们之前在fork的例子中看到的一样，只是这里正好没有发生而已。
    并不是说我们一定能看到上面的输出，实际上，如果看到其他的输出也不用奇怪。
    我怀疑这里背后的原因是，
    exec系统调用代价比较高，它需要访问文件系统，访问磁盘，分配内存，并读取磁盘中echo文件的内容到分配的内存中，分配内存又可能需要等待内存释放。
    所以，exec系统调用背后会有很多逻辑，很明显，处理这些逻辑的时间足够长，这样父进程可以在exec开始执行echo指令之前完成输出。
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
