

### git 仓库太大，clone失败的解决方法：

码云上，由于提交的版本历史记录过多，即长期以来，提交了成百上千个版本，导致仓库存储很大，超过了1G，直接clone会报错：
```
fatal: early EOF
fatal: the remote end hung up unexpectedly
fatal: index-pack failed
error: RPC failed; curl 18 transfer closed with outstanding read data remaining
```

git仓库某个分支历史版本过多，占用的存储也相应增大，会造成git客户端在clone或者checkout时失败。解决方法是clone或者checkout时设置一下拉取的深度（depth）。
clone项目：

将项目的Head版本clone下来：

$ git clone --depth 1 仓库地址

depth 1的作用是拉取的版本深度为1，即拉取最近一次提交的版本。这时候你可以看下clone下来的Head版本指向了哪个分支，查看当前已拉取的所有分支：

$ git branch -a

checkout其它分支：

通过分支名称去拉取其它分支。依次执行如下指令：

$ git remote set-branches origin '远程分支名称'
$ git fetch --depth 1 origin 远程分支名称


fetch --depth 1的作用是拉取该分支的版本深度为1，同理，也是拉取最近一次提交的版本。值也可以改为其他的，比如想拉取最近10次提交的版本历史，则–depth 10这样子。

### 下载riscv gun工具链

1. 安装virtualBox和unbuntu，我安装的是virtualBox 6.0.10, unbuntu 是14.04 版本
2. 下载一些安装包进行工具链的编译：
```
$ sudo apt-get install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev
```

3. 下载riscv gun工具链
正常版本 github，但是我老下载出错，所以采用码云进行下载，下载速度更快,我主要用它，这里可以多开几个tab页一块下载，但注意都在`riscv-gnu-toolchain`路径下进行，如下所示：
```
$ git clone https://gitee.com/mirrors/riscv-gnu-toolchain
$ cd riscv-gnu-toolchain
$ git clone --recursive https://gitee.com/mirrors/qemu.git
$ git clone --recursive https://gitee.com/mirrors/riscv-newlib.git
$ git clone --recursive https://gitee.com/mirrors/riscv-binutils-gdb.git
$ git clone --recursive https://gitee.com/mirrors/riscv-dejagnu.git
$ git clone --recursive https://gitee.com/mirrors/riscv-glibc.git
// riscv-gcc 超过1G，会clone出错，这里设置拉取最近一次提交的版本
$ git clone --depth 1 https://gitee.com/mirrors/riscv-gcc.git
```
需要注意的是：上面这种分步下载方式和一条命令`git clone --recursive https://github.com/riscv/riscv-gnu-toolchain`下载的文件夹有所不同，它还需要额外的copy操作,得riscv-binutils,riscv-gdb两个目录有数据，否则编译会出错：
```
cd riscv-binutils-gdb
cp -a * ../riscv-gdb
cp -a * ../riscv-binutils
```
4. 配置和编译工具链,编译时间较长，耐心等待：
```
$ cd riscv-gnu-toolchain
$ ./configure --prefix=/home/moocos/riscv-gnu-toolchain/bin  //这里为编译结果的路径
$ make
```
5. 编译后的文件结构如下：

6. 下载qemu 4.1,
```
// 可以执行这个命令下载
$ wget https://download.qemu.org/qemu-4.1.0.tar.xz
$ tar xf qemu-4.1.0.tar.xz
```
可以将`https://download.qemu.org/qemu-4.1.0.tar.xz`复制到浏览器里下载，我这里
下载速度太慢，在CSDN上找了个资源:`https://download.csdn.net/download/existmars/11377629?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522159135190719724846413484%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fdownload.%2522%257D&request_id=159135190719724846413484&biz_id=1&utm_medium=distribute.pc_search_result.none-task-download-2~download~first_rank_v2~rank_dl_v1-1-11377629.pc_v2_rank_dl_v1&utm_term=qemu-4.1.0`,后面放到百度云或者码云上

7. 编译qemu 
```
$ cd qemu-4.1.0
$ ./configure --disable-kvm --disable-werror --prefix=/usr/local --target-list="riscv64-softmmu"
$ make
$ sudo make install
$ cd ..
```
8. 检查是否安装好qemu 和gun tools 工具链
```
$ qemu-system-riscv64 --version
QEMU emulator version 4.1.0
Copyright (c) 2003-2019 Fabrice Bellard and the QEMU Project developers
[~/xv6-riscv-fall19]
$ riscv64-unknown-elf-gcc --version
riscv64-unknown-elf-gcc (GCC) 10.0.1 20200318 (experimental)
Copyright (C) 2020 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
9. 下载mit xv6os， `https://github.com/mit-pdos/xv6-riscv-fall19/tree/util`先fork到自己的github,然后git clone
编译：
```
$ git clone git://github.com/mit-pdos/xv6-riscv-fall19.git
Cloning into 'xv6-riscv'...
...
$ cd xv6-riscv-fall19
$ git checkout util
```
设置 编译结果的路径 到终端的环境变量
```
$ export PATH=$PATH:/home/moocos/riscv-gnu-toolchain/bin //路径可以替换为你自己的路径
//然后编译xv6
$ make
$ make qemu
...
mkfs/mkfs fs.img README user/_cat user/_echo user/_forktest user/_grep user/_init user/_kill user/_ln user/_ls user/_mkdir user/_rm user/_sh user/_stressfs user/_usertests user/_wc user/_zombie user/_cow 
nmeta 46 (boot, super, log blocks 30 inode blocks 13, bitmap blocks 1) blocks 954 total 1000
balloc: first 497 blocks have been allocated
balloc: write bitmap block at sector 45
qemu-system-riscv64 -machine virt -kernel kernel/kernel -m 3G -smp 3 -nographic -drive file=fs.img,if=none,format=raw,id=x0 -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0
hart 0 starting
hart 2 starting
hart 1 starting
```


这篇文章对应MIT_OS 6.S081官方计划的工具准备篇`https://pdos.csail.mit.edu/6.828/2019/tools.html`，万事开头难，记录好搭环境的每一步

