
// copy.c: copy input to output.

#include "kernel/types.h"
#include "user/user.h"

/*
  read()和write()是系统调用
  read()/write()第一个参数是“文件描述符”（fd）
    传递给内核以告诉它要读取/写入哪个“打开文件”
    必须先前已打开
    FD连接到文件/设备/套接字/＆c
    一个进程可以打开很多文件，有很多FD
    UNIX约定：fd 0是“标准输入”，1是“标准输出”
  read()第二个参数是指向一些要读取的内存的指针
  第三个参数是要读取的字节数
    read()可能读的少一些，但不能读更多
*/
int
main()
{
  char buf[64];

  while(1){
    int n = read(0, buf, sizeof(buf));
    if(n <= 0)
      break;
    write(1, buf, n);
  }

  exit();
}
