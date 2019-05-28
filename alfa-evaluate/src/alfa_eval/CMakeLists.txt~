cmake_minimum_required(VERSION 2.8.3)
project(publisher)

find_package(catkin REQUIRED
  roscpp
  std_msgs
)

catkin_package()

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(publisherNode src/publisher.cpp)
target_link_libraries(publisherNode ${catkin_LIBRARIES})
