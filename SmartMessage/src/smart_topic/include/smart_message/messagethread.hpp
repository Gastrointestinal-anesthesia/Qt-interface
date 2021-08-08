#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <vector>

int sendData(int socket,const char* strdata,int nlen);
class MessageThread
{
public:
  MessageThread();
public:
  std::vector<int> m_vecClientSocket;
public:
  
  void start();
  virtual void run() ;
};

#endif // MYTHREAD_H
