#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <string>
#include <ros/package.h>



int main(int argc, char** argv)
   {
     ros::init(argc, argv, "image_publisher");
     ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
      std::string imagepath = ros::package::getPath("ROS_ass1") + "/src/colors.jpg";
    image_transport::Publisher pub = it.advertise("image_publisher", 1);
    cv::Mat image = cv::imread(imagepath, CV_LOAD_IMAGE_COLOR);
    cv::waitKey(30);
    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();
  
    ros::Rate loop_rate(5);
    while (nh.ok()) {
      pub.publish(msg);
      ros::spinOnce();
      loop_rate.sleep();
    }
}

