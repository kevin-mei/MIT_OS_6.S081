
// pipe2.c: communication between two processes


#include "kernel/types.h"
#include "user/user.h"


/*
  示例：pipe2.c，进程之间进行通信
  pipe 管道与fork()很好地结合以实现ls | grep x
    shell创建一个管道，
    然后fork()（两次），
    然后将ls的FD 1连接到管道的写入FD，
    和grep的FD 0到管道
    [图表]
  $ pipe2 -- a simplified version
  FD的继承有助于使pipe/fork/exec更好工作
  更重要的是：管道是一个单独的抽象，但可以很好地与write/ fork()结合使用
*/

int
main()
{
  int n, pid;
  int fds[2];
  char buf[100];
  
  // create a pipe, with two FDs in fds[0], fds[1].
  pipe(fds);

  pid = fork();
  if (pid == 0) {
    // 子进程写入hello
    write(fds[1], "hello\n", 6);
  } else {
    // 父进程读取hello
    n = read(fds[0], buf, sizeof(buf));
    write(1, buf, n); //写入标准输出
  }

  exit();
}
