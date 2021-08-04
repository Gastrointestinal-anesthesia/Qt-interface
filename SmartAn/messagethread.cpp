#include "messagethread.hpp"
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

#include <iostream>
#include <QDebug>
#include "mainwindow.h"
#include "anesthesia.h"

#define PORT 7000
#define QUEUE 20//连接请求队列



int sendData(int socket,const char* strdata,int nlen)
{
    return  send(socket, strdata, nlen,0); ///发送

}

MessageThread::MessageThread()
{

}
void MessageThread::setParent(QMainWindow* pthat)
{
    m_pParent = pthat;
}
void MessageThread::run()
{
    int sock_cli = socket(AF_INET, SOCK_STREAM, 0); // 若成功则返回一个sockfd（套接字描述符）
    //printf("%d\n",ss);
    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);  // 服务器端口
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // 服务器ip，inet_addr用于IPv4的IP转换（十进制转换为二进制）

    while(true)
    {
        if (::connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        {
            perror("connect");
            continue;
        }
        else
        {
            break;
        }
    }
    char buffer[1024];
    while(true)
    {
        memset(buffer, 0 ,sizeof(buffer));
        int len = ::recv(sock_cli, buffer, 4, 0);//从TCP连接的另一端接收数据。
        if(len == 4)
        {
            int size = sizeof(smart_topic::Anesthesia);
            len = ::recv(sock_cli, buffer+4, size, 0);//从TCP连接的另一端接收数据。
            if(len == size)
            {
                smart_topic::Anesthesia* pmsg = (smart_topic::Anesthesia*)(buffer+4);
                smart_topic::Anesthesia* pmsgnew =new smart_topic::Anesthesia(*pmsg);
                MainWindow* pThat = (MainWindow*)m_pParent;
                pThat->sendData(pmsgnew);

            }
        }
    }

}


