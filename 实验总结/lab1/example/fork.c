
// fork.c: create a new process

#include "kernel/types.h"
#include "user/user.h"

/*
fork() 函数会创建一个新进程，内核会拷贝父进程然后生成一个子进程，它和父进程有相同的之类，数据，寄存器，文件描述符，当前目录
唯一不同的区别： fork返回值：再父进程中返回pid,在子进程中返回0
 only difference: fork() returns a pid in parent, 0 in child
  a pid (process ID) is an integer, kernel gives each process a different pid
  thus:
    fork.c's "fork() returned" executes in *both* processes fork的返回会在两个进程中同时返回(父进程，子进程)
    the "if(pid == 0)" allows code to distinguish
  ok, now we have *two* shell processes...
    what about running a program?
*/
int
main()
{
  int pid;

  pid = fork();

  printf("fork() returned %d\n", pid);

  if(pid == 0){
    printf("child\n");
  } else {
    printf("parent\n");
  }
  
  exit();
}
