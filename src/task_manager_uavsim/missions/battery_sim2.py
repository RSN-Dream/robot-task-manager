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

try:
    to_unicode = unicode
except NameError:
    to_unicode = str


rospy.init_node('battery_quad2',anonymous=True)
#server_node = rospy.get_param("~server","/quad1_tasks")
#default_period = rospy.get_param("~period",0.2)
#tc = TaskClient(server_node,default_period)


def callback_pose(msg):
	try:
		# Read JSON file
		with open('dataQuad2.json') as data_file:
			data_loaded = json.load(data_file)
			quadPos = data_loaded["position"]
			powerLevel = data_loaded["power"]
	except:
		powerLevel = 100 # power level at the beginning
		quadPos = [0,0,0]
	
	quadPosNext = [msg.linear.x,msg.linear.y,msg.linear.z]
	deltaPos = [quadPosNext[0]-quadPos[0],quadPosNext[1]-quadPos[1],quadPosNext[2]-quadPos[2]]
	distance = sqrt(deltaPos[0]**2+deltaPos[1]**2+deltaPos[2]**2)
	

	BATTERY_DISCHARGE_RATE_MOVING = 0.15
	BATTERY_DISCHARGE_RATE_HOVERING = 0.03

	if distance > 0.01:  # moving
		powerLevel -= distance * BATTERY_DISCHARGE_RATE_MOVING
	elif distance <= 0.01 and quadPosNext[2] > 0.01: # hovering
		powerLevel -= BATTERY_DISCHARGE_RATE_HOVERING


	data = {"position": quadPosNext, "power":powerLevel}
	# Write to JSON file
	with io.open('dataQuad2.json', 'w', encoding='utf8') as outfile:
		str_ = json.dumps(data,indent=4, sort_keys=True, separators=(',', ': '), ensure_ascii=False)
		outfile.write(to_unicode(str_))

	powerLvlPub = rospy.Publisher('powerQuad2', Float32, queue_size=10)
	powerLvlPub.publish(powerLevel)


try:
	rospy.Subscriber("/poseQuad2", Twist, callback_pose)
	rospy.spin()
except:
	rospy.spin()
