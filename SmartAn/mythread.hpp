#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QMainWindow>
#include <QThread>
#include <QObject>
#include <QVariant>


class MyThread: public QThread
{
  //Q_OBJECT


public:
  MyThread();
  void setParent(QMainWindow*);
protected:
    void run() ;
private:
    QMainWindow* m_pParent;
};

#endif // MYTHREAD_H
