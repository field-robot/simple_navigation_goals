


#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseActionFeedback.h>


double pie = 3.14159265;
double x;
double y;
double w;
double curr_x;
double curr_y;
double curr_w;
double goal_x;
double goal_y;
double goal_w;
double set_bot_w;
double set_bot_x;
double set_top_w;
double set_top_x;
int count;


void odomMsgs(const move_base_msgs::MoveBaseActionFeedback& feedback)						//callback function for the position of the robot
{
	x = feedback.feedback.base_position.pose.position.x;
	ROS_INFO("%d",x);
	y = feedback.feedback.base_position.pose.position.y;
	ROS_INFO("%d",y);
	w = feedback.feedback.base_position.pose.orientation.z;	
	ROS_INFO("%d",w);
}

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("move_base/feedback", 1, &odomMsgs);					//subscribed to odometry msg
  
  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);
  
  //ros::Rate loop_rate(10);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  

  while (ros::ok()) {


while(true){

	if (count == 0){
		goal_x = curr_x;
		goal_y = 13;
		goal_w = curr_w;
		
		if (curr_y > 1 && curr_y < 2){
			set_bot_x = curr_x;
			set_bot_w = curr_w;
			count = 1;
		}

	}

	if ((curr_y > 11 && curr_y < 12) && (count == 1)){
		set_top_x = curr_x;
		set_top_w = curr_w;
		goal_x = set_top_x + 0.75;
		goal_y = 11;
		goal_w = set_top_w + pie;
		count = 2;
	}

	if ((curr_y < 11 && curr_y > 10) && (count == 2)){
		set_top_x = curr_x;
		set_top_w = curr_w;
		goal_x = set_bot_x + 0.75;
		goal_y = 1;
		goal_w = set_bot_w + pie;
		count = 3;
	}

	if ((curr_y < 1 && curr_y > 0) && (count == 3)){
		set_bot_x = curr_x;
		set_bot_w = curr_w;
		goal_x = set_bot_x + 0.75;
		goal_y = 1;
		goal_w = set_bot_w + pie;
		count = 4;
	}
		
	if ((curr_y < 1 && curr_y > 0) && (count == 4)){
		set_bot_x = curr_x;
		set_bot_w = curr_w;
		goal_x = set_top_x + 0.75;
		goal_y = 11;
		goal_w = set_top_w + pie;
		count = 1;
	}
 ROS_INFO("Fuck alles");
      move_base_msgs::MoveBaseGoal goal;

      //we'll send a goal to the robot to move 1 meter forward
      goal.target_pose.header.frame_id = "base_link";
      goal.target_pose.header.stamp = ros::Time::now();

      goal.target_pose.pose.position.x = goal_x;
      goal.target_pose.pose.position.y = goal_y;
      goal.target_pose.pose.orientation.w = goal_w;

      ROS_INFO("Sending goal");
      ac.sendGoal(goal);

      ac.waitForResult();

      if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        ROS_INFO("Succesfully reached the goal");
      else
        ROS_INFO("Did not reach the goal. Your robot is fucked up");

	  //loop_rate.sleep();
      
  }
}

}
