#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

#include "nvcam.h"
#include <stdlib.h>
#include <stdio.h>
 #include <string.h>
#include "time.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>


int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "photo_talker");  
  ros::NodeHandle n;

  
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
  ros::Rate loop_rate(25);
  int count = 0;
  

  unsigned int  retval;
  char fname[255];
  FILE   *f;
  uint8_t  *data;
  uint32_t size;
  time_t start, stop;
  int i,tm;
  static const std::string OPENCV_WINDOW = "Image window";
  static const std::string IMAGE_TOPIC = "/camera/rgb/image_raw";
  static const std::string PUBLISH_TOPIC = "/image_converter/output_video";
  image_transport::ImageTransport it(n);
  image_transport::Publisher pub_frame = it.advertise("camera", 1);
     
  sensor_msgs::ImagePtr msgv;
  NVCamera* cam = new NVCamera;
  if (cam->CameraConnected()) {
  while (ros::ok())
  {
  
    size = cam->PreviewToMemory((uint8_t**)&data);
    sprintf(fname,"shot.jpg");
    f = fopen(fname, "wb");
    if (f) {
      fwrite (data, size, 1, f);            
      fclose(f);
           } 
    cv::Mat img = imread("shot.jpg", cv::IMREAD_COLOR);
    //src = cv::cvCloneImage(data);
    msgv = cv_bridge::CvImage(std_msgs::Header(), "bgr8", img).toImageMsg();
    pub_frame.publish(msgv);
    cv::Mat image1,image2;
   //image1=cv::imread(*data, 0);
    //cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
    //cv::imshow("camera", img);
    cv::waitKey(10);


    std_msgs::String msg;
    std::stringstream ss;
    ss << "hv world " << count;
    msg.data = ss.str();
    ROS_INFO("%s", msg.data.c_str());
    ROS_INFO("%d", size);
    ROS_INFO("%d", *data);
    chatter_pub.publish(msg);
    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }

  }
  delete cam;
  return 0;
}