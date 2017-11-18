#!/usr/bin/env python

import rospy
import roslib; roslib.load_manifest('task_manager_uavsim')
from std_msgs.msg import String
from geometry_msgs.msg import Twist

import threading
import serial
import time
import json

global connected
connected = False

global ser_mutex
ser_mutex = threading.Lock()

### Package: "##, SenderID, ReceiverID, Data, **"
def encode_package(message):
    return '##,' + str(MY_IP) + ',' + message + ',**,'


def decode_package(package):
    pck_split = [x.strip() for x in package.split(',')]
    return pck_split


def send_package(package):
    global ser_mutex
    global serial_port
    ser_mutex.acquire()    
    serial_port.write(package)
    ser_mutex.release()

# To publish the received data as a twist
def translateStr2Twist(msg):
    twistData = Twist()
    decodedMsg = decode_package(msg)
    try:
        if len(decodedMsg) == 7:
            rcvrQuadId = decodedMsg[0] # not used yet
            twistData.linear.x = float(decodedMsg[1])
            twistData.linear.y = float(decodedMsg[2])
            twistData.linear.z = float(decodedMsg[3])
            twistData.angular.x = float(decodedMsg[4])
            twistData.angular.y = float(decodedMsg[5])
            twistData.angular.z = float(decodedMsg[6])
    except:
        pass
    return twistData
    
def handle_data(inputData):
	print(inputData + '\n')

	twistData = translateStr2Twist(inputData)
	rcvdPosePub.publish(twistData)


def read_from_port(ser):
	global ser_mutex
	global connected
	while not connected:
		connected = True

		data_string = ''
		while True:
			ser_mutex.acquire()
			reading = ser.readline().decode()
			ser_mutex.release()
			if len(reading) > 0:
				data_string = data_string + reading
				data_split = decode_package(data_string)
				if '**' in data_split:
					last_idx = data_split.index('**')
					package_split = data_split[0:last_idx+1]

					if package_split[0] == '':
						del package_split[0]
					
					if package_split[2] == MY_IP:
						package = ','.join(package_split[2:9])
						handle_data(package)
					package_remaining = data_split[last_idx+1:len(data_split)]
					data_string = ', '.join(package_remaining)


def translateTwist2Str(msg):
	linX = format(msg.linear.x,'.4f')
	linY = format(msg.linear.y,'.4f')
	linZ = format(msg.linear.z,'.4f')
	angX = format(msg.angular.x,'.4f')
	angY = format(msg.angular.y,'.4f')
	angZ = format(msg.angular.z,'.4f')
	sMsg = linX + "," + linY + "," + linZ + "," + angX + "," + angY + "," + angZ

	otherQuadId = "192.168.1.101"
	receiverId = otherQuadId
	return receiverId + ',' + sMsg


def callback_send_pose(msg):
    pck = translateTwist2Str(msg) 
    pck = encode_package(pck)
    print("callback_send_pose:",pck)
    send_package(pck)


def load_config_data():
	global MY_ID
	global MY_PORT
	global MY_BAUD
	global MY_IP

	configFile = open('configFile2.json', 'r')
	configData = json.load(configFile)

	MY_ID = str(configData['my_id'])
	print "MY ID: ", MY_ID
	MY_PORT = str(configData['my_port'])
	print "MY PORT: ", MY_PORT
	MY_BAUD = int(configData['my_baud'])
	MY_IP = str(configData['my_ip'])

###### main #########

load_config_data()

rospy.init_node("comm_quad2")

print("Ready to broadcast!")

serial_port = serial.Serial(MY_PORT, MY_BAUD, timeout=0)
thread = threading.Thread(target=read_from_port, args=(serial_port,))
thread.start()

try:
    rospy.Subscriber("/quad2/pose", Twist, callback_send_pose)
    rcvdPosePub = rospy.Publisher('/quad1/rcvdPose', Twist, queue_size=10)

    RATE_VAL = 4
    rate = rospy.Rate(RATE_VAL) # 4hz
    rosthread = threading.Thread(name="ros spin thread", target=rospy.spin)
    rosthread.start()
except:
    rospy.spin()
