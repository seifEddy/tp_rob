#include "ros/ros.h"
#include "tp_rob/Motor.h"
#include "tp_rob/GetMotorData.h"
#include <math.h>
 
bool get_motor_data(tp_rob::GetMotorData::Request &req,
         tp_rob::GetMotorData::Response &res)
{
  if (req.id == 0)
  {
    return true;
  }
  return false;
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "publisher_motor");

  ros::NodeHandle n;

  ros::Publisher position_pub = n.advertise<tp_rob::Motor>("motor_data", 1000);
  ros::ServiceServer service = n.advertiseService("get_motor_data_service", get_motor_data);

  ros::Rate loop_rate(100);

  long double t = 0.0;

  tp_rob::Motor motor_msg;
  
  while (ros::ok())
  {

    t += 1.0/10;
    
    position_pub.publish(motor_msg);

    ros::spinOnce();

    loop_rate.sleep();

  }

  return 0;
}