/*  ***************************************************************************
*   alfa-evaluate_node.cpp -  basic ros node written in C++'11 language to evaluate
*   our dataset and publish relevant information
*
*   For more information about the dataset, please refer to:
*   http://theairlab.org/alfa-dataset
*
*   For more information about this project and the publications related to
*   the dataset and this work, please refer to:
*   http://theairlab.org/fault-detection-project
*
*   Air Lab, Robotics Institute, Carnegie Mellon University
*
*   Authors: Azarakhsh Keipour, Mohammadreza Mousaei, Sebastian Scherer
*   Contact: keipour@cmu.edu
*
*   Last Modified: May 19, 2019
*   ***************************************************************************/
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "eval_msg/evaluate.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Int8.h"
#include <sstream>

double timeout = 5;
ros::Time gt_time;
ros::Time detection_time;
ros::Time detection_delay;
bool gt_trig = false;
bool detection_trig = false;
bool fault_detected = false;

void engineFailHandler(const std_msgs::Bool::ConstPtr& data)
{
  if(data)
  {
    gt_time = ros::Time::now();
    gt_trig = true;
  }
}
void aileronFailHandler(const std_msgs::Int8::ConstPtr& data)
{
  if(data)
  {
    gt_time = ros::Time::now();
    gt_trig = true;
  }
}
void rudderFailHandler(const std_msgs::Int8::ConstPtr& data)
{
  if(data)
  {
    gt_time = ros::Time::now();
    gt_trig = true;
  }
}
void elevatorFailHandler(const std_msgs::Int8::ConstPtr& data)
{
  if(data)
  {
    gt_time = ros::Time::now();
    gt_trig = true;
  }
}
void detectionHandler(const std_msgs::Int8::ConstPtr& data)
{
  if(data)
  {
    detection_time = ros::Time::now();
    detection_trig = true;
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  ros::NodeHandle nhPrivate = ros::NodeHandle("~");

  ros::Publisher evalPub = n.advertise<eval_msg::evaluate>("/evaluation", 1);
  
  ros::Subscriber enginesSub = n.subscribe("/failure_status/engines", 1, engineFailHandler);
  ros::Subscriber aileronSub = n.subscribe("/failure_status/aileron", 1, aileronFailHandler);
  ros::Subscriber rudderSub = n.subscribe("/failure_status/rudder", 1, rudderFailHandler);
  ros::Subscriber elevatorSub = n.subscribe("/failure_status/elevator", 1, elevatorFailHandler);
  ros::Subscriber detectionSub = n.subscribe("/detection", 1, detectionHandler);


  ros::Rate loop_rate(10);

  nhPrivate.getParam("timeout", timeout);

  eval_msg::evaluate eval;

  int count = 0;
  while (ros::ok())
  {
    
    if(gt_trig)
    {
      if(detection_trig)
      {
        fault_detected = true;
        detection_delay.sec = detection_time.sec - gt_time.sec;
        detection_delay.nsec = detection_time.nsec - gt_time.nsec;
        if(detection_delay.nsec < 0)
        {
          detection_delay.sec -= 1;
          detection_delay.nsec = 1000000000 + detection_delay.nsec;
        }
        eval.fault_detected = fault_detected;
        eval.detection_delay = detection_delay;
        eval.false_positive = false;
        evalPub.publish(eval);
        ROS_INFO("fault_detected = %d, detection_delay.sec = %d, detection_delay.nsec = %d, false_positive = %d", eval.fault_detected, eval.detection_delay.sec, eval.detection_delay.nsec, eval.false_positive);
        gt_trig = false;
        detection_trig = false;
      }
      else
      {
        eval.fault_detected = false;
        eval.false_positive = false;
        detection_delay.sec = 0;
        detection_delay.nsec = 0;
        eval.detection_delay = detection_delay;
        if( (ros::Time::now().sec - gt_time.sec) > timeout )
        {
          fault_detected = false;
          detection_trig = false;
          gt_trig = false;
          evalPub.publish(eval);
          ROS_INFO("fault_detected = %d, detection_delay.sec = %d, detection_delay.nsec = %d, false_positive = %d", eval.fault_detected, eval.detection_delay.sec, eval.detection_delay.nsec, eval.false_positive);
        }
      }
    }
    else
    {
      if(detection_trig)
      {
        eval.false_positive = true;
        eval.fault_detected = false;
        detection_delay.sec = 0;
        detection_delay.nsec = 0;
        eval.detection_delay = detection_delay;
        evalPub.publish(eval);
        ROS_INFO("fault_detected = %d, detection_delay.sec = %d, detection_delay.nsec = %d, false_positive = %d", eval.fault_detected, eval.detection_delay.sec, eval.detection_delay.nsec, eval.false_positive);
        detection_trig = false;
      }
    }
    
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}