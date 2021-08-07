#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QMainWindow>
#include <QThread>
#include <QObject>
#include <QVariant>


class MessageThread: public QThread
{
  //Q_OBJECT


public:
  MessageThread();
  void setParent(QMainWindow*);
protected:
    void run() ;
private:
    QMainWindow* m_pParent;
};

#endif // MESSAGETHREAD_H
