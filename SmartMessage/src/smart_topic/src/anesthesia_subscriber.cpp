/***********************************************************************
Copyright 2020 GuYueHome (www.guyuehome.com).
***********************************************************************/

/**
 * 该例程将订阅/person_info话题，自定义消息类型learning_topic::Anesthesia
 */

#include <ros/ros.h>
#include <pthread.h>
#include "smart_topic/Anesthesia.h"
#include "../include/smart_message/messagethread.hpp"

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
MessageThread g_thread;
// 接收到订阅的消息后，会进入消息回调函数
void anesthesiaInfoCallback(const smart_topic::Anesthesia::ConstPtr& msg)
{
  // 将接收到的消息打印出来
  ROS_INFO("Subcribe Anesthesia Info: RATE:%d  DIAP:%d  SYSP:%d  SpO2:%d  BIS:%d",
           msg->RATE, msg->DIAP,
           msg->SYSP, msg->SpO2,
           msg->BIS);
  smart_topic::Anesthesia* pmsg =new smart_topic::Anesthesia(*msg);
  int nleng = sizeof(smart_topic::Anesthesia);
  char* strData = new char[4 + nleng];
  memcpy(strData,(char*)&nleng,4);
  memcpy(strData+4,(void*)pmsg,nleng);
  pthread_mutex_lock(&g_mutex);
  for(auto it = g_thread.m_vecClientSocket.begin();it != g_thread.m_vecClientSocket.end();++it)
  {
    int sclient = *it;
    sendData(sclient, strData, 4 + nleng); ///发送
  }
  pthread_mutex_unlock(&g_mutex);
  delete pmsg;
}

int main(int argc, char **argv)
{
  // 初始化ROS节点
  ros::init(argc, argv, "anesthesia_subscriber");

  // 创建节点句柄
  ros::NodeHandle n;

  // 创建一个Subscriber，订阅名为/anesthesia_info的topic，注册回调函数 anesthesiaInfoCallback
  ros::Subscriber anesthesia_info_sub = n.subscribe("/anesthesia_info0", 10, anesthesiaInfoCallback);

  g_thread.start();
  // 循环等待回调函数
  ros::spin();

  return 0;
}
