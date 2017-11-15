#!/usr/bin/python
# ROS specific imports
import roslib; roslib.load_manifest('task_manager_uavsim')
import rospy
from math import *
from task_manager_lib.TaskClient import *
import traceback
from geometry_msgs.msg import Twist
from std_msgs.msg import Float32

import json
import io

powerLevel = 100 # power level at the beginning
quadPos = [None, None] # quadPos at two instants Past & Current 
def callback_pose(msg):
	global quadPos
	global powerLevel
	
	currentPose = [msg.linear.x,msg.linear.y,msg.linear.z]
	quadPos[0] = quadPos[1]	
	quadPos[1] = currentPose
	
	if not quadPos[0] == None:
		deltaPos = [quadPos[1][0]-quadPos[0][0],quadPos[1][1]-quadPos[0][1],quadPos[1][2]-quadPos[0][2]]
		distance = sqrt(deltaPos[0]**2+deltaPos[1]**2+deltaPos[2]**2)

		BATTERY_DISCHARGE_RATE_MOVING = 0.05
		BATTERY_DISCHARGE_RATE_HOVERING = 0.01

		if distance > 0.01:  # moving
			powerLevel -= distance * BATTERY_DISCHARGE_RATE_MOVING
		elif distance <= 0.01 and quadPos[1][2] > 0.01: # hovering
			powerLevel -= BATTERY_DISCHARGE_RATE_HOVERING
	else:
		quadPos[0] = currentPose

	powerLvlPub.publish(powerLevel)

### main 
rospy.init_node('battery_quad1',anonymous=True)
try:
	rospy.Subscriber("/quad1/pose", Twist, callback_pose)
	powerLvlPub = rospy.Publisher('/quad1/power', Float32, queue_size=10)
	rospy.spin()
except:
	rospy.spin()
