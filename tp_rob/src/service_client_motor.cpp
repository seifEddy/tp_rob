#include "ros/ros.h"
#include "tp_rob/Motor.h"
#include "tp_rob/GetMotorData.h"
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "get_motor_data_client");
  if (argc != 2)
  {
    ROS_INFO("usage: get_motor_data_client id");
    return 1;
  }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<tp_rob::GetMotorData>("get_motor_data_service");
  tp_rob::GetMotorData srv;
  srv.request.id = atoll(argv[1]);
  if (client.call(srv))
  {
    ROS_INFO("Motor position: %f\n Motor velocity: %f", 
            (double)srv.response.position, (double)srv.response.velocity);
  }
  else
  {
    ROS_ERROR("Failed to call service get_motor_data_service");
    return 1;
  }

  return 0;
}