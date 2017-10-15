#ifndef TASK_GOTO_H
#define TASK_GOTO_H

#include "task_manager_lib/TaskDefinition.h"
#include "task_manager_uavsim/UAVEnv.h"
#include "task_manager_uavsim/TaskGoToConfig.h"

using namespace task_manager_lib;

namespace task_manager_uavsim {
    class TaskGoTo : public TaskInstance<TaskGoToConfig, UAVEnv>
    {
        public:
            TaskGoTo(TaskDefinitionPtr def, TaskEnvironmentPtr env) : Parent(def,env) {}
            virtual ~TaskGoTo() {};

            virtual TaskIndicator initialise() ;

            virtual TaskIndicator iterate();

            virtual TaskIndicator terminate();
    };
    class TaskFactoryGoTo : public TaskDefinition<TaskGoToConfig, UAVEnv, TaskGoTo>
    {

        public:
            TaskFactoryGoTo(TaskEnvironmentPtr env) : 
                Parent("GoTo","Reach a desired destination",true,env) {}
            virtual ~TaskFactoryGoTo() {};
    };
};

#endif // TASK_GOTO_H
