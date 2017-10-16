#include <math.h>
#include "TaskSetMotor.h"
#include "task_manager_uavsim/TaskSetMotorConfig.h"
using namespace task_manager_msgs;
using namespace task_manager_lib;
using namespace task_manager_uavsim;


TaskIndicator TaskSetMotor::initialise() 
{
    return TaskStatus::TASK_INITIALISED;
}


TaskIndicator TaskSetMotor::iterate()
{
    if (env->isMotorOn() == cfg.on) {
        return TaskStatus::TASK_COMPLETED;
    }
    env->publishMotorState(cfg.on);
	return TaskStatus::TASK_RUNNING;
}

TaskIndicator TaskSetMotor::terminate()
{
	return TaskStatus::TASK_TERMINATED;
}

DYNAMIC_TASK(TaskFactorySetMotor);
