#include "ros/ros.h"
#include <sensor_msgs/Image.h>
#include"ball_chaser/DriveToTarget.h"

ros::ServiceClient client;
//call command robot service
void drive_robot(float lin_x,float ang_z)
{
   ball_chaser::DriveToTarget rossrv;
   rossrv.request.angular_z=ang_z;
   rossrv.request.linear_x=lin_x;
   if(!client.call(rossrv))
   {
       ROS_ERROR("Fail to call service");
   }else
   {
       ROS_INFO("Sucessfully call service");
   }

}
//exectutes ad rea image data
void process_image_callback(const sensor_msgs::Image img)
{
    int white_pixel=255;
    int check_pos=0;
    bool ball_found=false;
    //checking each pixel in the image if there is a bright white one
    for(int i=0;i<img.height*img.step;i++)
    {
        //check 3 chanels of image
        if(img.data[i]==255 &&img.data[i+1]==255 &&img.data[i+2]==255)
        {
            //check position in one of three areas of image(left.forward,right)
            check_pos= (i%img.step);
            ball_found=true;
            break;
        }
    }
    //controll robot moving
    if(!ball_found)
    {
        drive_robot(0,0);
        ROS_INFO("STOP");
    }
    else
    {    
        if(check_pos<(0.33*img.step))
        {
            drive_robot(0.5, 1);
            ROS_INFO("TURN LEFT");
        }
        if(check_pos>(0.33*img.step) &&check_pos<(0.67*img.step))
        {
            drive_robot(0.3,0);
            ROS_INFO("GO STRAIGTH");
        }
        if(check_pos>(0.67*img.step))
        {
            drive_robot(0.5,-1);
            ROS_INFO("RIGHT TURN");
        }
    }
}
int main(int argc,char** argv)
{
    ros::init(argc,argv,"process_image");
    ros::NodeHandle n;
    client=n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
    ros::Subscriber sub1=n.subscribe("/camera/rgb/image_raw",10,process_image_callback);
    ros::spin();
    return 0;
}

