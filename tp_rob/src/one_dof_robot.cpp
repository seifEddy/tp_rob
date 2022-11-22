#include "tp_rob/one_dof_robot.h"

OneDoFRobot::OneDoFRobot(ros::NodeHandle *node_handle):nh(*node_handle)
{
    // nh = *node_handle;
    actuator.id = 0;
    actuator.position = 0.0;
    actuator.velocity = 0.0;

    qdd = 0.0;
    qd = 0.0; 
    q = 0.0; 
    prev_qdd = 0.0;
    prev_qd = 0.0; 
    prev_q = 0.0; 
    dt = 0.001;

    m = 1;                          // kg
    l = 1;                          // m
    g = 9.8;                        // m/s²
    I = m * pow(l, 2);              // kg.m²

    state_pub = nh.advertise<std_msgs::Float32>("q", 1);
    cmd_sub = nh.subscribe<std_msgs::Float32>("cmd", 1, &OneDoFRobot::cmd_callback, this);

    // Controller
    kp = 10.0;
    kd = 9000.0;

}

void OneDoFRobot::cmd_callback(const std_msgs::Float32::ConstPtr& msg)
{
    cmd = msg->data;
    model(cmd);
    // q = cmd;

    // ROS_INFO("Position should be %f", q);
}

void OneDoFRobot::model(float u)
{
    qdd  = (u/I) - (g/l) * sin(q);  // rad/s²
    // return qdd ; 
    integral();
}

void OneDoFRobot::integral()
{
    // first integral 
    qd = prev_qd + dt * prev_qdd;
    // second integral
    q = prev_q + dt * prev_qd;
    // stock
    prev_q = q;
    prev_qd = qd;
    prev_qdd = qdd;
}

void OneDoFRobot::derivative(){
    error_d = error - prev_error ;
    prev_error = error ;
}

void OneDoFRobot::pd_control(){
    error = q_ref - q;
    derivative();
    cmd = kp * error + kd * error_d;
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "one_dof_robot_node");
    ros::NodeHandle nh;
    ros::Rate rate(100);
    OneDoFRobot robot(&nh);
    std_msgs::Float32 state;

    while(ros::ok())
    {
        robot.q_ref = 2.0;
        robot.pd_control();
        robot.model(robot.cmd);

        state.data = robot.q;
        robot.state_pub.publish(state);
        rate.sleep();
        ros::spinOnce();
    }
    return 0;
}