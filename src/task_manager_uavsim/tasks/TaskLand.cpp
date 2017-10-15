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
    env->publishGoal(xl,yl,0,yawl);
    return TaskStatus::TASK_INITIALISED;
}


TaskIndicator TaskLand::iterate()
{ 
    if (env->isGoalReached()) {
        return TaskStatus::TASK_COMPLETED;
    }         
    env->publishGoal(xl,yl,0,yawl);
    return TaskStatus::TASK_RUNNING;
}

TaskIndicator TaskLand::terminate()
{
    ROS_INFO("Landing Successful");
    return TaskStatus::TASK_TERMINATED;
}

DYNAMIC_TASK(TaskFactoryLand);
