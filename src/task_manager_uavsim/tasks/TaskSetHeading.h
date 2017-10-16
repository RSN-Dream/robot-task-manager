#ifndef TASK_SetHeading_H
#define TASK_SetHeading_H

#include "task_manager_lib/TaskDefinition.h"
#include "task_manager_uavsim/UAVEnv.h"
#include "task_manager_uavsim/TaskSetHeadingConfig.h"

using namespace task_manager_lib;

namespace task_manager_uavsim {
    class TaskSetHeading : public TaskInstance<TaskSetHeadingConfig, UAVEnv>
    {
        protected:
            double x,y,z;
            ros::Time lastPubTime;

        public:
            TaskSetHeading(TaskDefinitionPtr def, TaskEnvironmentPtr env) : Parent(def,env) {}
            virtual ~TaskSetHeading() {};

            virtual TaskIndicator initialise() ;

            virtual TaskIndicator iterate();

            virtual TaskIndicator terminate();
    };
    class TaskFactorySetHeading : public TaskDefinition<TaskSetHeadingConfig, UAVEnv, TaskSetHeading>
    {

        public:
            TaskFactorySetHeading(TaskEnvironmentPtr env) : 
                Parent("SetHeading","Reach a desired heading",true,env) {}
            virtual ~TaskFactorySetHeading() {};
    };
};

#endif // TASK_SetHeading_H
