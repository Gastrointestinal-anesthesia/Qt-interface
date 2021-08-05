/***********************************************************************
Copyright 2020 GuYueHome (www.guyuehome.com).
***********************************************************************/

/**
 * 该例程将发布/person_info话题，自定义消息类型learning_topic::Person
 */

#include <ros/ros.h>
#include <stdlib.h>
#include "smart_topic/Anesthesia.h"

int GetRandomNumber()
{
  int randomNumber;
  srand((unsigned)time(NULL));//time()用系统时间初始化种。为rand()生成不同的随机种子。
  randomNumber = rand() % 100 + 1;//生成1~100随机数
  return randomNumber;
}


int main(int argc, char **argv)
{
  // ROS节点初始化
  ros::init(argc, argv, "anesthesia_publisher");

  // 创建节点句柄
  ros::NodeHandle n;

  // 创建一个 Publisher，发布名为 /anesthesia_info的t opic，消息类型为 smart_topic::Anesthesia，队列长度10
  ros::Publisher anesthesia_info_pub = n.advertise<smart_topic::Anesthesia>("/anesthesia_info", 10);

  // 设置循环的频率
  ros::Rate loop_rate(1);

  int count = 0;
  while (ros::ok())
  {
    // 初始化learning_topic::Person类型的消息
    smart_topic::Anesthesia anesthesia_msg;
    sleep(1);
    anesthesia_msg.RATE = GetRandomNumber();
    sleep(1);
    anesthesia_msg.DIAP  = GetRandomNumber();
    sleep(1);
    anesthesia_msg.SYSP = GetRandomNumber();
    sleep(1);
    anesthesia_msg.SpO2 = GetRandomNumber();
    sleep(1);
    anesthesia_msg.BIS = GetRandomNumber();

    // 发布消息
    anesthesia_info_pub.publish(anesthesia_msg);

    ROS_INFO("Publish Anesthesia Info: RATE:%d  DIAP:%d  SYSP:%d  SpO2:%d  BIS:%d",
             anesthesia_msg.RATE, anesthesia_msg.DIAP,
             anesthesia_msg.SYSP, anesthesia_msg.SpO2,
             anesthesia_msg.BIS);

    // 按照循环频率延时
    loop_rate.sleep();
  }

  return 0;
}
