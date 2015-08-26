cmake_minimum_required(VERSION 2.8.3)
project(camera_simulator)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  sensor_msgs
  std_msgs
  image_transport
)

catkin_package()

include_directories(include
  ${catkin_INCLUDE_DIRS}
)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories(tcpimage)
add_subdirectory(tcpimage)
add_subdirectory(lcmnode)

add_executable(unity_image src/unity_image.cpp)
target_link_libraries(unity_image ${catkin_LIBRARIES} tcpimage)
add_dependencies(unity_image beginner_tutorials_generate_messages_cpp)
