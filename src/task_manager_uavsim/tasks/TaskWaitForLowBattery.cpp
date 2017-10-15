#include <math.h>
#include "TaskWaitForLowBattery.h"
#include "task_manager_uavsim/TaskWaitForLowBatteryConfig.h"
using namespace task_manager_msgs;
using namespace task_manager_lib;
using namespace task_manager_uavsim;

TaskIndicator TaskWaitForLowBattery::initialise()
{
    return TaskStatus::TASK_INITIALISED;
}

TaskIndicator TaskWaitForLowBattery::iterate()
{
    if (env->getBatteryVoltage()<cfg.threshold) {
        ROS_WARN("Battery  is low !!");
        return TaskStatus::TASK_COMPLETED;
    }
    return TaskStatus::TASK_RUNNING;
}

TaskIndicator TaskWaitForLowBattery::terminate()
{
        return TaskStatus::TASK_TERMINATED;
}

DYNAMIC_TASK(TaskFactoryWaitForLowBattery);
