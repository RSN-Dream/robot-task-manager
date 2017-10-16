#include <math.h>
#include <tf/transform_datatypes.h>
#include "TaskGoTo.h"
#include "task_manager_uavsim/TaskGoToConfig.h"
using namespace task_manager_msgs;
using namespace task_manager_lib;
using namespace task_manager_uavsim;


TaskIndicator TaskGoTo::initialise()  {
    ROS_INFO("TaskGoTo: Going to (%.2f,%.2f)",cfg.goal_x,cfg.goal_y);
    geometry_msgs::Pose2D p = env->getPose2D();
    current_heading = p.theta;
    env->resetGoalReachedFlag();
    env->publishGoal(cfg.goal_x,cfg.goal_y,cfg.goal_z, current_heading);
    lastPubTime = ros::Time::now();
    return TaskStatus::TASK_INITIALISED;
}
            

TaskIndicator TaskGoTo::iterate()
{
    geometry_msgs::Pose2D p = env->getPose2D();
    if (env->isGoalReached() && (fabs(remainder(current_heading - p.theta,2*M_PI)) < 0.05)) {
		return TaskStatus::TASK_COMPLETED;
    }
        
    if ((ros::Time::now() - lastPubTime).toSec() > 1.0) {
        lastPubTime = ros::Time::now();
        env->publishGoal(cfg.goal_x,cfg.goal_y,cfg.goal_z, current_heading);
    }
	return TaskStatus::TASK_RUNNING;
}

TaskIndicator TaskGoTo::terminate()
{
	return Parent::terminate();
}

DYNAMIC_TASK(TaskFactoryGoTo);
