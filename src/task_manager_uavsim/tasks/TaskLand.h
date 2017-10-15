#ifndef TASK_TAKE_OFF_H
#define TASK_TAKE_OFF_H

#include <std_msgs/Int32.h>
#include "task_manager_lib/TaskDefinition.h"
#include "task_manager_uavsim/UAVEnv.h"
#include "task_manager_uavsim/TaskLandConfig.h"

using namespace task_manager_lib;

namespace task_manager_uavsim {
    class TaskLand : public TaskInstance<TaskLandConfig,UAVEnv>
    {
        protected:
            double xl,yl,yawl; // current value so that landing only affect z.

        public:
            TaskLand(TaskDefinitionPtr def, TaskEnvironmentPtr env) : Parent(def,env) {}
            virtual ~TaskLand() {};

            virtual TaskIndicator initialise() ;

            virtual TaskIndicator iterate();

            virtual TaskIndicator terminate();
    };
    class TaskFactoryLand : public TaskDefinition<TaskLandConfig, UAVEnv, TaskLand>
    {

        public:
            TaskFactoryLand(TaskEnvironmentPtr env) : 
                Parent("Land","UAV Landing",true,env) {}
            virtual ~TaskFactoryLand() {};
    };
};

#endif // TASK_TAKE_OFF_H
