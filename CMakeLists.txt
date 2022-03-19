cmake_minimum_required(VERSION 3.0.2)
project(sony_photo_pub)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg cv_bridge
image_transport)

## Declare ROS messages and services

## Declare a catkin package
catkin_package()
find_package(OpenCV 4.2.0 REQUIRED)
## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(photo_talker src/photo_talker.cpp src/nvcam.cpp)
target_link_libraries(photo_talker  gphoto2 ${catkin_LIBRARIES})
#add_dependencies(talker beginner_tutorials_generate_messages_cpp)