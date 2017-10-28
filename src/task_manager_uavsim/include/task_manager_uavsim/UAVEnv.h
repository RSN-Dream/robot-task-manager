#ifndef UAV_ENV_H
#define UAV_ENV_H

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Pose2D.h>
#include <std_msgs/String.h>
#include <task_manager_lib/TaskDefinition.h>
#include <task_manager_sync/TaskEnvironmentSync.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>

namespace task_manager_uavsim {
    class UAVEnv: public task_manager_sync::TaskEnvironmentSync
    {
        protected:
            unsigned int quadId;
            ros::Subscriber poseSub;
            ros::Subscriber wpReachedSub;
            ros::Subscriber motorStateSub;
            ros::Publisher goalPub;
            ros::Publisher motorPub;
            ros::Subscriber batterySub;

            void pose_cb(const geometry_msgs::Twist msg);
            void motor_cb(const std_msgs::Bool msg);
            void wpr_cb(const std_msgs::Bool msg);
            bool wpReached;
            bool motorState;
            void battery_cb(const std_msgs::Float32 msg);
            double batteryVoltage;			
            // Twist is probably not the right type for this message
            geometry_msgs::Twist currentPose;

        public:
            UAVEnv(ros::NodeHandle & nh, const std::string & name, unsigned int id=1);
            ~UAVEnv() {};

            geometry_msgs::Pose2D getPose2D() const ; 

            geometry_msgs::Pose getPose() const ;

            bool isMotorOn() const {
                return motorState;
            }

            void publishGoal(double x, double y, double z, double yaw) {
                geometry_msgs::Twist cmd;
                cmd.linear.x = x;
                cmd.linear.y = y;
                cmd.linear.z = z;
                cmd.angular.z = yaw;
                goalPub.publish(cmd);
            }

            void publishMotorState(bool state) {
                std_msgs::Bool msg;
                msg.data = state;
                motorPub.publish(msg);
            }

            void resetGoalReachedFlag() {
                wpReached = false;
            }

            bool isGoalReached() const {
                return wpReached;
            }

            double getBatteryVoltage() const {
                return batteryVoltage; // Not checked
            }
    };

    typedef boost::shared_ptr<UAVEnv> UAVEnvPtr;
    typedef boost::shared_ptr<UAVEnv const> UAVEnvConstPtr;
};

#endif // UAV_ENV_H
