#!/usr/bin/python
# ROS specific imports
import roslib; roslib.load_manifest('task_manager_uavsim')
import rospy
from math import *
from task_manager_lib.TaskClient import *
from geometry_msgs.msg import Twist
import mission_constants as mc
import traceback

rospy.init_node('task_client',anonymous=True)
server_node = rospy.get_param("~server","/quad1_tasks")
default_period = rospy.get_param("~period",0.2)
tc = TaskClient(server_node,default_period)


wp = [ [-8.8, -5.7, 1.0, True],
        [-8.8, -5.7, 1.0, True],
        [-8.8, -5.7, 1.0, True],
        [-8.8, -5.7, 1.0, True],
        [-8.8, -5.7, 1.0, True],
        [-8.8, -5.7, 1.0, True],
        [-8.8, -5.7, 1.0, True],
        [-8.8, -5.7, 1.0, True],
        [-8.8, -5.7, 1.0, True],
        [-8.8, -5.7, 1.0, True],
        [-8.8, -5.7, 1.0, True],
        [-8.8, -5.7, 1.0, True],
        [2., 0., 1.0, False], 
        [2., 2., 1.0, True], 
        [3., 0., 1.0, True],
        [2., -2., 1.0, True], 
        [1., 0., 1.0,True],
        [2., 0., 1.0, False]] 

p2 = None
def p2_pose_cb(msg):
    global p2
    p2 = msg

# subscribe to /rcvdPoseQuad2 published by the communication node, 
# instead of directly to /poseQuad2
sub = rospy.Subscriber("/quad2/pose",Twist, p2_pose_cb,queue_size=1)  
#sub = rospy.Subscriber("/rcvdPoseQuad2",Twist, p2_pose_cb,queue_size=1)  



try:
    tc.SetStatusSync(status=mc.QUAD_DOWN);
    tc.SetMotor(on=True)
    tc.TakeOff()

    # Wait for other quad to have taken off
    tc.SetStatusSync(status=mc.QUAD_FLYING);
    tc.WaitForStatusSync(partner="partner2",status=mc.QUAD_FLYING);

    w4partner = tc.WaitForStatusSync(foreground=False,partner="partner2",status=mc.QUAD_CALLING)
    tc.addCondition(ConditionIsCompleted("Partner2",tc,w4partner))
    w4battery = tc.WaitForLowBattery(foreground=False,threshold=70.0)
    tc.addCondition(ConditionIsCompleted("Battery",tc,w4battery))
    try:
        for i,p in enumerate(wp):
            tc.GoTo(goal_x=p[0],goal_y=p[1], goal_z=p[2])
            if p[3]:
                tc.RadioScan(num_sectors=3,acquisition_time=2.0)
        tc.SetStatusSync(status=mc.QUAD_DOWN);
        tc.clearConditions()

    except TaskConditionException, e:
        cnames = [str(c) for c in e.conditions]
        if "Partner2" in cnames:
            # We've been called...
            rospy.loginfo("Partner2 requested intervention, interrupting mission")
            tc.GoTo(goal_x=p2.linear.x,goal_y=p2.linear.y, goal_z=1.0)
            tc.RadioScan(num_sectors=3,acquisition_time=5.0)
            tc.SetStatusSync(status=mc.QUAD_DONE_HELPING);
            tc.GoTo(goal_x=wp[-1][0],goal_y=wp[-1][1], goal_z=wp[-1][2])

        if "Battery" in cnames:
            rospy.loginfo("Detected low battery, interrupting mission")
            tc.SetStatusSync(status=mc.QUAD_LOW_BAT);
            tc.GoTo(goal_x=wp[-1][0],goal_y=wp[-1][1], goal_z=wp[-1][2])
    
    tc.Land()
    tc.SetMotor(on=False)
except:
    traceback.print_exc()
    rospy.spin()

rospy.loginfo("Mission completed")


