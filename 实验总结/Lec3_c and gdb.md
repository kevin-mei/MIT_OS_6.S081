6.S081 / 6.828 2019课程3：c和gdb

## 一. 课程前准备：
1. 阅读给的示例代码pointers.c见：<https://pdos.csail.mit.edu/6.828/2019/lec/pointers.c>
```cpp
// pointers.c 运行结果
1: a = 0x7ffeedc707d0, b = 0x7fa586c057f0, c = 0x0
2: a[0] = 200, a[1] = 101, a[2] = 102, a[3] = 103
3: a[0] = 200, a[1] = 300, a[2] = 301, a[3] = 302
4: a[0] = 200, a[1] = 400, a[2] = 301, a[3] = 302
5: a[0] = 200, a[1] = 128144, a[2] = 256, a[3] = 302
6: a = 0x7ffeedc707d0, b = 0x7ffeedc707d4, c = 0x7ffeedc707d1
```
2. 阅读给定的书籍gdb_slides.pdf 见：<https://pdos.csail.mit.edu/6.828/2019/lec/gdb_slides.pdf>

## 二. 知识点
1. We provide a file called .gdbinit which automaticallysets up GDB for use with QEMU.
- Must run GDB from the lab or xv6 directory
- Edit~/.gdbinit to allow other gdbinits
Use make to start QEMU with or without GDB.With GDB: 
- run `make qemu [-nox] -gdb`, then startGDB in a second shell 
- Use `make qemu [-nox]`when you don’t need GDB

    >确认了下，xv6-riscv-fall19 代码里确实存在这个文件
    ```cpp
    $ cat .gdbinit.tmpl-riscv 
    set confirm off
    set architecture riscv
    target remote 127.0.0.1:1234
    symbol-file kernel/kernel
    ```
2. GDB commandsRunhelp <command-name>if you’re not sure how touse a command.
   All commands may be abbreviated if unambiguous:
   c=co=cont=continue
   Some additional abbreviations are defined, e.g.
   s=step and si=stepi
3. Stepping
   stepruns one line of code at a time.  When there is afunction call, it stepsintothe called function.
   next does the same thing, except that it steps over function calls.
   stepi and nexti do the same thing for assemblyinstructions rather than lines of code.
   All take a numerical argument to specify repetition.Pressing the enter key repeats the previous command

4. Running
   continue runs code until a breakpoint is encountered oryou interrupt it with Control-C.
   finish runs code until the current function returns.
   advance <location>runs code until the instructionpointer gets to the specified location.
5. Breakpoints
   break <location>sets a breakpoint at the specifiedlocation.
   Locations can be memory addresses (“*0x7c00”) ornames (“monbacktrace”, “monitor.c:71”).
   Modify breakpoints usingdelete,disable,enable.
6. Conditional breakpoints
    break <location> if <condition>sets a breakpointat the specified location, but only breaks if the conditionis satisfied.
    cond <number> <condition>adds a condition on anexisting breakpoint.
7. Watchpoints
   Like breakpoints, but with more complicated conditions.
   watch <expression>will stop execution whenever theexpression’s value changes.
   watch -l <address>will stop execution whenever thecontents of the specified memory address change.What’s the difference betweenwa varandwa -l &var?
   rwatch [-l] <expression>will stop executionwhenever the value of the expression is read.
8. Examining
   x prints the raw contents of memory in whatever format you specify (x/xfor hexadecimal,x/i for assembly, etc).
   print evaluates a C expression and prints the result asits proper type.  It is often more useful thanx.The output fromp *((struct elfhdr *) 0x10000)is much nicer than the output fromx/13x 0x10000.
9. More examining
    info registers prints the value of every register.
    info frame prints the current stack frame.
    list <location>p rints the source code of the functionat the specified location.
    backtrace might be useful as you work on lab 1!
10. Layouts
    GDB has a text user interface that shows usefulinformation like code listing, disassembly, and registercontents in a curses UI.
    layout <name>switches to the given layout.
11. Other tricks
    You can use thesetcommand to change the value of avariable during execution.
    You have to switch symbol files to get function andvariable names for environments other than the kernel.For example, when debugging JOS:
    symbol-file obj/user/<name>
    symbol-file obj/kern/kernel
12. Summary
    Read the fine manual!  Use the help command.
    GDB is tremendously powerful and we’ve only scratched the surface today.
    It is well worth your time to spend an hour learning moreabout how to use it.






