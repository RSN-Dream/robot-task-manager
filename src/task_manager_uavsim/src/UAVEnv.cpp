
#include "task_manager_uavsim/UAVEnv.h"
#include "tf/transform_datatypes.h"

using namespace task_manager_uavsim;

UAVEnv::UAVEnv(ros::NodeHandle & n, 
        const std::string & name, unsigned int id) : task_manager_sync::TaskEnvironmentSync(n,name,"sync"), quadId(id)
{
    char idstrc[128];
    sprintf(idstrc,"/quad%d",quadId);
    std::string idstr(idstrc);

    goalPub = nh.advertise<geometry_msgs::Twist>(idstr+"/go",1);
    motorPub = nh.advertise<std_msgs::Bool>(idstr+"/motor",1);

    poseSub = nh.subscribe(idstr+"/pose",1,&UAVEnv::pose_cb,this);
    wpReachedSub = nh.subscribe(idstr+"/wp_reached",1,&UAVEnv::wpr_cb,this);
    motorStateSub = nh.subscribe(idstr+"/motor_status",1,&UAVEnv::motor_cb,this);
    batterySub = nh.subscribe(idstr+"/power",1,&UAVEnv::battery_cb,this);
}

void UAVEnv::pose_cb(const geometry_msgs::Twist msg) 
{
    currentPose = msg;
}


void UAVEnv::wpr_cb(const std_msgs::Bool msg) 
{
    wpReached = msg.data;
}

void UAVEnv::motor_cb(const std_msgs::Bool msg) 
{
    motorState = msg.data;
}

void UAVEnv::battery_cb(const std_msgs::Float32 msg) 
{
    batteryVoltage = msg.data;
}


geometry_msgs::Pose2D UAVEnv::getPose2D() const {
    geometry_msgs::Pose2D pose;
    pose.theta = currentPose.angular.z;
    pose.x = currentPose.linear.x;
    pose.y = currentPose.linear.y;
    return pose;
}

geometry_msgs::Pose UAVEnv::getPose() const {
    geometry_msgs::Pose pose;
    tf::quaternionTFToMsg(tf::createQuaternionFromRPY(0,0,currentPose.angular.z),
            pose.orientation);
    pose.position.x = currentPose.linear.x;
    pose.position.y = currentPose.linear.y;
    pose.position.z = currentPose.linear.z;
    return pose;
}

