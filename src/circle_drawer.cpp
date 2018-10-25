#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/String.h"
#include <iostream>
#include <ROS_ass1/cords_rad.h>
#include <string>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
 

using namespace cv;
using namespace std;
ROS_ass1::cords_rad circle_cords;
bool got_cords = false;
bool first = true;
void imageCallback(const sensor_msgs::ImageConstPtr& msg){
	cout << "got image" << endl;
	if (got_cords && first){
		cv::Mat display = cv_bridge::toCvShare(msg, "bgr8")->image;
		if( display.empty() )
    {
        std::cerr<<"Invalid input image\n";
    }
		 
			first = false;
		 Point center(cvRound(circle_cords.x), cvRound(circle_cords.y));
			 // circle center
            circle( display, center, 3, Scalar(0,255,0), -1, 8, 0 );
            // circle outline
            circle( display, center, circle_cords.r, Scalar(0,0,255), 3, 8, 0 );
            cout<<"saving image"<<endl;
             imwrite( "/home/alex/catkin_ws/src/ROS_ass1/src/selected_circle.jpg", display);
             cout<<"saved image"<<endl;

	}

}

void cordsRadCallback(const ROS_ass1::cords_rad& cords)
{
	 
  	circle_cords = cords;
  	cout<<"got records"<<endl;
  	got_cords = true;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "circle_drawer");
	ros::NodeHandle nh;
	//cv::namedWindow("view");
	//cv::startWindowThread();
	image_transport::ImageTransport it(nh);
	image_transport::Subscriber subImage = it.subscribe("image_publisher", 1, imageCallback);

	ros::Subscriber cords_rad = nh.subscribe("cords_rad", 1000, cordsRadCallback);
	ros::spin();
	//cv::destroyWindow("view");
}