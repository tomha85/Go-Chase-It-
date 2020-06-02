#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <std_msgs/Float64.h>
#include "ball_chaser/DriveToTarget.h"
#include <iostream>
ros::Publisher motor_command_publisher;

bool handle_drive_request(ball_chaser::DriveToTarget::Request& req,ball_chaser::DriveToTarget::Response& res)
{  
    ROS_INFO("Goto DriveToTarget received -linear_x:%1.2f,angular_z:%1.2f",(float)req.linear_x,(float)req.angular_z);
    geometry_msgs::Twist motor_command;
    motor_command.linear.x = req.linear_x;
    motor_command.angular.z = req.angular_z;

    motor_command_publisher.publish(motor_command);
    res.msg_feedback = "Moving - linear_x: " + std::to_string(req.linear_x) + ", angular_z: " + std::to_string(req.angular_z);
    ROS_INFO_STREAM(res.msg_feedback);
    return true;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "drive_bot");
    ros::NodeHandle n;   
    
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);
    
    ros::spin();

    return 0;
}