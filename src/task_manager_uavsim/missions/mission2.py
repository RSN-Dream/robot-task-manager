#!/usr/bin/python
# ROS specific imports
import roslib; roslib.load_manifest('task_manager_uavsim')
import rospy
from math import *
from task_manager_lib.TaskClient import *
import traceback

rospy.init_node('task_client',anonymous=True)
server_node = rospy.get_param("~server","/quad2_tasks")
default_period = rospy.get_param("~period",0.2)
tc = TaskClient(server_node,default_period)

wp = [ [0., 0., 1.0, 0.0], 
        [0., 2., 1.0, 1.57], 
        [2., 0., 1.0, 1.57],
        [0., -2., 1.0, 3.14], 
        [-2., 0., 1.0,-1.57],
        [0., 0., 1.0, 0.0]] 


try:
    tc.SetMotor(on=True)
    tc.TakeOff()
    for p in wp:
        tc.GoTo(goal_x=p[0],goal_y=p[1], goal_z=p[2])
        tc.SetHeading(goal_heading=p[3])

    tc.Land()
    tc.SetMotor(on=False)
except:
    traceback.print_exc()
    rospy.spin()

rospy.loginfo("Mission completed")


