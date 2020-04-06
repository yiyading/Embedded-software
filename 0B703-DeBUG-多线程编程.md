# 实验题目 多线程编程 
# 小组名称 第13组
# 小组成员：伊亚玎 谢豪 周泽园
# 一、实验目的
1、掌握基本的多线程编程技术

2、掌握基本的线程间同步技术

3、熟练使用pthread线程库调用接口

# 二、实验内容
编写多线程程序解决读者-写者问题：

1. 至少有3个读者和2个写者

2. 每个读者写者进出数据集都要求打印输出信息

3. 实现读者优先策略

4. 实现写者优先策略

# 三、实验过程与结果  
## 实验过程：
### 读者优先情况：
1. 一个写者不能和其他写者或读者同时访问数据集；

由于**一个写者不能和其他写者或读者同时访问数据集**，要实现写者和其他读者写者均互斥，在写者线程中采用互斥锁在写操作区上锁：

```
pthread_mutex_lock(&mutex_write);
printf("写线程id%d进入数据集\n",pthread_self());
write();
rintf("写线程id%d退出数据集\n",pthread_self());
 pthread_mutex_unlock(&mutex_write);
```
保证写操作和所有操作互斥。
2.读操作优先级高于写操作，也就是所有读操作完成后执行写操作。

这就需要一个计数量来判断此时到来的读者为第几个（这里所说的第几个为未退出读者线程的读者），定义为readerCnt；在每个读者到来时给该变量+1，在一个读者线程的读操作结束后即将退出该线程时，再将该变量-1。

```
sem_wait(&sem_read);
readerCnt++;
if(readerCnt == 1){
    pthread_mutex_lock(&mutex_write);
}
sem_post(&sem_read);
printf("读线程id%d进入数据集\n",pthread_self());
read();
printf("读线程id%d退出数据集\n",pthread_self());    
sem_wait(&sem_read);
readerCnt--;
if(readerCnt == 0){
    pthread_mutex_unlock(&mutex_write);
}
sem_post(&sem_read);
```
### 写者优先情况：
1.写者优先只有在写者读者优先级上不同，因此将上面的第二步的两个对象进行调换即可。这次要在写者程序中进行判别，当写者数量=1时，读者进程锁，当写者进程数=0时，才将读者进程unlock。

