#include <math.h>
#include <tf/transform_datatypes.h>
#include "TaskRadioScan.h"
#include "task_manager_uavsim/TaskRadioScanConfig.h"
using namespace task_manager_msgs;
using namespace task_manager_lib;
using namespace task_manager_uavsim;


TaskIndicator TaskRadioScan::initialise()  {
    geometry_msgs::Pose2D p = env->getPose2D();
    initial_heading = p.theta;
    geometry_msgs::Pose P = env->getPose();
    x = P.position.x; y= P.position.y; z = P.position.z;
    step = 0;
    acquiring = false;

    env->resetGoalReachedFlag();
    env->publishGoal(x,y,z, initial_heading);
    lastPubTime = ros::Time::now();
    return TaskStatus::TASK_INITIALISED;
}
            

TaskIndicator TaskRadioScan::iterate()
{
    if (acquiring) {
        if ((ros::Time::now() - lastPubTime).toSec() > cfg.acquisition_time) {
            acquiring = false;
            ROS_INFO("RadioScan: Acquisition sector %d completed",step);
            step += 1;
        } 
    } else {
        if ((ros::Time::now() - lastPubTime).toSec() > 0.5) {
            geometry_msgs::Pose2D p = env->getPose2D();
            double target_angle = initial_heading + step * 2 * M_PI/cfg.num_sectors;
            if (env->isGoalReached() && (fabs(remainder(target_angle - p.theta,2*M_PI)) < 0.05)) {
                if (step == cfg.num_sectors) {
                    return TaskStatus::TASK_COMPLETED;
                }
                lastPubTime = ros::Time::now();
                acquiring = true;
            } else {
                lastPubTime = ros::Time::now();
                env->publishGoal(x,y,z, target_angle);
            }
        }
    }
	return TaskStatus::TASK_RUNNING;
}

TaskIndicator TaskRadioScan::terminate()
{
	return Parent::terminate();
}

DYNAMIC_TASK(TaskFactoryRadioScan);
