编写一个makefile文件，用来编译child.c文件和parent.c文件

child: child.c parent

  gcc -o $@ $<
  
parent: parent.c

  gcc -o $@ $<

parent中的sleep函数使得父进程进入睡眠，然后进入子进程，执行execv函数。在execv中执行目录下被编译出来的child函数，此时parent中的父进程进入睡眠，而子进程用来执行child函数。

当parent中的主进程从睡眠中醒来，通过pid向parent中子进程发送信号。parent中的子进程将从父进程中接受到的信号，发送个child中
