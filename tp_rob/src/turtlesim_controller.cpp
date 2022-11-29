#include "ros/ros.h"
#include <math.h>
#include "geometry_msgs/Pose2D.h"										// to get desired position command
#include "turtlesim/Pose.h"												// to read current position
#include "geometry_msgs/Twist.h"											// to send velocity command

void ComPoseCallback(const geometry_msgs::Pose2D::ConstPtr& msg);
void CurPoseCallback(const turtlesim::Pose::ConstPtr& msg);
float GetErrorLin(turtlesim::Pose curpose, geometry_msgs::Pose2D despose);
float GetErrorAng(turtlesim::Pose curpose, geometry_msgs::Pose2D despose);


geometry_msgs::Pose2D DesPose;											// variable to hold desired pose
bool STOP = true;														// to hold stop flag, wait till first command given
geometry_msgs::Twist CmdVel;
turtlesim::Pose CurPose;												// to hold current pose

int main(int argc, char **argv)
{
	ros::init(argc, argv, "LookatAroundModes_sub");						// connect to roscore
	
	ros::NodeHandle n;													// node object
	// register sub to get desired position/pose commands
	ros::Subscriber ComPose_sub = n.subscribe("/turtle1/PositionCommand", 5, ComPoseCallback);
	// register sub to get current position/pose
	ros::Subscriber CurPose_sub = n.subscribe("/turtle1/pose", 5, CurPoseCallback);
	// register pub to send twist velocity (cmd_vel)
	ros::Publisher Twist_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 100);
	
	ros::Rate loop_rate(10);											// freq to run loops in (10 Hz)
	float ErrorLin = 0;
	float ErrorAng = 0;
	
	
	ROS_INFO("Ready to send position commands");						// let user know we are ready and good
	while (ros::ok() && n.ok() )										// while ros and the node are ok
	{
		ros::spinOnce();
		if (STOP == false)												// and no stop command
		{
			ErrorLin = GetErrorLin(CurPose, DesPose);
			ErrorAng = GetErrorAng(CurPose, DesPose);
			printf("Error linear: %f, Error angular: %f\n", ErrorLin, ErrorAng);
			
			if (ErrorLin > 0)
			{
				CmdVel.linear.x = 0.2 * ErrorLin;
			}
			else
			{
				CmdVel.linear.x = 0;
			}
			CmdVel.angular.z = 0.5 * ErrorAng;
			Twist_pub.publish(CmdVel);
		}
		else
		{
			printf("Waiting...\n");
		}
		
		loop_rate.sleep();												// sleep to maintain loop rate
	}
}

// call back to send new desired Pose msgs
void ComPoseCallback(const geometry_msgs::Pose2D::ConstPtr& msg)			
{
	STOP = false;														// start loop
	DesPose.x = msg->x;													// copy msg to varible to use elsewhere
	DesPose.y = msg->y;
	//~ DesPose.theta = msg->theta; // ignore theta, not using
	return;
}

// call back to send new current Pose msgs
void CurPoseCallback(const turtlesim::Pose::ConstPtr& msg)			
{
	CurPose.x = msg->x;
	CurPose.y = msg->y;
	CurPose.theta = msg->theta;											// copy msg to varible to use elsewhere
	return;
}

// function to get linear error from the turtles perspective. Error only along turtle X axis
float GetErrorLin(turtlesim::Pose curpose, geometry_msgs::Pose2D despose)
{
	// create error vector
	float Ex = despose.x - curpose.x;									// Error X. X component
	float Ey = despose.y - curpose.y;									// Error Y. Y component 
	float Et = GetErrorAng(curpose, despose);							// get angle between vectors
	
	// project error onto turtle x axis
	//~ float Etx =  pow( pow(Ex,2.0) + pow(Ey,2.0), 0.5 )*cos(Et);
	float Etx = hypotf(Ex, Ey)*cos(Et); // improved c function
	
	return Etx;
}

// function to get angular error between facing direction of the turtle and direction to desired pose
float GetErrorAng(turtlesim::Pose curpose, geometry_msgs::Pose2D despose)
{
	// create error vector
	float Ex = despose.x - curpose.x;									// Error X. X component 
	float Ey = despose.y - curpose.y;									// Error Y. Y component 
	
	// get desire angle
	float dest = atan2f(Ey, Ex); 										// use float version to get arc tangent
	
	// get angle error
	float Et = dest - curpose.theta + despose.theta;
	
	//~ ROS_INFO("Ex: %f, Ey: %f, Et: %f", Ex, Ey, Et);
	return Et;
}