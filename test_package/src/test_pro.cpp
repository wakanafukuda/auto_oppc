
#include <stdio.h>
#include <stdlib.h>

#include "ros/ros.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int16MultiArray.h"

#include "std_msgs/Int16.h"

ros::Publisher pub;
ros::Subscriber sub;
ros::Publisher pub2;

/*
void callbackfunc(std_msgs::Int16 msg){
//  std_msgs::Int16 dt;
//  dt.data = 10;
  pub2.publish(msg);
//  ROS_INFO("I subscribed: [ %d ]",msg.data);
}

int main(int argc, char** argv){
  ros::init(argc, argv, "test_pro");
  ros::NodeHandle n;

  pub = n.advertise<std_msgs::Int16MultiArray>("talker",1000);
  sub = n.subscribe("listener",1000,callbackfunc);
  pub2 = n.advertise<std_msgs::Int16>("talker2",1000);

  std_msgs::Int16MultiArray dt;
  while(ros::ok()){

    dt.data.clear();
    for(int i = 0;i < 10;i++){
      dt.data.push_back(255);
    }
    pub.publish(dt);

    ROS_INFO("I published something!");
    sleep(2);
    ros::spinOnce();
  }
}

*/
/*
#include <stdio.h>
#include <stdlib.h>

#include "ros/ros.h"
#include "msg2015/CmdPWM.h"
#include "msg2015/Sensors2015.h"

void callbackf(const msg2015::CmdPWMPtr& msg){
  ROS_INFO("I received:[ %d, %d]",msg->Turn, msg->Forward);
}

int main(int argc, char** argv){
  ros::init(argc, argv,"test_pro");
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("listener",1000,callbackf);
  ros::spin();
}
*/

int main(int argc, char** argv){
  ros::init(argc, argv,"test_pro");
  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<std_msgs::Int16>("arm_pan",1000);
  
  std_msgs::Int16 msg;
  while(ros::ok()){
    msg.data = 90;
    pub.publish(msg);
    sleep(2);
    msg.data = 0;
    pub.publish(msg);
    sleep(2);
    ROS_INFO("Published");
    ros::spinOnce();
  }
}
