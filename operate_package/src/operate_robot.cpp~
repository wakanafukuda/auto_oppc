//配列に値を代入する前にclear()しないと直前のデータが残る。

#include <stdio.h>
#include <stdlib.h>

#include "ros/ros.h"
#include "msg2015/CmdPWM.h"
#include "msg2015/Sensors2015.h"

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"

#include "std_msgs/Int16MultiArray.h"
#include "std_msgs/Int16.h"
#include "std_msgs/String.h"

#define STOP 0
#define START 1
#define PAUSE 2

#define FORWARD 50
#define REVERSE -50

#define TIMEBASE 15

using namespace std;

ros::Publisher ino_pub;
ros::Subscriber ino_sub;
//ros::Publisher sen_pub;
//ros::Subscriber sen_sub;
ros::Subscriber state_sub;

std_msgs::Int16MultiArray r_motor;
std_msgs::Int16 b_value;
static int state = 0;
msg2015::Sensors2015 sdt;

void moveMotor(const msg2015::CmdPWM msg){
  int i;
  int sleep_time = 0;

  //Turn  
  r_motor.data.clear();
  if(msg.Turn > 0){
    for(i = 0;i < 2;i++){
      if(i == 0){
        r_motor.data.push_back(FORWARD);
      }else if(i == 1){
        r_motor.data.push_back(REVERSE);
      }
    }
    sleep_time = msg.Turn / TIMEBASE;
  }else if(msg.Turn < 0){
    for(i = 0;i < 2;i++){
      if(i == 0){
        r_motor.data.push_back(REVERSE);
      }else if(i == 1){
        r_motor.data.push_back(FORWARD);
      }
    }
    sleep_time = (-msg.Turn) / TIMEBASE;
  }else if(msg.Turn == 0){
    for(i = 0;i < 2;i++){
      r_motor.data.push_back(STOP);
    }
  }
  ino_pub.publish(r_motor);
  sleep(sleep_time);

  //Go straight
  if(msg.Forward != 0){
  r_motor.data.clear();
  for(i = 0;i < 2;i++){
    r_motor.data.push_back(FORWARD);
  }
  ino_pub.publish(r_motor);
  sleep(2);
  }

  //Stop
  r_motor.data.clear();
  for(i = 0;i < 2;i++){
    r_motor.data.push_back(STOP);
  }
  ino_pub.publish(r_motor);

}
void movementCallBack(const msg2015::CmdPWM msg){
  int i; 
/*
  static int flag = 0;
  if(flag == 0){
    flag = 1;
  }else if(flag == 1){
    if(state == 0 || state == 2){
      r_motor.data.clear();
      for(i = 0;i < 2;i++){
        r_motor.data.push_back(STOP);
      }
      ino_pub.publish(r_motor);
    }else if(state == 1){
      moveMotor(msg);
      flag = 0;
    }
  }
  sleep(1);
*/
  if(state == 0 || state == 2){
    r_motor.data.clear();
    for(i = 0;i < 2;i++){
      r_motor.data.push_back(STOP);
    }
    ino_pub.publish(r_motor);
  }else if(state == 1){
    moveMotor(msg);
  }

//デバッグ用
//  ROS_INFO("I subscribed : [ %d, %d]", msg->Turn, msg->Forward);
/*
  int i;
  int dt[2];
  dt[0] = msg->Turn;
  dt[1] = msg->Forward;
  r_motor.data.clear();
  for(i = 0;i < 2;i++){
    r_motor.data.push_back(dt[i]);
  }

  ino_pub.publish(r_motor);
*/

}

void stateCallBack(const std_msgs::String::ConstPtr& msg){
  if( (msg->data) == "stop"){
    state = 0;
  }else if( (msg->data) == "start"){
    state = 1;
  }else if( (msg->data) == "pause"){
    state = 2;
  }
  ROS_INFO("I heard : [ %s ]", msg->data.c_str());
  ROS_INFO("I heard : [ %d ]", state);
  
}

/*
void batteryCallBack(const std_msgs::Int16 msg){
  sdt.BV = msg.data;
  sdt.CO2 = 0;
  sen_pub.publish(sdt);
}
*/

int main(int argc, char** argv){
  ros::init(argc, argv, "operate_robot");
  ros::NodeHandle n;

  ino_pub = n.advertise<std_msgs::Int16MultiArray>("send_arduino",1000);
//  ino_pub = n.advertise<msg2015::CmdPWM>("send_arduino",1000);
  ino_sub = n.subscribe("movement",1000,movementCallBack);

//  sen_pub = n.advertise<msg2015::Sensors2015>("sensors",1000);
//  sen_sub = n.subscribe("chatterbattery",1000,batteryCallBack);

  state_sub = n.subscribe("state",1000,stateCallBack);


  while(ros::ok()){
    ros::spinOnce();
  }
  return 0;
}
