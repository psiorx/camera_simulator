#!/usr/bin/env python

import rospy
import SocketServer
import struct
import time

from sensor_msgs.msg import Image

class ImagePublisher:
    def __init__(self, topic, maxqueue):
        self.pub = rospy.Publisher(topic, Image, queue_size=maxqueue)
        rospy.init_node('UnityPublisher', anonymous=True)        

    def publishImage(self, width, height, imageData):
        msg = Image()
        msg.header.stamp = rospy.Time.now()
        msg.width = width
        msg.height = height
        msg.encoding = 'rgb8'
        msg.step = width * 3
        msg.data = list(imageData)
        self.pub.publish(msg)

imagePublisher = ImagePublisher('unity_image', 10)

class TCPImageServer(SocketServer.BaseRequestHandler):
    
    def readInt(self):
        data = self.request.recv(4)
        
        if len(data) == 4:
            value = struct.unpack("i", data)[0]
            return value, True
        else:
            return 0, False

    def readImage(self, imageWidth, imageHeight, chunkSize):
        image = bytearray()
        dataremaining = imageWidth * imageHeight * 3
        while dataremaining > 0:
            data = self.request.recv(min(dataremaining, chunkSize))
            dataremaining = dataremaining - len(data)
            image.extend(data)
        if dataremaining == 0:
            return image, True
        else:
            return 0, False


    def handle(self):
        global imagePublisher
        while True:
            if rospy.is_shutdown():
                print("rospy is shutting down")
                self.server.socket.close();
                return
            
            imageWidth, success = self.readInt()
            
            if not success:
                print("failed to read image width")
                return

            imageHeight, success = self.readInt()
            
            if not success:
                print("failed to read image height")
                return

            image, success = self.readImage(imageWidth, imageHeight, 4096)

            if success:
                imagePublisher.publishImage(imageWidth, imageHeight, image)
            else:
                print("failed to read image data")
                return

if __name__ == '__main__':
    try:
        HOST, PORT = "localhost", 12345 #todo: pass these in
        server = SocketServer.TCPServer((HOST, PORT), TCPImageServer)
        server.serve_forever()
    except KeyboardInterrupt:
        print('closing server')
        server.server_close();
        server.shutdown();
