#!/usr/bin/python
# ROS specific imports
import roslib; roslib.load_manifest('task_manager_uav')
import rospy
from math import *
from task_manager_lib.TaskClient import *

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


tc.TakeOff()
for p in wp:
    tc.GoTo(goal_x=p[0],goal_y=p[1], goal_z=p[2],goal_heading=p[3])

tc.Land()

rospy.loginfo("Mission completed")


