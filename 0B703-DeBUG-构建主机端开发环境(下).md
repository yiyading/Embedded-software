# 构建主机端开发环境(下)
# 第13组 DeBUG小组
# 谢豪 伊亚玎 周泽园
## 一、实验目的
1、学会下载安装Linux及相关工具到目标机或目标机仿真环境。<br>
2、学会使用交叉编译器编译应用程序，并能够将可执行程序下载到目标机运行。<br>
3、学会使用交叉调试工具gdb-gdbserver调试应用程序。<br>
## 二、实验内容
1、Host-Target <br>
* 方案1：下载安装Linux系统到开发板，其中包括bootloader、内核、模块、库、文件系统等；建立host-target连接 ，常见连接有SSH，VNC，Putty。<br> 
* 方案2：下载安装目标机仿真环境qemu，安装qemu树莓派系统。 <br>

2、构建并测试开发板交叉编译器。<br>
3、建立gdb-gdbserver交叉调试环境并能够调试应用程序。<br>
## 三、实验过程与结果
1、树莓派安装linux系统<br>
&emsp;&emsp;实验所用树莓派型号为树莓派3B，首先[从官网下载树莓派镜像](https://www.raspberrypi.org/downloads/raspbian/ "https://www.raspberrypi.org/downloads/raspbian/")，选择
[Raspbian Buster with desktop and recommended software](https://downloads.raspberrypi.org/raspbian_full_latest.torrent)下载,用Win32DiskImager将镜像刷写到SD卡中，将插入树莓派卡槽，接通电源，通过HDMI连接显示器，树莓派自动开机并显示桌面。<br>
2、SSH连接树莓派<br>
首先配置树莓派的网络，需要给树莓派连接无线网并配置静态IP。<br>
①输入
```bash
sudo vim /etc/wpa_supplicant/wpa_supplicant.conf
```
在尾部添加<br>
```
network={
ssid="ssid"
psk="xxxx"
}
```
②配置静态IP,输入
```bash
sudo vim /etc/dhcpcd.conf
```
在文件末尾添加:<br>
```
interface wlan0
static ip_address=192.168.0.22/24
static routers=192.168.0.1
static domain_name_servers=192.168.0.1
```
③SSH连接树莓派<br>
用XShell软件通过SSH连接树莓派，首先设置树莓派IP为192.168.0.22，选择协议为SSH，然后设置登录用户名和密码为pi，即可连接树莓派。<br>
![ssh配置](https://github.com/yiyading/Embedded-software/blob/master/20200420_src/img/ssh.png)<br>
![ssh连接结果](https://github.com/yiyading/Embedded-software/blob/master/20200420_src/img/ssh2.png)<br>
<br>
2、主机端linux下载安装树莓派的交叉编译工具链<br>
①下载树莓派交叉编译工具<br>
```bash
git clone git://github.com/raspberrypi/tools.git
cd /home/xiehao/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
ls
```
![工具下载](https://github.com/yiyading/Embedded-software/blob/master/20200420_src/img/crosslink.png)<br>
②把编译器路径加入到系统环境变量<br>
```bash
export PATH=$PATH:/home/xiehao/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
```
③用交叉编译工具 arm-linux-gnueabihf-gcc 编译程序<br>
继续使用之前的排序程序，编写makefile:<br>
![makefile](https://github.com/yiyading/Embedded-software/blob/master/20200420_src/img/makefilw.png)<br>
make之后生成可执行文件，在host端无法运行，将可执行文件复制到树莓派上则可以运行：<br>
![make结果](https://github.com/yiyading/Embedded-software/blob/master/20200420_src/img/make.png)<br>
![主机端运行结果](https://github.com/yiyading/Embedded-software/blob/master/20200420_src/img/proj.png)<br>
![树莓派运行结果](https://github.com/yiyading/Embedded-software/blob/master/20200420_src/img/raspberry.png)<br>
<br>
3、使用gdb/gdbserver调试程序<br>
①编译出arm-linux-gdb<br>
下载gdb：
```bash
wget http://ftp.gnu.org/gnu/gdb/gdb-8.0.tar.gz
tar -zxvf gdb-8.0.tar.gz
cd gdb-8.0.tar
```
编译出arm-linux-gdb，安装在host端:<br>
```bash
./configure --target=arm-linux --prefix=/xxx/arm-gdb
make
make install
```
![arm-linux-gdb](https://github.com/yiyading/Embedded-software/blob/master/20200420_src/img/arm-linux-gdb.png)<br>
添加环境变量<br>
```bash
export PATH=$PATH:/usr/local/arm-gdb/bin
```
②编译出gdbserver:<br>
```bash
cd /home/xiehao/gdb-8.0/gdb/gdbserver
./configure --target=arm-linux-gnueabihf --host=arm-linux-gnueabihf
make CC=arm-linux-gnueabihf-gcc
```
③利用arm-linux-gdb和gdbserver调试程序<br>
首先在树莓派上运行gdbserver:
```bash
gdbserver :7788 myproj
```
![gdbserver](https://github.com/yiyading/Embedded-software/blob/master/20200420_src/img/gdbserver.png)<br>
之后在host端运行arm-linux-gdb
```
arm-linux-gdb myproj
```
target remote 192.168.0.22:7788<br>
continue<br>
host端运行gdb卡死在这里，不知道如何解决：<br>
![host端调试结果](https://github.com/yiyading/Embedded-software/blob/master/20200420_src/img/arm-linuxgdb.png)<br>
## 四、实验总结
1、ubuntu18.04编译gdb6.6会出错，我们不知道怎么解决，将gdb换成gdb 8.0后并在configure中加入--disable-werror，再次编译就可以成功编译<br>
2、host端出现 Remote 'g' packet reply is too long的问题：<br>
修改gdb/remote.c文件，屏蔽process_g_packet函数中的下列两行：<br>
```c
if (buf_len > 2 * rsa->sizeof_g_packet)

error (_(“Remote ‘g’ packet reply is too long: %s”), rs->buf);
```
在后面添加:<br>
```c
if (buf_len > 2 * rsa->sizeof_g_packet) {
      rsa->sizeof_g_packet = buf_len ;
      for (i = 0; i < gdbarch_num_regs (gdbarch); i++)
      {
         if (rsa->regs[i].pnum == -1)
         continue;

         if (rsa->regs[i].offset >= rsa->sizeof_g_packet)
         rsa->regs[i].in_g_packet = 0;
         else
         rsa->regs[i].in_g_packet = 1;
      }
   }
 ```
3、如果使用 arm-linux-gnueabihf-gcc的交叉编译工具，需要在./configure时配置--target=arm-linux-gnueabihf <br>
