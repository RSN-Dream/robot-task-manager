#include <math.h>
#include "TaskLand.h"
#include "task_manager_uavsim/TaskLandConfig.h"
using namespace task_manager_msgs;
using namespace task_manager_lib;
using namespace task_manager_uavsim;


TaskIndicator TaskLand::initialise() 
{
    ROS_INFO("Start Landing");
    geometry_msgs::Pose2D p = env->getPose2D();
    xl = p.x; yl = p.y; yawl = p.theta;
    env->resetGoalReachedFlag();
    env->publishGoal(xl,yl,0.2,yawl);
    lastPubTime = ros::Time::now();
    return TaskStatus::TASK_INITIALISED;
}


TaskIndicator TaskLand::iterate()
{ 
    if (env->isGoalReached()) {
        ROS_INFO("Landing Successful");
        return TaskStatus::TASK_COMPLETED;
    }         
    if ((ros::Time::now() - lastPubTime).toSec() > 1.0) {
        lastPubTime = ros::Time::now();
        env->publishGoal(xl,yl,0.2,yawl);
    }
    return TaskStatus::TASK_RUNNING;
}

TaskIndicator TaskLand::terminate()
{
    return TaskStatus::TASK_TERMINATED;
}

DYNAMIC_TASK(TaskFactoryLand);
