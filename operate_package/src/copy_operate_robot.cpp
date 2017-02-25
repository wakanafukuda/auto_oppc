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

#define STRIGHT 1
#define RIGHT 2
#define LEFT 3

ros::Publisher flag_pub;
ros::Publisher ino_pub;
ros::Publisher sen_pub;
ros::Subscriber sub_state;
ros::Subscriber sub_sen;

std_msgs::Int16 r_motor;
std_msgs::Int16 flag_dt;
msg2015::Sensors2015 sdt;
int robot_mode;

void moveMotor(msg2015::CmdPWM msg){
  int i;
  ros::Rate r1(1);
  ros::Rate r2(30);


  if(msg.Turn > 0){//turn right
    flag_dt.data = RIGHT;
  }else if(msg.Turn < 0){//turn left
    flag_dt.data = LEFT;
  }
  flag_pub.publish(flag_dt);
  sleep(1);//スリープの時間は後で計算する

  //go straight
  flag_dt.data = STRIGHT;
  ino_pub.publish(flag_dt);
//  r2.sleep();
  sleep(1);
  //stop
  flag_dt.data = STOP;
  flag_pub.publish(flag_dt);

}

void movementCallBack(const msg2015::CmdPWM& msg){
  static int flag = 0;
  int i;

  //--メインプログラム--
  if(flag == 0){
    flag = 1;
  }else if(flag == 1){
    if(robot_mode == 0 || robot_mode == 2){ //stop or pause
      r_motor.data = STOP;
      ino_pub.publish(r_motor);
    }else if(robot_mode == 1){ //start
      moveMotor(msg);
    }
  flag = 0;
  }
}

void batteryCallBack(const std_msgs::Int16 msg){
  sdt.BV = msg.data;
  sdt.CO2 = 0;
  sen_pub.publish(sdt);
}

int main(int argc, char** argv){
  ros::init(argc,argv,"operate_robot");
  ros::NodeHandle n;

  flag_pub = n.advertise<std_msgs::Int16>("send_flag",1000);

  sub_state = n.subscribe("movement",1000,&movementCallBack);
  ino_pub = n.advertise<std_msgs::Int16>("send_arduino",1000);

  sub_sen = n.subscribe<std_msgs::Int16>("chatterbattery",1000,&batteryCallBack);
  sen_pub = n.advertise<msg2015::Sensors2015>("sensors",1000);

  while(ros::ok()){
    n.getParam("/robot_state_param_value",robot_mode);
    sen_pub.publish(sdt);
    ros::spinOnce();
  }

  return 0;
}
