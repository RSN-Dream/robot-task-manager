#!/usr/bin/python
# ROS specific imports
import roslib; roslib.load_manifest('task_manager_uavsim')
import rospy
from math import *
from task_manager_lib.TaskClient import *
import traceback

rospy.init_node('task_client',anonymous=True)
server_node = rospy.get_param("~server","/quad1_tasks")
default_period = rospy.get_param("~period",0.2)
tc = TaskClient(server_node,default_period)


wp = [ [0., 0., 0.5, 0.0], 
        [0., 2., 0.5, 1.57], 
        [2., 0., 0.5, 1.57],
        [0., -2., 0.5, 3.14], 
        [-2., 0., 0.5,-1.57],
        [0., 0., 0.5, 0.0]] 


try:
    tc.SetStatusSync(status=0);
    tc.SetMotor(on=True)
    tc.TakeOff()
    tc.SetStatusSync(status=1);
    tc.WaitForStatusSync(partner="partner2",status=4);
    for i,p in enumerate(wp):
        tc.GoTo(goal_x=p[0],goal_y=p[1], goal_z=p[2])
        tc.SetHeading(goal_heading=p[3])
        tc.SetStatusSync(status=i+2);

    tc.SetStatusSync(status=0);
    tc.Land()
    tc.SetMotor(on=False)
except:
    traceback.print_exc()
    rospy.spin()

rospy.loginfo("Mission completed")


