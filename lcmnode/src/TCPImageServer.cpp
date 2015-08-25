//
// Created by drc on 8/24/15.
//

#include "TCPImageServer.h"
#include <iostream>

#define CHUNK_SIZE 4096
#define BYTES_PER_PIXEL 3

using namespace std;
using namespace libsocket;

TCPImageServer::TCPImageServer(string const &host, string const &port, const bool flip_image)
  :m_flip(flip_image), m_good(false), m_host(host), m_port(port), m_server(host, port, LIBSOCKET_IPv4)
{
  m_server_set.add_fd(m_server, LIBSOCKET_READ);
  m_server_stream = nullptr;
}


void TCPImageServer::wait_connect() {
  m_readypair = m_server_set.wait();
  m_ready_server = dynamic_cast<inet_stream_server *>(m_readypair.first.back());
  m_readypair.first.pop_back();
  m_server_stream = m_ready_server->accept();
  m_good = true;
}


ImageData TCPImageServer::get_image() {

  try {
    int width = read_int();
    int height = read_int();
    return m_flip ? flip_image(read_image(width, height)) : read_image(width, height);
  } catch (const socket_exception& ex) {
    cerr << ex.mesg << endl;
    shutdown();
  }
}

int TCPImageServer::read_int() {
  size_t bytes_received = 0;
  int value;

  if(m_server_stream != nullptr) {
    bytes_received = m_server_stream->rcv(&value, 4, 0);
  }

  if(bytes_received != 4) {
    throw socket_exception("TCPImageServer", 49 , "Failed to read int from server stream");
  }

  return value;
}

ImageData TCPImageServer::read_image(const int width, const int height) {
  ImageData img;
  img.width = width;
  img.height = height;
  int data_remaining = width * height * BYTES_PER_PIXEL;
  int current_offset = 0;

  if (m_server_stream != nullptr) {
    img.data = new uint8_t[data_remaining];
    while (data_remaining > 0) {
      size_t bytes_received = m_server_stream->rcv(img.data + current_offset, min(data_remaining, CHUNK_SIZE), 0);
      data_remaining -= bytes_received;
      current_offset += bytes_received;
    }
  }

  if(data_remaining != 0) {
    throw socket_exception("TCPImageServer", 64, "Failed to read image data from server stream");
  }

  return img;
}

void TCPImageServer::shutdown() {
  m_good = false;
  m_server.destroy();
}

TCPImageServer::~TCPImageServer() {
  shutdown();
}

bool TCPImageServer::good() {
  return m_good;
}

ImageData TCPImageServer::flip_image(ImageData const &img) {
  const int stride = img.width * BYTES_PER_PIXEL;
  ImageData flipped_image;
  flipped_image.width = img.width;
  flipped_image.height = img.height;
  flipped_image.data = new uint8_t[img.height * stride];
  for(int row = 0; row < img.height ; row++) {
    memcpy(flipped_image.data + row * stride, img.data + (img.height - row - 1) * stride, stride);
  }
  return flipped_image;
}
