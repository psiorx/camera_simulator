#include <iostream>
#include "TCPImageServer.h"

#include <lcm/lcm.h>
#include <lcmtypes/bot_core_image_t.h>

using namespace std;

int main() {

  lcm_t* lcm = lcm_create(NULL);

  if (!lcm) {
    cerr << "couldn't initialize LCM" << endl;
    return 1;
  }
  TCPImageServer image_server("localhost", "12345", true);
  image_server.wait_connect();

  while (image_server.good()) {
    ImageData img = image_server.get_image();
    bot_core_image_t image_msg;
    image_msg.utime = 0;
    image_msg.nmetadata = 0;
    image_msg.width = img.width;
    image_msg.height = img.height;
    image_msg.pixelformat = BOT_CORE_IMAGE_T_PIXEL_FORMAT_RGB;
    image_msg.row_stride = img.width * 3;
    image_msg.size = image_msg.row_stride * img.height;
    image_msg.data = img.data;
    bot_core_image_t_publish(lcm, "unity_image", &image_msg);
  }

  return 0;
}
