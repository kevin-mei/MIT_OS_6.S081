# lab1 实验目的：熟悉xv6系统和它的系统调用

## 一. 实验前准备
按计划，先阅读下面三项

1. 介绍： <https://pdos.csail.mit.edu/6.828/2019/lec/l-overview.txt>
   > 大体意思是：6.S081专注于系统调用接口的设计和实现。
  我们将研究UNIX（Linux，Mac，POSIX和xv6）。
  让我们看看程序如何使用系统调用接口。
2. 例子： <https://pdos.csail.mit.edu/6.828/2019/lec/l-overview/>;
    > 实例 给了多个.c文件，上面的链接中有对应的英文解释，自己看的时候可以对应翻译到.c文件
  另外，推荐<https://mit-public-courses-cn-translatio.gitbook.io/mit6-s081>，大佬应该是看了视频课程，并做了翻译，很强；
3. read chapter 1 of the xv6 book
   > 推荐使用知云文献翻译，中英文对照阅读

4. 接下来就是进行实验：作业链接：<https://pdos.csail.mit.edu/6.828/2019/labs/util.html>


## 二. 知识点

1. UNIX约定：fd 0是“标准输入”，1是“标准输出”
2. FD可以做一个指向“管道”的引用,就像对文件一样引用
  系统调用pipe()创建一对FD
    从第一个FD读取
    从第二个FD写入



3. 单词
    >amusement [ə'mjuzmənt] 娱乐
 
    >assignment [ə'saɪnmənt] 任务，作业

4. vi 跳转到指定行号：

    ```cpp
    // 第一种方式
    $ vi makefile
    $:n //n 为行号

    // 第二种方式
    $ vi +169 makefile // 打开makefile 直接跳转到169行
    ```

5. 在当前文件夹下的所有文件内寻找包含3G的文件并打印出行号
  
    ```cpp
    // 在当前文件夹下寻找包含3G的行号
    $ grep 3G . -rn
    ```
      按照作业一开始，需要切换到util分支，切换之后，执行make，make qemu; 发现启动不了，内存溢出,报错信息如下，解决过程也附在下面：
    ```c
    // make qemu 报错信息
    qemu-system-riscv64 -machine virt -kernel kernel/kernel -m 3G -smp 3 -nographic -drive file=fs.img,if=none,format=raw,id=x0 -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0
    qemu-system-riscv64: cannot set up guest memory 'riscv_virt_board.ram': Cannot allocate memory
    make: *** [qemu] Error 1
    // 查看下自己的内存大小，剩余654M，果然不够3G，毕竟是虚拟机
    moocos-> free -h
                total       used       free     shared    buffers     cached
    Mem:          3.4G       2.7G       654M        14M       242M       1.6G
    -/+ buffers/cache:       952M       2.4G
    Swap:           0B         0B         0B
    // 查看下是哪里写着3G的内存，发现是makefile文件
    moocos-> grep 3G -rn
    Makefile:169:QEMUOPTS = -machine virt -kernel $K/kernel -m 3G -smp $(CPUS) -nographic
    ```
    到这里已经排查到是makefile 文件里设定的3G的启动内存大小，vi进去修改3G为128M，然后make clean,make, make qemu,搞定；


附提问解答：

1. 学生提问：为什么父进程在子进程调用exec之前就打印了“parent waiting”？
    > Robert教授：这里只是巧合。父进程的输出有可能与子进程的输出交织在一起，就像我们之前在fork的例子中看到的一样，只是这里正好没有发生而已。并不是说我们一定能看到上面的输出，实际上，如果看到其他的输出也不用奇怪。我怀疑这里背后的原因是，exec系统调用代价比较高，它需要访问文件系统，访问磁盘，分配内存，并读取磁盘中echo文件的内容到分配的内存中，分配内存又可能需要等待内存释放。所以，exec系统调用背后会有很多逻辑，很明显，处理这些逻辑的时间足够长，这样父进程可以在exec开始执行echo指令之前完成输出。这样说得通吧？
2. 学生提问：子进程可以等待父进程吗？
    >Robert教授：Unix并没有一个直接的方法让子进程等待父进程。wait系统调用只能等待当前进程的子进程。所以wait的工作原理是，如果当前进程有任何子进程，并且其中一个已经退出了，那么wait会返回。但是如果当前进程没有任何子进程，比如在这个简单的例子中，如果子进程调用了wait，因为子进程自己没有子进程了，所以wait会立即返回-1，表明出现错误了，当前的进程并没有任何子进程。
  **简单来说，不可能让子进程等待父进程退出。**
3. 学生提问：当我们说子进程从父进程拷贝了所有的内存，这里具体指的是什么呢？是不是说子进程需要重新定义变量之类的？
    >Robert教授：在编译之后，你的C程序就是一些在内存中的指令，这些指令存在于内存中。所以这些指令可以被拷贝，因为它们就是内存中的字节，它们可以被拷贝到别处。通过一些有关虚拟内存的技巧，可以使得子进程的内存与父进程的内存一样，这里实际就是将父进程的内存镜像拷贝给子进程，并在子进程中执行。
  实际上，当我们在看C程序时，你应该认为它们就是一些机器指令，这些机器指令就是内存中的数据，所以可以被拷贝。
4. 学生提问：如果父进程有多个子进程，wait是不是会在第一个子进程完成时就退出？这样的话，还有一些与父进程交错运行的子进程，是不是需要有多个wait来确保所有的子进程都完成？
    >Robert教授：是的，如果一个进程调用fork两次，如果它想要等两个子进程都退出，它需要调用wait两次。每个wait会在一个子进程退出时立即返回。当wait返回时，你实际上没有必要知道哪个子进程退出了，但是wait返回了子进程的进程号，所以在wait返回之后，你就可以知道是哪个子进程退出了。

## 三. 作业

1. 实现sleep

Implement the UNIX program sleep for xv6; your sleep should pause for a user-specified number of ticks. (A tick is a notion of time defined by the xv6 kernel, namely the time between two interrupts from the timer chip.) Your solution should be in the file user/sleep.c. 我的代码应该写到 user/sleep.c里面

Some hints:

Look at some of the other programs in user/ to see how you can obtain the command-line arguments passed to a program. If the user forgets to pass an argument, sleep should print an error message. 我可以看看user下的其它文件，看看是怎么获取命令行参数，记得对空参数的处理
The command-line argument is passed as a string; you can convert it to an integer using atoi (see user/ulib.c).
Use the system call sleep (see user/usys.S and kernel/sysproc.c). sleep功能最终应该调用 系统调用sleep 
Make sure main calls exit() in order to exit your program. 调用exit()确保自己的程序最后要退出
Add the program to UPROGS in Makefile and compile user programs by typing make fs.img. 把自己的程序写到makefile里
Look at Kernighan and Ritchie's book The C programming language (second edition) (K&R) to learn about C. 跟着大佬的书学习C语言

先来看第一个事，怎么获取命令行参数