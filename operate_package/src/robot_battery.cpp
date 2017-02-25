#include "ros/ros.h"
#include "msg2015/Sensors2015.h"

void batteryCallBack(std_msgs::Int16& msg){
  msg2015::Sensors2015 sensor_msg;

  sensor_msg.BV.data = msg.data;
  sensor_msg.CO2.data = 0;
  pub.publish(sensor_msg);
}


int main(int argc, char** argv){
  ros::init(argc, argv, "robot_battery");
  ros::NodeHandle n;

  ros::Pulisher pub = n.advertise<msg2015::Sensors2015>("sensors",1000);
  ros::Subscriber sub = n.subscribe("chatterbattery",1000,batteryCallBack);
  ros::spin();

  return 0;
}
