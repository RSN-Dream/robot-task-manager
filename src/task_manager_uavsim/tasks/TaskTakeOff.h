#ifndef TASK_TAKE_OFF_H
#define TASK_TAKE_OFF_H

#include <std_msgs/Int32.h>
#include "task_manager_lib/TaskDefinition.h"
#include "task_manager_uavsim/UAVEnv.h"
#include "task_manager_uavsim/TaskTakeOffConfig.h"

using namespace task_manager_lib;

namespace task_manager_uavsim {
    class TaskTakeOff : public TaskInstance<TaskTakeOffConfig,UAVEnv>
    {
        protected:
            double xl,yl,yawl; // current value so that landing only affect z.

        public:
            TaskTakeOff(TaskDefinitionPtr def, TaskEnvironmentPtr env) : Parent(def,env) {}
            virtual ~TaskTakeOff() {};

            virtual TaskIndicator initialise() ;

            virtual TaskIndicator iterate();

            virtual TaskIndicator terminate();
    };
    class TaskFactoryTakeOff : public TaskDefinition<TaskTakeOffConfig, UAVEnv, TaskTakeOff>
    {

        public:
            TaskFactoryTakeOff(TaskEnvironmentPtr env) : 
                Parent("TakeOff","UAV Take Off",true,env) {}
            virtual ~TaskFactoryTakeOff() {};
    };
};

#endif // TASK_TAKE_OFF_H
