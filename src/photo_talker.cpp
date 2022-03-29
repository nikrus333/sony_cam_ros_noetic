#include "ros/ros.h"
#include <sstream>

#include "nvcam.h"
#include <stdlib.h>
#include <stdio.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "photo_talker");  
  ros::NodeHandle n;
  ros::Rate loop_rate(5);

  unsigned int  retval;
  char fname[255];
  uint8_t  *data;
  uint32_t size;
  bool realtime;

  image_transport::ImageTransport it(n);
  image_transport::Publisher pub_frame = it.advertise("photo", 1);
     
  sensor_msgs::ImagePtr msgv;
  n.getParam("realtime", realtime);
  if (realtime == true) {
    NVCamera* cam = new NVCamera;

    if (cam->CameraConnected()) {
      while (ros::ok())
      {
        size = cam->PreviewToMemory((uint8_t**)&data);
        cv::Mat raw_data(1, size, CV_8UC1, (void*) data);
        cv::Mat img = imdecode(raw_data, cv::IMREAD_ANYCOLOR);
        //src = cv::cvCloneImage(data);
        msgv = cv_bridge::CvImage(std_msgs::Header(), "bgr8", img).toImageMsg();
        pub_frame.publish(msgv);
        
        ros::spinOnce();
        loop_rate.sleep();
      }
    }
    ROS_ERROR("camera not found");
    delete cam;
  }
  ROS_INFO("delete node sony_photo_talker");
  ros::shutdown();
  return 0;
}
