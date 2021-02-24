贝尔实验室和CSP线程
拉斯·考克斯
rsc@swtch.com

同时有塞尔维亚-克罗地亚， 瑞典

介绍

该页面是并发编程历史的一部分，重点介绍了Hoare的顺序顺序通信（CSP​​）语言[1] [1a]的特定血统 。这种风格的并发编程很有趣，原因不是效率而是清晰。即，仅将并发编程视为提高性能的一种手段是一个普遍的错误， 例如，以重叠磁盘I / O请求，通过将结果预取到预期的查询来减少延迟，或利用多个处理器。这些优势很重要，但与本讨论无关。毕竟，它们可以用其他样式来实现，例如异步事件驱动的编程。相反，我们对并发编程很感兴趣，因为它提供了自然的抽象，可以使某些程序更简单。

这不是什么

大多数计算机科学专业的本科生都被迫阅读安德鲁·比雷尔（Andrew Birrell）的 “线程编程简介”。 SRC线程模型是当前可用的大多数线程包使用的模型。所有这些的问题在于它们太低级了。与Hoare提供的通信原语不同，SRC样式线程模块中的原语必须与其他技术（通常是共享内存）组合才能有效使用。通常，程序员倾向于不构建自己的高级结构，而由于需要注意这些低级细节而感到沮丧。

目前，将Birrell的教程排除在外。这是一个不同的线程模型。如果您将其作为其他线程模型来处理，则可能会更容易理解。

顺序流程沟通

到1978年，在对多处理器进行编程的情况下，有许多提议的方法用于通信和同步。共享内存是最常见的通信机制，而信号量，关键区域和监视器则属于同步机制。CAR Hoare使用一种语言原语解决了这两个问题：同步通信。在Hoare的CSP语言中，进程通过从命名的未缓冲通道发送或接收值进行通信。由于通道是无缓冲的，因此发送操作将阻塞，直到将值传输到接收器为止，从而提供了一种同步机制。

Hoare的示例之一是重新格式化80列卡片以在125列打印机上进行打印的格式。在他的解决方案中，一个进程一次读取一张卡，将分解后的内容逐个字符发送到第二个进程。第二个过程将组合125个字符的组，然后将这些组发送到行式打印机。这听起来很琐碎，但是在没有缓冲的I / O库的情况下，单进程解决方案中涉及的必要簿记工作繁重。实际上，缓冲的I / O库实际上只是导出单字符通信接口的这两种过程的封装。

举另一个例子，霍亚尔（Hoare）归功于道格·麦克罗伊（Doug McIlroy），考虑所有质数的生成少于一千。可以通过执行以下伪代码的一系列流程来模拟Eratosthenes的筛网：

p =从左邻居得到一个数字
打印p
环形：
    n =从左邻居得到一个数字
    如果（p不除n）
        发送n到正确的邻居
生成过程可以将数字2、3、4，...，1000馈入管道的左端：该行中的第一个过程消除了2的倍数，第二个过程消除了3的倍数，第三个过程消除了5的倍数，依此类推：



The linear pipeline nature of the examples thus far is misrepresentative of the general nature of CSP, but even restricted to linear pipelines, the model is quite powerful. The power has been forcefully demonstrated by the success of the filter-and-pipeline approach for which the Unix operating system is well known [2] Indeed, pipelines predate Hoare's paper. In an internal Bell Labs memo dated October 11, 1964, Doug McIlroy was toying with ideas that would become Unix pipelines: “We should have some ways of coupling programs like garden hose--screw in another segment when it becomes necessary to massage data in another way. This is the way of IO also.” [3]

Hoare's communicating processes are more general than typical Unix shell pipelines, since they can be connected in arbitrary patterns. In fact, Hoare gives as an example a 3x3 matrix of processes somewhat like the prime sieve that can be used to multiply a vector by a 3x3 square matrix.

Of course, the Unix pipe mechanism doesn't require the linear layout; only the shell syntax does. McIlroy reports toying with syntax for a shell with general plumbing early on but not liking the syntax enough to implement it (personal communication, 2011). Later shells did support some restricted forms of non-linear pipelines. Rochkind's 2dsh supports dags; Tom Duff's rc supports trees.

Hoare's language was novel and influential, but lacking in a few key aspects. The main defect is that the unbuffered channels used for communication are not first-class objects: they cannot be stored in variables, passed as arguments to functions, or sent across channels. As a result of this, the communication structure must be fixed while writing the program. Hence we must write a program to print the first 1000 primes rather than the first n primes, and to multiply a vector by a 3x3 matrix rather than an nxn matrix.

Pan and Promela

In 1980, barely two years after Hoare's paper, Gerard Holzmann and Rob Pike created a protocol analyzer called pan that takes a CSP dialect as input. Pan's CSP dialect had concatenation, selection, and looping, but no variables. Even so, Holzmann reports that “Pan found its first error in a Bell Labs data-switch control protocol on 21 November 1980. ” [14]. That dialect may well have been the first CSP language at Bell Labs, and it certainly provided Pike with experience using and implementing a CSP-like language, his first of many.

Holzmann's protocol analyzer developed into the Spin model checker and its Promela language, which features first-class channels in the same way as Newsqueak (q.v.).

Newsqueak

