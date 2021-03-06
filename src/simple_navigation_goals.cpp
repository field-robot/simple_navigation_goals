#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>




double x;
double w;
double phi;
double pi = 3.1415;
double x_w = 0;
double y_w = 0;
int location = 0;

void odometry(const nav_msgs::Odometry& odom)
{
	phi = odom.pose.pose.orientation.z;
	x_w = odom.pose.pose.position.x;
	y_w = odom.pose.pose.position.y;
	ROS_INFO("Rotation: %f",phi);
	ROS_INFO("X position: %f",x_w);
	ROS_INFO("Y position: %f",y_w);
}


int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals");
	ros::NodeHandle nh;	
  ros::Publisher SendSpeed = nh.advertise<geometry_msgs::Twist>("r2d2_diff_drive_controller/cmd_vel",1);
  ros::Subscriber receiveOdemetry = nh.subscribe("r2d2_diff_drive_controller/odom" ,1, &odometry);

  
  //tell the action client that we want to spin a thread by default
	ros::Rate loop_rate(10);

  //wait for the action server to come up
  while (ros::ok())
 {
  

  geometry_msgs::Twist goal;

  //we'll send a goal to the robot to move 1 meter forward
  
  w = 10;
 


  goal.linear.x = x;
  goal.angular.z = w;
  SendSpeed.publish(goal);
  ros::spinOnce();
  loop_rate.sleep();
  }
return 0;
}
