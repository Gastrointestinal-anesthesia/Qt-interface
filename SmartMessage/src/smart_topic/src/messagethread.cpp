#include "../include/smart_message/messagethread.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <thread>
#include <pthread.h>
#include <iostream>
#define PORT 7000
#define QUEUE 20//连接请求队列
int conn;
extern  pthread_mutex_t g_mutex;

void* thread1(void*);

int sendData(int socket,const char* strdata,int nlen)
{
  return  send(socket, strdata, nlen,0); ///发送
  
}

MessageThread::MessageThread()
{
  
}

void MessageThread::start()
{
  pthread_t tid1;
  int rc1=0;
  rc1 = pthread_create(&tid1, NULL, thread1, this);
}
void MessageThread::run()
{
  int ss = socket(AF_INET, SOCK_STREAM, 0);//若成功则返回一个sockfd（套接字描述符）
  //printf("%d\n",ss);
  struct sockaddr_in server_sockaddr;//一般是储存地址和端口的。用于信息的显示及存储使用
  /*设置 sockaddr_in 结构体中相关参数*/
  server_sockaddr.sin_family = AF_INET;
  server_sockaddr.sin_port = htons(PORT);//将一个无符号短整型数值转换为网络字节序，即大端模式(big-endian)
  //printf("%d\n",INADDR_ANY);
  //INADDR_ANY就是指定地址为0.0.0.0的地址，这个地址事实上表示不确定地址，或“所有地址”、“任意地址”。
  //一般来说，在各个系统中均定义成为0值。
  server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);//将主机的无符号长整形数转换成网络字节顺序。
  if(bind(ss, (struct sockaddr* ) &server_sockaddr, sizeof(server_sockaddr))==-1)
  {
    perror("bind");
    exit(1);
  }
  if(listen(ss, QUEUE) == -1)
  {
    perror("listen");
    exit(1);
  }
  while(1)
  {
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    ///成功返回非负描述字，出错返回-1
    conn = accept(ss, (struct sockaddr*)&client_addr, &length);
    //如果accpet成功，那么其返回值是由内核自动生成的一个全新描述符，代表与所返回客户的TCP连接。
    //accpet之后就会用新的套接字conn
    if( conn < 0 )
    {
      perror("connect");
      exit(1);
    }
    pthread_mutex_lock(&g_mutex);
    m_vecClientSocket.push_back(conn);
    pthread_mutex_unlock(&g_mutex);
  }
}
void* thread1(void* pthis)
{
  MessageThread* pthat = (MessageThread* )pthis;
  pthat->run();
}

