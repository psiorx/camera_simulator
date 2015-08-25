#ifndef LCMNODE_TCPIMAGESERVER_H
#define LCMNODE_TCPIMAGESERVER_H

#include <string>
#include <stdint.h>
#include <libsocket/inetserverstream.hpp>
#include <libsocket/select.hpp>
#include <iostream>

struct ImageData{
    int width = 0;
    int height = 0;
    uint8_t *data = nullptr;
    ~ImageData() {
        if(data != nullptr) {
            delete []  data;
            data = nullptr;
        }
    }
};

class TCPImageServer {
    std::string m_host;
    std::string m_port;
    libsocket::inet_stream_server m_server;
    libsocket::inet_stream_server* m_ready_server;
    libsocket::inet_stream *m_server_stream;
    libsocket::selectset<libsocket::inet_stream_server> m_server_set;
    libsocket::selectset<libsocket::inet_stream_server>::ready_socks m_readypair;
    bool m_good;
    bool m_flip;
    int read_int();
    ImageData read_image(const int width, const int height);
    ImageData flip_image(ImageData const& img);
public:
    TCPImageServer(std::string const& host, std::string const& port, const bool flip_image = false);
    virtual ~TCPImageServer();
    void wait_connect();
    bool good();
    ImageData get_image();
    void shutdown();
};

#endif //LCMNODE_TCPIMAGESERVER_H
