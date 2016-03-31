#!/usr/bin/env python
import rospy
import socket

from geometry_msgs.msg import Pose2D
from std_msgs.msg import String

def main():
   #init TCP Socket
   TCP_IP = 'localhost'
   TCP_PORT = 6666
   BUFFER_SIZE = 1024
   s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   s.connect((TCP_IP, TCP_PORT))
   #init ROS Node
   rospy.init_node('cosphi_ros_bridge_node')
   #to send all the robots at one string like the original CosPhi 
   #but as a ROS topic 
   pub = rospy.Publisher('/cosphi_ros_bridge/poses', String,queue_size=10)
   
   rate = rospy.Rate(100)
   print("connected to the CosPhi- Topics are Publishing....")
   while not rospy.is_shutdown():
      data = s.recv(BUFFER_SIZE)
      pub.publish(data)
      #parse to extract data of each robot  
      Array_Robots= data.split('\n')
      #parse to extract the coordinats of each robot , add them to the standerd form then publish them on a topic defined by the robot ID
      for index in range(len(Array_Robots)-2):
         Array_All = Array_Robots[index+1].split()
         ID = Array_All[1]
         Topic_Name = '/cosphi_ros_bridge/robot'+ID
         pub1 = rospy.Publisher(Topic_Name, Pose2D, queue_size=10)
         X = float(Array_All[2])
         Y = float(Array_All[3])
         Theta = float(Array_All[4])
         data1 =Pose2D(X, Y, Theta)
	 pub1.publish(data1)
  	 rate.sleep()






   

if __name__ == '__main__':
    main()   
	
  
