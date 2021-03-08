# Lab: Simple xv6 shell
## 一. 实验准备
This lab will shed light on how UNIX shells use basic system calls.
这个实验将阐明unix的shell如何使用系统调用

Your job is to write a simple shell for xv6. It should be able to run commands with arguments, handle input and output redirection, and set up two-element pipelines. Your shell should act like the xv6 shell sh for these examples as well as similar commands:
我需要写一个简单的shell,它要支持运行带参数的命令，输入输出重定向，设置两个管道；我的shell 应该支持那些指令，那么面向测试程序编程，可以看看testsh.c中t1~t9函数，一共有9条，详见：user/testsh.c
提示：
- 我的shell应该写在user/nsh.c下，然后在makefile里添加它并编译；
- 我的shell应以@开头而不是$,方便和xv6shell区分开；运行效果像下面这样：
- 不要使用内存分配器函数，如`malloc()`;尽量使用栈内存和全局变量；
- 可以限制cmd名字的长度，参数的数量，和单个参数的长度等；
- 可以使用testsh测试你的nsh.c；testsh会将输出重定向，你也可以修改testsh的代码，方便你看到自己程序的输出；
- 大佬c语言书里的代码时很有用的，多看看，如5.12节的gettoken(); 这个函数sh.c有用到

参考资料：这里可以参考哈工大的实验链接，它也是用的mit 6.s081的课程，<https://hitsz-lab.gitee.io/os_lab/lab2/part3/>,可以把它当成中文版的翻译；

## 二. 知识点
1. `int unlink(const char*);`函数，关闭给定文件的所有文件描述符的链接，然后删除文件；如果给定的是一个文件的软链接，则删除这个软链接
2. `int chdir(const char*);`是C语言中的一个系统调用函数（同cd），用于改变当前工作目录，其参数为Path 目标目录，可以是绝对目录或相对目录。
3. `gets(char *buf, int max)`从标准输入中读取字符，直到遇到`\n`获`\r`。
4. `fprintf(int fd, const char *fmt, ...)` 指定给指定文件描述符fd中写入字符串

## 三. 任务分解
我自己实现的shell都要完成那些任务

1. 先`main()`函数， shell的main函数不需要参数，它从标准输入中读取数据
```cpp
int main(void)
{
	static char buf[100];
	int fd;

	// Ensure that three file descriptors are open.
	while ((fd = open("console", O_RDWR)) >= 0)
	{
		if (fd >= 3)
		{
			close(fd);
			break;
		}
	}

	// Read and run input commands.
  // getcmd 从标准输入中读取字符，直到遇到`\n`获`\r`。
	while (getcmd(buf, sizeof(buf)) >= 0)
	{
		if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
		  // Chdir must be called by the parent, not the child.
		  buf[strlen(buf)-1] = 0;  // chop \n
		  if(chdir(buf+3) < 0)
		    fprintf(2, "cannot cd %s\n", buf+3);
		  continue;
		}
		if (fork1() == 0)
		{ // 在子进程中运行指令
			char *argv[MAXARGS];
			int argc;
			parsecmd(buf, argv, &argc); // 解析指令，将指令按照空格填充到argv中
			runcmd(argc, argv); // 运行指令，运行后执行cmd
		}
    // 父进程等子进程执行完指令后继续从标准输入读取下一条指令
		wait(0);
	}
	exit(0);
}
```

2. 解析指令， 将指令按照空格填充到argv中

```cpp
void parsecmd(char *buf, char *argv[], int *argc)
{
	int i = 0, j = 0;
	for (; buf[j] != '\n' && buf[j] != '\0'; j++)
	{
		// 当buf[j]属于whitespace的string时，返回对应的下标
		while (strchr(whitespace, buf[j]))
		{
			j++;
		}
		// 此时得到的j一定不是whitespace
		argv[i++] = buf + j; // buf+j是地址，这里也可以用&buf[j]
		// 在找到下一个whitespace
		while (0 == strchr(whitespace, buf[j]))
		{
			j++;
		}
		buf[j] = '\0';// 这里buf[j]="\0"; 
	}
	argv[i] = 0; 
	*argc = i;
}
```

3. 执行指令

