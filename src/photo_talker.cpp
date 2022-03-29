#include "ros/ros.h"
#include <sstream>

#include "nvcam.h"
#include <stdio.h>
#include <stdlib.h>

#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sensor_msgs/image_encodings.h>

int main(int argc, char **argv) {

  ros::init(argc, argv, "photo_talker");
  ros::NodeHandle n;

  unsigned int retval;
  char fname[255];
  uint8_t *data;
  uint32_t size;
  bool realtime;

  image_transport::ImageTransport it(n);
  image_transport::Publisher pub_frame = it.advertise("photo", 1);
  ros::Rate loop_rate(5);
  sensor_msgs::ImagePtr msgv;
  n.getParam("realtime", realtime);
  if (!realtime) {
    return 0;
  }
  NVCamera cam;
  if (cam.CameraConnected()) {
    while (ros::ok()) {
      size = cam.PreviewToMemory((uint8_t **)&data);
      cv::Mat raw_data(1, size, CV_8UC1, (void *)data);
      //сигнатура метода imdecode это cv::InputArray, ты посылаешь туда Mat.
      //Поэтому происходит неявное преобразование и на 10 герцах у меня вылетал
      //сегфолт
      cv::Mat img = imdecode(raw_data, cv::IMREAD_ANYCOLOR);
      msgv = cv_bridge::CvImage(std_msgs::Header(), "bgr8", img).toImageMsg();
      pub_frame.publish(msgv);

      ros::spinOnce();
      loop_rate.sleep();
    }
  } else
    ROS_ERROR("camera not found");
  ROS_INFO("delete node sony_photo_talker");
  ros::shutdown();
  return 0;
}
