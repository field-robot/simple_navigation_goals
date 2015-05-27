#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <nav_msgs/Odometry.h>

double x;
double y;
double w;
double goal_x;
double goal_y;
double goal_w;


void odomMsgs(const nav_msgs::Odometry& odometry)						//callback function for the position of the robot
{
	x = odometry.pose.pose.position.x;
	y = odometry.pose.pose.position.y;
	w = odometry.twist.twist.linear.z;	
}

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("odom", 1, &odomMsgs);					//subscribed to odometry msg
  
  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  while (true) {

      //CALCULATIONS

      if (x<1,02){
          goal_x = 1.52;
          goal_y = 0,76;
          goal_w = 0.5 * M_PI;
      }

      if ((x > 1,02) && (x < 1,12)){
          goal_x = 3,02;
          goal_y = 0,76;
          goal_w = 0.5 * M_PI;
      }

      //END CALCULATIONS





      move_base_msgs::MoveBaseGoal goal;

      //we'll send a goal to the robot to move 1 meter forward
      goal.target_pose.header.frame_id = "base_link";
      goal.target_pose.header.stamp = ros::Time::now();

      goal.target_pose.pose.position.x = goal_x;
      goal.target_pose.pose.position.y = goal_y;
      goal.target_pose.pose.orientation.z = goal_w;

      ROS_INFO("Sending goal");
      ac.sendGoal(goal);

      ac.waitForResult();

      if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        ROS_INFO("Hoera, luc broeren is een kneus");
      else
        ROS_INFO("The base failed to move forward 1 meter for some reason");


      //ros::spinOnce();
  }
}
