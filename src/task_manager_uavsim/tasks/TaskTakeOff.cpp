#include <math.h>
#include "TaskTakeOff.h"
#include "task_manager_uavsim/TaskTakeOffConfig.h"
using namespace task_manager_msgs;
using namespace task_manager_lib;
using namespace task_manager_uavsim;


TaskIndicator TaskTakeOff::initialise() 
{
    ROS_INFO("Start Take-Off");
    geometry_msgs::Pose2D p = env->getPose2D();
    xl = p.x; yl = p.y; yawl = p.theta;
    env->resetGoalReachedFlag();
    env->publishGoal(xl,yl,cfg.height,yawl);
    return TaskStatus::TASK_INITIALISED;
}


TaskIndicator TaskTakeOff::iterate()
{ 
    if (env->isGoalReached()) {
        return TaskStatus::TASK_COMPLETED;
    }         
    env->publishGoal(xl,yl,cfg.height,yawl);
}

TaskIndicator TaskTakeOff::terminate()
{
    ROS_INFO("Take-Off Successful");
    return TaskStatus::TASK_TERMINATED;
}

DYNAMIC_TASK(TaskFactoryTakeOff);
