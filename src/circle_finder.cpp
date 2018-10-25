#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/String.h"
#include <iostream>
#include <ROS_ass1/cords_rad.h>
using namespace cv;
using namespace std;
	const int cannyThresholdInitialValue = 75;
	const int accumulatorThresholdInitialValue = 75;
	const int maxAccumulatorThreshold = 200;
	const int maxCannyThreshold = 255;
	string color;
	map<string,string> colorDic;

class SubscribeAndPublish{
public:
	SubscribeAndPublish(){
		cout<<"in ctor"<<endl;
		image_transport::ImageTransport it(nh);
		subImage = it.subscribe("image_publisher", 1, &SubscribeAndPublish::imageCallback, this);
		subColor = nh.subscribe("color", 1000, &SubscribeAndPublish::colorNodeCallback, this);
		// ros::Publisher cords_rad_pub = nh.advertise<std_msgs::String>("cords_rad",1000);
		cout<<"ctor done"<<endl;
	}
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
    	cout<< "in HoughDetection" <<endl;
        // will hold the results of the detection
        std::vector<Vec3f> circles;
        // runs the actual detection
        HoughCircles( src_gray, circles, HOUGH_GRADIENT, 1, src_gray.rows/8, cannyThreshold, accumulatorThreshold, 0, 0 );

        // clone the colour, input image for displaying purposes
        cv::Mat display = src_display.clone();
        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            float radius = cvRound(circles[i][2]);
            int B = display.at<cv::Vec3b>(center)[0];
            int G = display.at<cv::Vec3b>(center)[1];
            int R = display.at<cv::Vec3b>(center)[2];

            string strRGB ="";
            strRGB.append(intToHEX(R));
            strRGB.append(intToHEX(G));
            strRGB.append(intToHEX(B));
            // cout << strRGB << endl;
 			cout<<"outside if"<<endl;
            if (!colorDic[strRGB].compare(color)){
              cout<<"strRGB "<< strRGB << endl;
              cout<<"colorDic[strRGB] "<< colorDic[strRGB] << endl;
              cout<<"color "<< color << endl;

            	cout<<"in if, the color is "<< color << colorDic[strRGB].compare(color) <<endl;
        		 ROS_ass1::cords_rad msg;
        		 msg.x = center.x;
        	 msg.y = center.y;
        		 msg.r = radius;
          

            ros::Publisher cords_rad_pub = nh.advertise<ROS_ass1::cords_rad>("cords_rad",1000);  
            ros::Rate loop_rate(10);
                 while (ros::ok())
              {
                  cords_rad_pub.publish(msg);
                   ros::spinOnce();
                   loop_rate.sleep();
              }
            	
        		   
            }
        }
    }   
	void imageCallback(const sensor_msgs::ImageConstPtr& msg)

	{
			cout<<"in imageCallback"<<endl;
	  try
	  {
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
	  	color =  msg->data.c_str();

	}

	private:
		ros::NodeHandle nh;
		image_transport::Subscriber subImage;
		ros::Subscriber subColor;
		ros::Publisher cords_rad_pub;

	};



int main(int argc, char **argv)
{
	ros::init(argc, argv, "circle_finder");
	 


	colorDic["056eee"] = "blue";
	colorDic["f23936"]= "red";
	colorDic["2d6444"]= "green";
	colorDic["8a3973"]= "purple";
	colorDic["f1ef20"]= "yellow";

	SubscribeAndPublish SAPObject;
// 	 cout<<"spinning"<<endl;
// 	 ros::Publisher color_pub = nh.advertise<std_msgs::String>("color2",1000);
// 	 std_msgs::String msg;
// 	 msg.data = "hi2";
//    ros::Rate loop_rate(10);
//      while (ros::ok())
//   {
// 	 color_pub.publish(msg);
       ros::spin();
//     loop_rate.sleep();
// }
}