Moving in a different direction, Luca Cardelli and Rob Pike developed the ideas in CSP into the Squeak mini-language [4] for generating user interface code. (This Squeak is distinct from the Squeak Smalltalk implementation.) Pike later expanded Squeak into the fully-fledged programming language Newsqueak [5][6] which begat Plan 9's Alef [7] [8], Inferno's Limbo [9], and Google's Go [13]. The main semantic advantage of Newsqueak over Squeak is that Newsqueak treats communications channels as first-class objects: unlike in CSP and Squeak, channels can be stored in variables, passed as arguments to functions, and sent across channels. This in turn enables the programmatic construction of the communication structure, thus allowing the creation of more complex structures than would be reasonable to design by hand. In particular, Doug McIlroy demonstrated how the communication facilities of Newsqueak can be employed to write elegant programs for manipulating symbolic power series [10]. Similar attempts in traditional languages tend to mire in bookkeeping. In a similar vein, Rob Pike demonstrated how the communication facilities can be employed to break out of the common event-based programming model, writing a concurrent window system [11].

Alef

Alef [7] [8] was a language designed by Phil Winterbottom to apply the Newsqueak ideas to a full-fledged systems programming language. Alef has two types of what we have been calling processes: procs and threads. The program is organized into one or more procs, which are shared-memory operating system processes that can be preemptively scheduled. Each proc contains one or more tasks, which are cooperatively scheduled coroutines. Note that each task is assigned to a particular proc: they do not migrate between procs.

The main use of procs is to provide contexts that can block for I/O independently of the main tasks. (Plan 9 has no select call, and even on Unix you need multiple procs if you want to overlap computation with non-network I/O.) The Acme paper [12] has a nice brief discussion of procs and threads, as do the lecture notes about the Plan 9 window system, also mentioned below.

Limbo

The Inferno operating system is a Plan 9 spinoff intended for set-top boxes. Its programming language, Limbo [9], was heavily influenced by Alef. It removed the distinction between procs and tasks, effectively having just procs, though they were of lighter weight than what most people think of as processes. All parallelism is preemptive. It is interesting that despite this, the language provides no real support for locking. Instead, the channel communication typically provides enough synchronization and encourages programmers to arrange that there is always a clear owner for any piece of data. Explicit locking is unnecessary.

Libthread

Back in the Plan 9 world, the Alef compilers turned out to be difficult to maintain as Plan 9 was ported to ever more architectures. Libthread was originally created to port Alef programs to C, so that the Alef compilers could be retired. Alef's procs and tasks are called procs and threads in libthread. The manual page is the definitive reference.

Go

Rob Pike and Ken Thompson moved on to Google and placed CSP at the center of the Go language's concurrency support.

Getting Started

To get a feel for the model, especially how processes and threads interact, it is worth reading the Alef User's Guide [8]. The first thirty slides of this presentation are a good introduction to how Alef constructs are represented in C.

The best examples of the power of the CSP model are McIlroy's and Pike's papers, mentioned above [10] [11].

Rob Pike's home page contains lecture notes from a course on concurrent programming: an introduction, and slides about the two aforementioned papers: squinting and window system. The last of the three provides a good example of how Plan 9 programs typically use procs and tasks.

Rob Pike gave a tech talk at Google that provides a good introduction (57 minute video).

Rob Pike's half of his 2010 Google I/O talk with Russ Cox shows how to use channels and Go's concurrency to implement a load balancing work management system.

Related Resources

John Reppy has applied the same ideas to ML, producing Concurrent ML. He used CML to build, among other things, the eXene multithreaded (non-event-driven) X Window System toolkit.

References

[1] C. A. R. Hoare, “Communicating Sequential Processes,” Communications of the ACM 21(8) (August 1978), 666-677.

[1a]C. A. R. Hoare, Communicating Sequential Processes. Prentice Hall, Englewood Cliffs, New Jersey, 1985.

[2] Michael S. Mahoney, ed., The Unix Oral History Project, Release 0: The Beginning

[3] M. Douglas McIlroy, internal Bell Labs memorandum, October 1964.

[4] Luca Cardelli and Rob Pike, “Squeak: a Language for Communicating with Mice,” Computer Graphics, 19(3) (July 1985: SIGGRAPH '85 Proceedings), 199-204.

[5] Rob Pike, “The Implementation of Newsqueak,” Software--Practice and Experience, 20(7) (July 1990), 649-659.

[6] Rob Pike, “Newsqueak: a Language for Communicating with Mice,” Computing Science Technical Report 143, AT&T Bell Laboratories, Murray Hill, 1989.

[7] Phil Winterbottom, “Alef Language Reference Manual,” in Plan 9 Programmer's Manual: Volume Two, AT&T, Murray Hill, 1995.

[8] Bob Flandrena, “Alef Users' Guide,” in Plan 9 Programmer's Manual: Volume Two, AT&T, Murray Hill, 1995.

[9] Dennis M. Ritchie, “The Limbo Programming Language,” in Inferno Programmer's Manual, Volume 2, Vita Nuova Holdings Ltd., York, 2000.

[10] M. Douglas McIlroy, “Squinting at Power Series,” Software--Practice and Experience, 20(7) (July 1990), 661-683.

[11] Rob Pike, “A Concurrent Window System,” Computing Systems, 2(2) 133-153.

[12] Rob Pike, “Acme: A User Interface for Programmers,” Proceedings of the Winter 1994 USENIX Conference, 223-234.

[13] golang.org, “The Go Programming Language”.

[14] Gerard Holzmann, “Spin's Roots”.

[15] Gerard Holzmann, “Promela Language Reference”.