#ifndef TASK_WAIT_FOR_LowBattery_H
#define TASK_WAIT_FOR_LowBattery_H

#include "task_manager_lib/TaskDefinition.h"
#include "task_manager_uavsim/UAVEnv.h"
#include "task_manager_uavsim/TaskWaitForLowBatteryConfig.h"

using namespace task_manager_lib;

namespace task_manager_uavsim {
    class TaskWaitForLowBattery : public TaskInstance<TaskWaitForLowBatteryConfig,UAVEnv>
    {
        public:
            TaskWaitForLowBattery(TaskDefinitionPtr def, TaskEnvironmentPtr env) : Parent(def,env) {}
            virtual ~TaskWaitForLowBattery() {};

            virtual TaskIndicator initialise();

            virtual TaskIndicator iterate();

            virtual TaskIndicator terminate();

    };
    class TaskFactoryWaitForLowBattery : public TaskDefinition<TaskWaitForLowBatteryConfig, UAVEnv, TaskWaitForLowBattery>
    {
        public:
            TaskFactoryWaitForLowBattery(TaskEnvironmentPtr env) : 
                Parent("WaitForLowBattery","wait for battery level to be critical",true,env) {}
            virtual ~TaskFactoryWaitForLowBattery() {};
    };
};

#endif // TASK_WAIT_FOR_LowBattery_H