```cpp
void runcmd(int argc, char *argv[])
{
	// 先看argv里有没有'|'，需要管道执行的
	for (int i = 0; i < argc; i++)
	{
		if (!strcmp(argv[i], "|")) // strcmp返回的是两个str相减，为0说明找到了
		{
			myPipe(argc, argv); // 待实现 有管道的时候是否需要退出
		}
	}

  // 这里输入输出重定向，最终要实现的是 echo < inputfile > outfile 替换为 echo \0
	for (int i = 0; i < argc; i++)
	{
		// 判断输出重定向
		if (!strcmp(argv[i], ">"))
		{
			if (argc - 1 != i)
			{
				close(1);
				open(argv[i + 1], O_CREATE | O_WRONLY);
				argv[i] = '\0'; // 这样后续执行的时候 只执行>之前的命令
			}
			else
			{
				fprintf(2, "%s\n", "output redirect error"); //fprintf 指定给指定文件描述符fd中写入字符串
			}
		}
		// 判断输入重定向
		if (!strcmp(argv[i], "<"))
		{
			if (argc - 1 != i)
			{
				close(0);
				open(argv[i + 1], O_RDONLY);
				argv[i] = '\0'; // 这样后续执行的时候 只执行<之前的命令
			}
			else
			{
				fprintf(2, "%s\n", "input redirect error"); //fprintf 指定给指定文件描述符fd中写入字符串
			}
		}
	}
	// 这个指令既没有输入重定向，也没有输出重定向，也没有pipe，这里执行直接执行
	if (argv[0] == 0)
		exit(-1);
	exec(argv[0], argv);
	fprintf(2, "exec %s failed\n", argv[0]);
}
```

4. 管道, `cat readme | grep xxx`子进程执行'|'左边的，我原以为父进程等待子进程执行完后，在执行'|'右边的;但实际这里不用等子进程执行完，剩余的`grep xxx`会自己读取标准输入的内容，然后过滤结果到标准输出；
`grep xxx [file] // file为空时，缺省为标准输入` 

```cpp
void myPipe(int argc, char *argv[])
{
	// 找到argv中的"|",然后更换为"\0";
	int i = 0;
	for (i = 0; i < argc; i++)
	{
		if (!strcmp(argv[i], "|")) // strcmp返回的是两个str相减，为0说明找到了
		{
			argv[i] = 0;
			break;
		}
	}

	// 此时指令已经从i出分开 [0,i)出为第一条指令c长度为i; [i+1,argc) 为第二条指令，长度为argc-i-1
	// 这里左边的指令要输出到右边的指令
	// 利用管道
	int fds[2];
	pipe(fds);
	int pid = fork();
	if (pid < 0)
	{
		fprintf(2, "myPipe exec failed,fork error.\n");
	}
	else if (0 == pid)
	{
		// 子进程 执行左边的指令，然后将标准输出重定向到fds[1] fds1
		close(1);
		dup(fds[1]); // 利用最低的fd拷贝一份fds[1] ，详见book-riscv-rev0.pdf 1.3 pipe
		close(fds[1]);
		close(fds[0]);
		runcmd(i, argv);
	}
	else
	{
		// 父进程 读取fds[0]中的数据，然后执行右边的指令
		// 父进程这里需要等到读到数据后才执行么？或者是等到子进程退出后再执行么？
		// 不需要，具体读取标准输入的数据交给runcmd去执行，这里只做输入重定向
		close(0);
		dup(fds[0]);
		close(fds[0]);
		close(fds[1]);
		runcmd(argc - i - 1, argv + i + 1);
	}
}

```

## 四. 思考问题

1. shell里打印在控制台的`$`文件字符为啥是打印在标准错误的fd中，而不是打印在标准输出的fd中？
   个人认为和shell 的main()函数里下面的代码有关， 标准输入，输出，错误都是console文件，这个console文件应该就是屏幕这个黑框，都现实在上面，我们执行个错误指令，对应的错误信息也会显示出来；
   那都会显示为啥不直接写到标准输入中，如果有人读标准输入，也就会多读一个'$'，因此写在'$'标准错误中
```cpp
// 假如说标准输入，输出，错误都关了，那就得打开3次
  while((fd = open("console", O_RDWR)) >= 0){
    if(fd >= 3){
      close(fd);
      break;
    }
  }
```
