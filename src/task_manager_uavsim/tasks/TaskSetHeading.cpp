#include <math.h>
#include <tf/transform_datatypes.h>
#include "TaskSetHeading.h"
#include "task_manager_uavsim/TaskSetHeadingConfig.h"
using namespace task_manager_msgs;
using namespace task_manager_lib;
using namespace task_manager_uavsim;


TaskIndicator TaskSetHeading::initialise()  {
    geometry_msgs::Pose p = env->getPose();
    x=p.position.x; y=p.position.y; z=p.position.z;
    env->resetGoalReachedFlag();
    env->publishGoal(x,y,z, cfg.goal_heading);
    lastPubTime = ros::Time::now();
    return TaskStatus::TASK_INITIALISED;
}
            

TaskIndicator TaskSetHeading::iterate()
{
    if (env->isGoalReached()) {
		return TaskStatus::TASK_COMPLETED;
    }
        
    if ((ros::Time::now() - lastPubTime).toSec() > 1.0) {
        lastPubTime = ros::Time::now();
        env->publishGoal(x,y,z, cfg.goal_heading);
    }
	return TaskStatus::TASK_RUNNING;
}

TaskIndicator TaskSetHeading::terminate()
{
	return Parent::terminate();
}

DYNAMIC_TASK(TaskFactorySetHeading);
