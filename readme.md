# EFServer调试指南
## makefile
```mk
#-----------------------------------------------------------------------
# 服务名称
APP       := Center
TARGET    := EFServer
# tar打包成tgz压缩文件时包含的文件
CONFIG    := ./lib/libthosttraderapi.so ./config.conf
STRIP_FLAG:= N
TARS2CPP_FLAG:=

INCLUDE   +=
# 依赖的三方动态库，其中`-Wl,-R .`编译选项设置了动态库的运行时搜索路径
LIB       += -L./lib -lthosttraderapi -Wl,-R .

#-----------------------------------------------------------------------
# 载入tars框架提供的makefile文件
include /works/evn_tars/cpp/makefile/makefile.tars

#-----------------------------------------------------------------------
```

## 文件依赖
### 编译依赖
 * 编译过程中的头文件依赖关系在代码中通过相对路径的方式来解决，不在makefile文件中处理。
 * LINK过程中类、函数等符号的依赖关系在makefile文件中通过配置`LIB += -L./lib -lthosttraderapi`来解决，其中`-L`表示动态库所在路径、`-l`表示动态库的名字。

### 运行时依赖
为了能在发布过程中方便的替换三方的交易动态库，我们不将`thosttraderapi`放到系统的lib搜索目录，而是通过g++的编译选项`-Wl,-R .`来讲应用程序的当前目录设置成为动态库优先搜索目录。

* ldd命令查看可执行文件所需要的动态链接库
```sh
[root@devs EFServer]# ldd EFServer
	linux-vdso.so.1 =>  (0x00007ffdb1188000)
	libthosttraderapi.so => not found
	libpthread.so.0 => /lib64/libpthread.so.0 (0x00007f2b6e556000)
	libstdc++.so.6 => /lib64/libstdc++.so.6 (0x00007f2b6e24f000)
	libm.so.6 => /lib64/libm.so.6 (0x00007f2b6df4d000)
	libgcc_s.so.1 => /lib64/libgcc_s.so.1 (0x00007f2b6dd36000)
	libc.so.6 => /lib64/libc.so.6 (0x00007f2b6d973000)
	/lib64/ld-linux-x86-64.so.2 (0x0000560956548000)
```

* strace命令可以跟踪操作系统把程序`program`变成进程`process`的过程
  - fork进程，在内核创建进程相关内核想，加载进程可执行文件
  - 查找依赖的so，并把它映射到虚拟地址空间
  - 初始化程序变量

```sh
[root@devs EFServer]# strace -t ./EFServer
15:04:07 execve("./EFServer", ["./EFServer"], [/* 25 vars */]) = 0
15:04:07 brk(NULL)                      = 0x1139000
15:04:07 mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f0120d40000
15:04:07 access("/etc/ld.so.preload", R_OK) = -1 ENOENT (No such file or directory)
15:04:07 open("./tls/x86_64/libthosttraderapi.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
```

* readelf命令查看可执行文件动态库的搜索路径
```sh
[root@devs EFServer]# readelf -d EFServer

Dynamic section at offset 0x136dc8 contains 30 entries:
  Tag        Type                         Name/Value
 0x0000000000000001 (NEEDED)             Shared library: [libthosttraderapi.so]
 0x0000000000000001 (NEEDED)             Shared library: [libpthread.so.0]
 0x0000000000000001 (NEEDED)             Shared library: [libstdc++.so.6]
 0x0000000000000001 (NEEDED)             Shared library: [libm.so.6]
 0x0000000000000001 (NEEDED)             Shared library: [libgcc_s.so.1]
 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
 0x000000000000000f (RPATH)              Library rpath: [.]
 0x000000000000000c (INIT)               0x405970
 0x000000000000000d (FINI)               0x4dfbac
 0x0000000000000019 (INIT_ARRAY)         0x736bd8
 0x000000000000001b (INIT_ARRAYSZ)       480 (bytes)
 0x000000000000001a (FINI_ARRAY)         0x736db8
 0x000000000000001c (FINI_ARRAYSZ)       8 (bytes)
 0x000000006ffffef5 (GNU_HASH)           0x400298
 0x0000000000000005 (STRTAB)             0x402000
 0x0000000000000006 (SYMTAB)             0x400458
 0x000000000000000a (STRSZ)              7120 (bytes)
 0x000000000000000b (SYMENT)             24 (bytes)
 0x0000000000000015 (DEBUG)              0x0
 0x0000000000000003 (PLTGOT)             0x737000
 0x0000000000000002 (PLTRELSZ)           6096 (bytes)
 0x0000000000000014 (PLTREL)             RELA
 0x0000000000000017 (JMPREL)             0x4041a0
 0x0000000000000007 (RELA)               0x403f30
 0x0000000000000008 (RELASZ)             624 (bytes)
 0x0000000000000009 (RELAENT)            24 (bytes)
 0x000000006ffffffe (VERNEED)            0x403e20
 0x000000006fffffff (VERNEEDNUM)         5
 0x000000006ffffff0 (VERSYM)             0x403bd0
 0x0000000000000000 (NULL)               0x0
```

## 调试参数
### 配置项
tars开发的服务可以脱离于tars框架单独运行，不过需要将配置文件以应用程序启动项的方式传递进去，配置项的具体含义请参见`config.conf`文件中的说明。
```sh
cd ~
rm -rf EFServer
cp /works/Center/EFServer/EFServer.tgz ./ -y

tar -zxf EFServer.tgz
cd EFServer
./EFServer --config=config.conf
```

```sh
cd ~
rm -rf ProxyServer
cp /works/Center/EFProxy/ProxyServer.tgz ./ -y

tar -zxf ProxyServer.tgz
cd ProxyServer
./ProxyServer --config=config.conf
```

```sh
gdb EFServer
set args --config=config.conf
set scheduler-locking on

b TraderServantBase::userLogin
```

```sh
./TestClient --c="Center.EFServer.TraderServantObj@tcp -h 192.168.3.167 -p 21111 -t 1000" --t=1 --s=1 --n=1
```

目前UserID、AccountID、InvestorID，如果要填，都填一样的

### 远端地址
| 类型     | 名称                     | 地址           | 端口 | BrokerID |
| -------- | ------------------------ | -------------- | ---- | -------- |
| 实盘     | 逸富内盘云中心交易端     | 106.15.10.58   | 9611 | 8000     |
| 模拟盘   | 逸富内盘公测交易端       | 120.55.162.84  | 9609 | 8001     |
| SP模拟盘 | 我们对接的SP盘模拟交易端 | 101.132.17.212 | 9112 |          |

### 模拟账号
| 账号    | 密码   | 额度 |
| ------- | ------ | ---- |
| 6612125 | 888888 | 20W  |
| 6612126 | 888888 | 20W  |
| 6612127 | 888888 | 20W  |

## 发布到TARS框架