```
void write()
{
    int rd = rand();
    printf("write %d\n",rd);
    data = rd;
}
void read()
{
    printf("read %d\n",data);
}
void * writer(void * in)
{
//    while(1)
//    {
        sem_wait(&sem_write);
        {//临界区,希望修改 writerCnt,独占 writerCnt
            writerCnt++;
           if(writerCnt == 1){
                //阻止后续的读者加入待读队列
               pthread_mutex_lock(&mutex_read);
           }
        }
       sem_post(&sem_write);
         
        
        pthread_mutex_lock(&mutex_write);
        {//临界区，限制只有一个写者修改数据
            printf("写线程id%d进入数据集\n",pthread_self());
            write();
            printf("写线程id%d退出数据集\n",pthread_self());        
        }
        pthread_mutex_unlock(&mutex_write);
        
        sem_wait(&sem_write);
        {//临界区,希望修改 writerCnt,独占 writerCnt
            writerCnt--;
            if(writerCnt == 0){
                //阻止后续的读者加入待读队列
                pthread_mutex_unlock(&mutex_read);
            }
        }
        sem_post(&sem_write);
        sleep(W_SLEEP);
//    }
    pthread_exit((void *) 0);
}
```
2.但是这里有一个问题，可能有成千上万的读者被阻塞在 pthread_mutex_lock(&mutex_read)处，若在写者线程全部退出并释放mutex_readv时，又有新的写者到来，将导致该写者和成千上万个读者共同竞争该互斥锁，这将很可能导致新到来的写者线程的饥饿，与写者优先的原则相违背，为了解决这一问题，可以在读者线程中再添加一把互斥锁mutex_write：
```
void * reader (void * in)
{
//    while(1)
//    {
        //假如写者锁定了mutex_read,那么成千上万的读者被锁在这里
            pthread_mutex_lock(&mutex_read);//只被一个读者占有
            {//临界区
                sem_wait(&sem_read);//代码段 1
                {//临界区
                    readerCnt++;
                    if(readerCnt == 1){
                        pthread_mutex_lock(&mutex_write);
                    }
                }
                sem_post(&sem_read);
            }
            pthread_mutex_unlock(&mutex_read);//释放时,写者将优先获得mutex_read
        printf("读线程id%d进入数据集\n",pthread_self());
        read();
        printf("读线程id%d退出数据集\n",pthread_self());
        sem_wait(&sem_read);//代码段2
        {//临界区
            readerCnt--;
            if(readerCnt == 0){
                pthread_mutex_unlock(&mutex_write);//在最后一个并发读者读完这里开始禁止写者执行写操作
            }
        }
        sem_post(&sem_read);
        
        sleep(R_SLEEP);
//    }
    pthread_exit((void *) 0);
}
```
## 实验结果：
### 读者优先
![图片](https://uploader.shimo.im/f/O53CAxY9IB8gwKk3.PNG!thumbnail)

### 写者优先：
![图片](https://uploader.shimo.im/f/Uk5E20WV72AllqrC.PNG!thumbnail)

# 四、实验总结
## 实验收获：
了解基本的多线程编程技术

掌握基本的线程间同步技术

熟练使用pthread线程库调用接口

对于互斥锁的使用经过较长时间的理解和探索，在不同进程中对于互斥锁的原理进行了研究理解。

# 五、实验源码
```
/*
*  多线程,读者优先
*/


#include "stdio.h"
#include <stdlib.h>
#include <pthread.h>
#include<semaphore.h>




#define N_WRITER 30 //写者数目
#define N_READER 5 //读者数目
#define W_SLEEP  1 //控制写频率
#define R_SLEEP  1 //控制读频率




pthread_t wid[N_WRITER],rid[N_READER];
pthread_mutex_t mutex_write;//同一时间只能一个人写文件,互斥
sem_t sem_read;//同一时间只能有一个人访问 readerCnt
int data = 0;
int readerCnt = 0;
void write()
{
    int rd = rand();
    printf("write %d\n",rd);
    data = rd;
}
void read()
{
    printf("read %d\n",data);
}
void * writer(void * in)
{
//    while(1)
//    {
        pthread_mutex_lock(&mutex_write);
        printf("写线程id%d进入数据集\n",pthread_self());
        write();
        printf("写线程id%d退出数据集\n",pthread_self());
        pthread_mutex_unlock(&mutex_write);
        sleep(W_SLEEP);
//    }
    pthread_exit((void *) 0);
}


void * reader (void * in)
{
//    while(1)
//    {
        sem_wait(&sem_read);
        readerCnt++;
        if(readerCnt == 1){
            pthread_mutex_lock(&mutex_write);
        }
        sem_post(&sem_read);
        printf("读线程id%d进入数据集\n",pthread_self());
        read();
        printf("读线程id%d退出数据集\n",pthread_self());    
        sem_wait(&sem_read);
        readerCnt--;
        if(readerCnt == 0){
            pthread_mutex_unlock(&mutex_write);
        }
        sem_post(&sem_read);
        sleep(R_SLEEP);
//    }
    pthread_exit((void *) 0);
}


int main()
{
    printf("多线程,读者优先\n");    
    pthread_mutex_init(&mutex_write,NULL);
    sem_init(&sem_read,0,1);
    int i = 0;
    for(i = 0; i < N_WRITER; i++)
    {
        pthread_create(&wid[i],NULL,writer,NULL);
    }
        for(i = 0; i < N_READER; i++)
    {
        pthread_create(&rid[i],NULL,reader,NULL);
    }
    sleep(1);
    return 0;
}


读者优先
```
```
#include "stdio.h"
#include <stdlib.h>
#include <pthread.h>
#include<semaphore.h>


#define N_WRITER 5 //写者数目
#define N_READER 20 //读者数目
#define W_SLEEP 1 //控制写频率
#define R_SLEEP  0.5 //控制读频率




pthread_t wid[N_WRITER],rid[N_READER];
int data = 0;
int readerCnt = 0, writerCnt = 0;
pthread_mutex_t sem_read;
pthread_mutex_t sem_write;
pthread_mutex_t mutex_write;
pthread_mutex_t mutex_read;


void write()
{
    int rd = rand();
    printf("write %d\n",rd);
    data = rd;
}
void read()
{
    printf("read %d\n",data);
}
void * writer(void * in)
{
//    while(1)
//    {
        sem_wait(&sem_write);
        {//临界区,希望修改 writerCnt,独占 writerCnt
            writerCnt++;
            if(writerCnt == 1){
                //阻止后续的读者加入待读队列
                pthread_mutex_lock(&mutex_read);
            }
        }
        sem_post(&sem_write);
        
        
        pthread_mutex_lock(&mutex_write);
        {//临界区，限制只有一个写者修改数据
            printf("写线程id%d进入数据集\n",pthread_self());
            write();
            printf("写线程id%d退出数据集\n",pthread_self());        
        }
        pthread_mutex_unlock(&mutex_write);
        
        sem_wait(&sem_write);
        {//临界区,希望修改 writerCnt,独占 writerCnt
            writerCnt--;
            if(writerCnt == 0){
                //阻止后续的读者加入待读队列
                pthread_mutex_unlock(&mutex_read);
            }
        }
        sem_post(&sem_write);
        sleep(W_SLEEP);
//    }
    pthread_exit((void *) 0);
}


void * reader (void * in)
{
//    while(1)
//    {
        //假如写者锁定了mutex_read,那么成千上万的读者被锁在这里
            pthread_mutex_lock(&mutex_read);//只被一个读者占有
            {//临界区
                sem_wait(&sem_read);//代码段 1
                {//临界区
                    readerCnt++;
                    if(readerCnt == 1){
                        pthread_mutex_lock(&mutex_write);
                    }
                }
                sem_post(&sem_read);
            }
            pthread_mutex_unlock(&mutex_read);//释放时,写者将优先获得mutex_read
        printf("读线程id%d进入数据集\n",pthread_self());
        read();
        printf("读线程id%d退出数据集\n",pthread_self());
        sem_wait(&sem_read);//代码段2
        {//临界区
            readerCnt--;
            if(readerCnt == 0){
                pthread_mutex_unlock(&mutex_write);//在最后一个并发读者读完这里开始禁止写者执行写操作
            }
        }
        sem_post(&sem_read);
        
        sleep(R_SLEEP);
//    }
    pthread_exit((void *) 0);
}


int main()
{
    printf("多线程,写者优先\n");
    pthread_mutex_init(&mutex_write,NULL);
    pthread_mutex_init(&mutex_read,NULL);
    sem_init(&sem_write,0,1);
    sem_init(&sem_read,0,1);
int i = 0;
    for(i = 0; i < N_READER; i++)
    {
        pthread_create(&rid[i],NULL,reader,NULL);
    }
    for(i = 0; i < N_WRITER; i++)
    {
        pthread_create(&wid[i],NULL,writer,NULL);
    }
    sleep(1);    
    return 0;
}


写者优先
```
