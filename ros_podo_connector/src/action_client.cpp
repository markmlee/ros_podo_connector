#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <ros_podo_connector/RosPODOmotionAction.h>
#include <ros_podo_connector/RosPODO_BaseAction.h>
#include <ros_podo_connector/RosPODO_ArmAction.h>
/*custom defined header to TCP/IP to PODO */
#include "ROSLANData.h"

int main (int argc, char **argv)
{
  ros::init(argc, argv, "testClient");

  // create the action client
  // true causes the client to spin its own thread
  actionlib::SimpleActionClient<ros_podo_connector::RosPODO_BaseAction> ac_base("rospodo_base", true);
  actionlib::SimpleActionClient<ros_podo_connector::RosPODO_ArmAction> ac_arm("rospodo_arm", true);

  ROS_INFO("Waiting for action server to start.");
  // wait for the action server to start
  ac_base.waitForServer(); //will wait for infinite time
  ac_arm.waitForServer(); //will wait for infinite time

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action
  ros_podo_connector::RosPODO_BaseGoal goal_base;
  ros_podo_connector::RosPODO_ArmGoal goal_arm;
  
  
  /* ============== Base Data Action  ==============  */
  goal_base.command = 1;
  goal_base.MoveX = 1.0;
  goal_base.MoveY = 0.0;
  goal_base.ThetaDeg = 0;
  ac_base.sendGoal(goal_base);
  
  //wait to test repeated TX or need to clear
  ros::Duration(3).sleep();
		  
  /* ============== Arm Data Action WBIK ==============  */
  //goal_arm.command = 3;
  //goal_arm.wbik_ref[RIGHT_HAND].OnOff_position = CONTROL_ON;
  //goal_arm.wbik_ref[RIGHT_HAND].goal_position[0] = 0.5;
  //goal_arm.wbik_ref[RIGHT_HAND].goal_position[1] = -0.3;
  //goal_arm.wbik_ref[RIGHT_HAND].goal_position[2] = 0.2;
  //goal_arm.wbik_ref[RIGHT_HAND].GoalmsTime = 2000;
  
  
  //goal_arm.wbik_ref[RIGHT_ELBOW].OnOff_position = CONTROL_ON;
  //goal_arm.wbik_ref[RIGHT_ELBOW].goal_angle = -30.0;
  //goal_arm.wbik_ref[RIGHT_ELBOW].GoalmsTime = 2000;


    //goal_arm.wbik_ref[WAIST].OnOff_position = CONTROL_ON;
    //goal_arm.wbik_ref[WAIST].goal_angle = -30.0;
    //goal_arm.wbik_ref[WAIST].GoalmsTime = 2000;
    
    /* ============== Arm Data Action JOINT ==============  */
  
  goal_arm.command = MODE_MOVE_JOINT;
  goal_arm.joint_ref[rosREB].OnOffControl = CONTROL_ON;
  goal_arm.joint_ref[rosREB].reference = -30.0;
  goal_arm.joint_ref[rosREB].GoalmsTime = 3000;
  
  goal_arm.joint_ref[rosLEB].OnOffControl = CONTROL_ON;
  goal_arm.joint_ref[rosLEB].reference = -30.0;
  goal_arm.joint_ref[rosLEB].GoalmsTime = 3000;
  
  /* ============== Arm Data Action JOINT PUBLISH ==============  */



  ac_arm.sendGoal(goal_arm);

  //wait for the action to return
  bool finished_before_timeout = ac_base.waitForResult(ros::Duration(30.0));

  if (finished_before_timeout)
  {
    actionlib::SimpleClientGoalState state = ac_base.getState();
    ROS_INFO("Action finished: %s",state.toString().c_str());
  }
  else
    ROS_INFO("Action did not finish before the time out.");

  //exit
  return 0;
}