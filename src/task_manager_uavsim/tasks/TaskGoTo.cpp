#include <math.h>
#include <tf/transform_datatypes.h>
#include "TaskGoTo.h"
#include "task_manager_uavsim/TaskGoToConfig.h"
using namespace task_manager_msgs;
using namespace task_manager_lib;
using namespace task_manager_uavsim;


TaskIndicator TaskGoTo::initialise()  {
    ROS_INFO("TaskGoTo: Going to (%.2f,%.2f)",cfg.goal_x,cfg.goal_y);
    env->resetGoalReachedFlag();
    env->publishGoal(cfg.goal_x,cfg.goal_y,cfg.goal_z, cfg.goal_heading);
    return TaskStatus::TASK_INITIALISED;
}
            

TaskIndicator TaskGoTo::iterate()
{
    if (env->isGoalReached()) {
		return TaskStatus::TASK_COMPLETED;
    }
        
    env->publishGoal(cfg.goal_x,cfg.goal_y,cfg.goal_z, cfg.goal_heading);
	return TaskStatus::TASK_RUNNING;
}

TaskIndicator TaskGoTo::terminate()
{
	return Parent::terminate();
}

DYNAMIC_TASK(TaskFactoryGoTo);
