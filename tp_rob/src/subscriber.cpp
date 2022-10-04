#include "ros/ros.h"
#include "std_msgs/Float32.h"

void positionCallback(const std_msgs::Float32::ConstPtr& msg)
{
  ROS_INFO("J ai reçu la position : [%f]", msg->data);
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("chatter", 1000, positionCallback);

  ros::spin();

  return 0;
}