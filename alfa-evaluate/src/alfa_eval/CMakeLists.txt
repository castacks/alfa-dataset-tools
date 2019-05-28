cmake_minimum_required(VERSION 2.8.3)
project(alfa-evaluate)

find_package(catkin REQUIRED
  roscpp
  eval_msg
  std_msgs
)

catkin_package()

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(alfa-evaluate_node src/alfa-evaluate_node.cpp)
target_link_libraries(alfa-evaluate_node ${catkin_LIBRARIES})
