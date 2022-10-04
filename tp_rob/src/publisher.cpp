#include "ros/ros.h"
#include "std_msgs/Float32.h"

int main(int argc, char **argv)
{

  ros::init(argc, argv, "publisher");

  ros::NodeHandle n;

  ros::Publisher position_pub = n.advertise<std_msgs::Float32>("position", 1000);

  ros::Rate loop_rate(10);

  while (ros::ok())
  {

    std_msgs::Float32 msg;

    ROS_INFO("%f", msg.data);
    position_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();

  }

  return 0;
}