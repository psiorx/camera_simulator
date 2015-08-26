#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include <image_transport/image_transport.h>
#include "TCPImageServer.h"

using namespace std;

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "unity_image");
  ros::NodeHandle n;
//  ros::Publisher image_pub = n.advertise<sensor_msgs::Image>("unity_image_topic", 1000);
  image_transport::ImageTransport img_trans(n);
  image_transport::Publisher image_pub = img_trans.advertise("unity/image", 10);

  ROS_INFO("Creating image server...");
  TCPImageServer image_server("localhost", "12345", true);
  ROS_INFO("Waiting for connection...");
  image_server.wait_connect();
  ROS_INFO("Got a connection...");
  while (image_server.good() && ros::ok())
  {
    ImageData img = image_server.get_image();
    sensor_msgs::Image img_msg;
    img_msg.width = img.width;
    img_msg.height = img.height;
    img_msg.encoding = "rgb8";
    img_msg.step = img.width * 3;
    img_msg.data = vector<uint8_t>(img.data.get(), img.data.get() + img.height * img_msg.step);
    ROS_INFO("Publishing %dx%d image", img.width, img.height);
    image_pub.publish(img_msg);
    ros::spinOnce();
  }


  return 0;
}
