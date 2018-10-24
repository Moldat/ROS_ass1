#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/String.h"
#include <iostream>

std::string color;
using namespace cv;
using namespace std;
    const int cannyThresholdInitialValue = 75;
    const int accumulatorThresholdInitialValue = 75;
    const int maxAccumulatorThreshold = 200;
    const int maxCannyThreshold = 255;
std::map<string,string> colorDic;



    string intToHEX(int n)
    {

            string zero = "0";
            std::stringstream stream;
            stream << std::hex <<  n;
            std::string result( stream.str() );
            if (result.length() == 1){
              return zero.append(result);
            }
            return result;
    }

    void HoughDetection(const Mat& src_gray, const Mat& src_display, int cannyThreshold, int accumulatorThreshold)
    {
        // will hold the results of the detection
        std::vector<Vec3f> circles;
        // runs the actual detection
        HoughCircles( src_gray, circles, HOUGH_GRADIENT, 1, src_gray.rows/8, cannyThreshold, accumulatorThreshold, 0, 0 );

        // clone the colour, input image for displaying purposes
        cv::Mat display = src_display.clone();
        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);

            int B = display.at<cv::Vec3b>(center)[0];
            int G = display.at<cv::Vec3b>(center)[1];
            int R = display.at<cv::Vec3b>(center)[2];

            string strRGB ="";
            strRGB.append(intToHEX(R));
            strRGB.append(intToHEX(G));
            strRGB.append(intToHEX(B));
            cout << strRGB << endl;
 
            if (colorDic[strRGB] == color){
            circle( display, center, 3, Scalar(0,255,0), -1, 8, 0 );
            
            circle( display, center, radius, Scalar(0,0,255), 3, 8, 0 );
            }

 
        }

        // shows the results
        imshow( "result", display);
    }   
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
    cv::Mat src = cv_bridge::toCvShare(msg, "bgr8")->image;
    cv::Mat src_gray;
         if( src.empty() )
    {
        std::cerr<<"Invalid input image\n";        
    }


    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

        int cannyThreshold = cannyThresholdInitialValue;
    int accumulatorThreshold = accumulatorThresholdInitialValue;

    HoughDetection(src_gray, src, 75, accumulatorThreshold);


    cv::waitKey(30);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

void colorNodeCallback(const std_msgs::String::ConstPtr& msg)
{
 // ROS_INFO("I heard: [%s]", msg->data.c_str());
  color =  msg->data.c_str();
}


int main(int argc, char **argv)
{
  colorDic["056eee"] = "blue";
colorDic["f23936"]= "red";
colorDic["2d6444"]= "green";
colorDic["8a3973"]= "purple";
colorDic["f1ef20"]= "yellow";
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  cv::namedWindow("view");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber subImage = it.subscribe("image_publisher", 1, imageCallback);
  ros::Subscriber subColor = nh.subscribe("color", 1000, colorNodeCallback);
  ros::spin();
  cv::destroyWindow("view");
}