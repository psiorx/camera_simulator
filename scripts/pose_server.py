#!/usr/bin/env python

import rospy
import SocketServer
import struct
import time
from math import sin, cos

class TCPPoseServer(SocketServer.BaseRequestHandler):
    def handle(self):
        t = 0
        pose_rate = 60
        r = rospy.Rate(pose_rate)
        while True:
            if rospy.is_shutdown():
                print("rospy is shutting down")
                self.server.socket.close();
                return
            t = t + 1.0/pose_rate;
            pose = [2*sin(t),6.5,-10.0 + 2*cos(t),0.0,0.0,0.0]
            print pose
            r.sleep()
            buf = struct.pack('%sf' % 6, *pose)
            self.request.sendall(buf)

if __name__ == '__main__':
    try:
        rospy.init_node('PoseServer', anonymous=True)        
        HOST, PORT = "localhost", 12346 #todo: pass these in
        server = SocketServer.TCPServer((HOST, PORT), TCPPoseServer)
        server.serve_forever()
    except KeyboardInterrupt:
        print('closing server')
        server.server_close();
        server.shutdown();
