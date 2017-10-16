#ifndef TASK_RadioScan_H
#define TASK_RadioScan_H

#include "task_manager_lib/TaskDefinition.h"
#include "task_manager_uavsim/UAVEnv.h"
#include "task_manager_uavsim/TaskRadioScanConfig.h"

using namespace task_manager_lib;

namespace task_manager_uavsim {
    class TaskRadioScan : public TaskInstance<TaskRadioScanConfig, UAVEnv>
    {
        protected:
            double x,y,z;
            double initial_heading;
            unsigned int step;
            bool acquiring;
            ros::Time lastPubTime;

        public:
            TaskRadioScan(TaskDefinitionPtr def, TaskEnvironmentPtr env) : Parent(def,env) {}
            virtual ~TaskRadioScan() {};

            virtual TaskIndicator initialise() ;

            virtual TaskIndicator iterate();

            virtual TaskIndicator terminate();
    };
    class TaskFactoryRadioScan : public TaskDefinition<TaskRadioScanConfig, UAVEnv, TaskRadioScan>
    {

        public:
            TaskFactoryRadioScan(TaskEnvironmentPtr env) : 
                Parent("RadioScan","Build a radio scan of the environment",true,env) {}
            virtual ~TaskFactoryRadioScan() {};
    };
};

#endif // TASK_RadioScan_H
