#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "tp_rob/Motor.h"
#include <math.h>

class OneDoFRobot
{
    public:
        ros::NodeHandle nh;
        tp_rob::Motor actuator;
        float qdd, qd, q, prev_qdd, prev_qd, 
              prev_q, dt;

        float error, error_d, prev_error, cmd;
        float q_ref, kp, kd;
        void derivative();
        void pd_control();

        float m, l, g, I;
        ros::Subscriber cmd_sub;
        ros::Publisher  state_pub;
    public:
        OneDoFRobot(ros::NodeHandle *node_handle);
        // OneDoFRobot(tp_rob::Motor motor);
        void cmd_callback(const std_msgs::Float32::ConstPtr& msg);
        void model(float cmd);
        void integral();

    private:
    
};