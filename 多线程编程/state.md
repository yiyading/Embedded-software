# 进程和线程的区别
**进程是资源分配的最小单位，线程是CPU调度的最小单位**

做个简单的比喻：进程=火车，线程=车厢<br>

* 线程在进程下行进（单纯的车厢无法运行）<br>

* 一个进程可以包含多个线程（一辆火车可以有多个车厢）<br>

* 不同进程间数据很难共享（一辆火车上的乘客很难换到另外一辆火车，比如站点换乘）<br>

* 同一进程下不同线程间数据很易共享（A车厢换到B车厢很容易）<br>

* 进程要比线程消耗更多的计算机资源（采用多列火车相比多个车厢更耗资源）<br>

* 进程间不会相互影响，一个线程挂掉将导致整个进程挂掉（一列火车不会影响到另外一列火车，但是如果一列火车上中间的一节车厢着火了，将影响到所有车厢）<br>

* 进程可以拓展到多机，进程最多适合多核（不同火车可以开在多个轨道上，同一火车的车厢不能在行进的不同的轨道上）<br>

* 进程使用的内存地址可以上锁，即一个线程使用某些共享内存时，其他线程必须等它结束，才能使用这一块内存。（比如火车上的洗手间）－"互斥锁"<br>

* 进程使用的内存地址可以限定使用量（比如火车上的餐厅，最多只允许多少人进入，如果满了需要在门口等，等有人出来了才能进去）－“信号量”<br>

# 内核线程
* 由内核创建和撤销<br>

* 只运行在**内核态(Kernel Modei)**<br>

* 只使用大于PAGE_OFFSET的线性地址空间<br>

* Regular processes use all 4G of linear addresses, in either User Mode or Kernel Mode (通过系统调用进入内核)<br>

在Linux中，线程被视为一个与其它进程共享某些资源的进程，每个线程都拥有唯一属于自己的task_struct。所以，Linux线程只是一种共享资源 (包括地址空间)的手段。<br>
**线程是一类特殊的进程**<br>

pthread类函数的头文件及链接库如下：<br>
> #include<pthread.h><br>
> #include<semaphore.h><br>
> gcc -o mycode mycode.c -lpthread -lm

# 一、创建线程
## 1. pthread_creat() 
> 线程的创建，类似于fork()，vfork()，但创建线程。<br>
> clone()创建的是一个LWP，内核可见，由内核调度。<br>
> pthread_create()创建的是一个**用户线程**，内核不可见，由线程库调度，由内核调度需设置绑定属性。<br>

函数原型：<br>
```c
int pthread_create (
	pthread_t *thread_tid,
	const pthread_attr_t *attr,
	void * ( * start_routine )( void * ),
	void *arg
);
```
返回值：成功返回0，失败返回错误码<br>

第一个参数：如果新线程创建成功，参数thread_tid 返回新生成的线程的ID,一个进程中的每个线程都有一个线程ID，其类型为pthread_t；<br>

第二个参数：指向线程属性的指针，每个线程都有很多属性，包括优先级，起始栈大小，是否是守护线程等，通常将attr参数的值设置为NULL，这是使用系统默认的属性。<br>

第三个参数：处理线程函数的地址，该函数必须是一个静态的函数，该参数使用了一个通用指针arg；<br>

第四个参数：start_routine ()中的参数，如果想传递多个参数的话，需要将多个参数打包成结构体，然后将arg指向该结构体。

## 2.pthread_join()
> pthread_join()是线程阻塞函数，调用者将处于等待状态一直到被等待的线程结束。<br>
> pthread_join()一般由主线程来调用，等待各个创建的子线程退出。<br>
> pthread_exit()一般由子线程调用，用来结束当前线程。<br>

函数原型：<br>
```c
int pthread_join(pthread_t thread,void **thread_ret);
```
返回值：成功返回0，失败返回错误代码，返回的内容存储在void \*\*thread_ret中。<br>

函数描述：<br>
**该函数类似于进程调用的wait()函数**，以阻塞的方式等待参数thread指定的线程结束。函数返回时，被等待的线程资源被回收，如果被等待的线程之前已经结束，该函数会立即返回。<br>

第一个参数：等待线程的标识符或ID，即pthread_create()第一个参数的值。<br>

## 3.pthread_exit()
> 线程的退出，线程通过该函数来终止执行，就像进程在结束时候调用exit()函数一样。<br>
> **注意**：在线程中要避免使用进程exit()函数，它会使进程中的所有线程退出！<br>

函数原型：<br>
```c
void pthread_exit(void* retval);<br>
```
返回值存储在void *retval中。<br>

## 创建线程的源代码
[pthread.c](https://github.com/yiyading/Embedded-software/blob/master/%E5%A4%9A%E7%BA%BF%E7%A8%8B%E7%BC%96%E7%A8%8B/pthread.c)

## POSIX信号量
