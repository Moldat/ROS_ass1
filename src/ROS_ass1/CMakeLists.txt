cmake_minimum_required(VERSION 2.8.3)
project(ROS_ass1)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS message_generation roscpp rospy genmsg std_msgs cv_bridge image_transport)
find_package( OpenCV REQUIRED )
include_directories(
  ${catkin_INCLUDE_DIRS}
)

include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})
## Declare ROS messages and services
# add_message_files(FILES Num.msg)
# add_service_files(FILES AddTwoInts.srv)

## Generate added messages and services


add_message_files(FILES cords_rad.msg)
generate_messages(DEPENDENCIES std_msgs)
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(image_converter src/image_converter.cpp)
target_link_libraries(image_converter ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})


add_executable(circle_finder src/circle_finder.cpp)
target_link_libraries(circle_finder ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(color_requester src/color_requester.cpp)
target_link_libraries(color_requester ${catkin_LIBRARIES})

add_executable(circle_drawer src/circle_drawer.cpp)
target_link_libraries(circle_drawer ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
