#include <ros/ros.h>
#include "std_msgs/String.h"
#include <ros/package.h>
int main(int argc, char** argv)
{
      ros::init(argc, argv, "color_requester");
      ros::NodeHandle nh;
      std::string color;
      std::cout<<"please enter a color:"<<std::endl;
      std::getline(std::cin, color);
      ros::Publisher color_pub = nh.advertise<std_msgs::String>("color",1000);
      std_msgs::String msg;
      ros::Rate loop_rate(10);
      while (nh.ok()) {
        msg.data = color;
        color_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
      }
}