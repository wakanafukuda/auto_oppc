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

#define STOP 0
#define START 1
#define PAUSE 2

#define FORWARD 50
#define REVERSE -50

#define TIMEBASE 15

using namespace std;

ros::Publisher ino_pub;
ros::Publisher sen_pub;
ros::Subscriber sub_state;
ros::Subscriber sub_sen;

std_msgs::Int16MultiArray r_motor;
msg2015::Sensors2015 sdt;
int robot_mode = 0;

void moveMotor(msg2015::CmdPWM msg){
  int i;
  int sleep_time = 0;

  r_motor.data.clear();
  if(msg.Turn > 0){//turn right
    for(i = 0;i < 2;i++){
      if(i == 0){
        r_motor.data.push_back(FORWARD);
      }else if(i == 1){
        r_motor.data.push_back(REVERSE);
      }
    }
    sleep_time = msg.Turn / TIMEBASE;
  }else if(msg.Turn < 0){//turn left
    for(i = 0;i < 2;i++){
      if(i == 0){
        r_motor.data.push_back(REVERSE);
      }else if(i == 1){
        r_motor.data.push_back(FORWARD);
      }
    }
    sleep_time = (-(msg.Turn)) / TIMEBASE;
  }else if(msg.Turn == 0){
    for(i = 0;i < 2;i++){
      r_motor.data.push_back(STOP);
    }
  }
  ino_pub.publish(r_motor);
  sleep(sleep_time);

  //go straight
  r_motor.data.clear();
  for(i = 0;i < 2;i++){
    r_motor.data.push_back(FORWARD);
  }
  ino_pub.publish(r_motor);
  sleep(1);

  //stop
  r_motor.data.clear();
  for(i = 0;i < 2;i++){
    r_motor.data.push_back(STOP);
  }
  ino_pub.publish(r_motor);

}

void movementCallBack(const msg2015::CmdPWM msg){
  static int flag = 0;
  int i;

  if(flag == 0){
    flag = 1;
  }else if(flag == 1){
    if(robot_mode == 0 || robot_mode == 2){ //stop or pause
      r_motor.data.clear();
      for(i = 0;i < 2;i++){
        r_motor.data.push_back(STOP);
      }
      ino_pub.publish(r_motor);
    }else if(robot_mode == 1){ //start
      moveMotor(msg);
      flag = 0;
    }
  }
}

void batteryCallBack(const std_msgs::Int16 msg){
//  sdt.BV = msg->data;
  sdt.BV = 0;
  sdt.CO2 = 0;
}


int main(int argc, char** argv){
  ros::init(argc,argv,"operate_robot");
  ros::NodeHandle n;

  sub_state = n.subscribe("movement",1000,movementCallBack);
  ino_pub = n.advertise<std_msgs::Int16MultiArray>("send_arduino",1000);

  sub_sen = n.subscribe("chatterbattery",1000,batteryCallBack);
  sen_pub = n.advertise<msg2015::Sensors2015>("sensors",1000);

  while(ros::ok()){
    n.getParam("/robot_state_param_value",robot_mode);
    sen_pub.publish(sdt);
    ros::spinOnce();
  }

  return 0;
}
