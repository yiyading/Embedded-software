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
<br>
3、主机端linux下载安装树莓派的交叉编译工具链<br>
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
## 四、实验总结
