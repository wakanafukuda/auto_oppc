#include<ros/ros.h>
#include<stdlib.h>

int main(int argc,char *argv[]){
  ros::init(argc,argv,"gui_node");
  ros::NodeHandle n;

  //system("gnome-terminal --tab -e --command='firefox /home/rodepoppc/rosjstest/gui.html'");
  system("firefox /home/rodepoppc/rosjstest/new_gui.html");
  return 0;
}
