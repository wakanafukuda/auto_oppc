#include <stdio.h>
#include <stdlib.h>

#include "ros/ros.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int16MultiArray.h"

#include "std_msgs/Int16.h"

int main(int argc, char** argv){
  ros::init(argc, argv, "test_pro");
  ros::NodeHandle n;
  ros::Rate r(1);

  ros::Publisher pub = n.advertise<std_msgs::Int16MultiArray>("talker",1000);

  std_msgs::Int16MultiArray dt;
  while(ros::ok()){
    dt.data.clear();
    for(int i = 0;i < 90;i++){
      dt.data.push_back(255);
    }
    pub.publish(dt);
    ROS_INFO("I published something!");
    ros::spinOnce();
    sleep(2);
  }
}

