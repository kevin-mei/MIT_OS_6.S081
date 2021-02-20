
// list.c: list file names in the current directory

#include "kernel/types.h"
#include "user/user.h"


/*
*示例：list.c，列出目录中的文件
  ls如何获取目录中文件的列表？试着理解ls是怎么实现的
  您可以打开目录并阅读->文件名
  "." 是进程当前目录的伪名称
  有关更多详细信息，请参见ls.c
*/

struct dirent {
  ushort inum;
  char name[14];
};

int
main()
{
  int fd;
  struct dirent e;

  fd = open(".", 0); // "." 是进程当前目录的伪名称 打开当前目录
  while(read(fd, &e, sizeof(e)) == sizeof(e)){
    if(e.name[0] != '\0'){
      printf("%s\n", e.name);
    }
  }
  exit();
}
