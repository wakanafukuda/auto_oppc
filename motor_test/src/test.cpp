#include <stdio.h>
#include <stdlib.h>

#include "ros/ros.h"
#include "std_msgs/Int16MultiArray.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/MultiArrayLayout.h"

#include "std_msgs/Int16.h"

int main(int argc, char** argv){
  ros::init(argc, argv, "motor_test");
  ros::NodeHandle n;
  
  ros::Publisher pub = n.advertise<std_msgs::Int16MultiArray>("send_arduino",1000);

  std_msgs::Int16MultiArray dt;

  while(ros::ok()){
    dt.data.clear();
    for(int i = 0;i < 2;i++){
      if(i == 0){
        dt.data.push_back(50);
      }else if(i == 1){
        dt.data.push_back(-50);
      }
    }
    pub.publish(dt);
    sleep(5);
    dt.data.clear();
    for(int i = 0;i < 2;i++){
      dt.data.push_back(0);
    }
    pub.publish(dt);
    ROS_INFO("Published.");
    ros::spinOnce();
    sleep(10);
  }
}
