//
// Created by sfaxi19 on 10.01.18.
//

#ifndef VIDEO_STREAMING_UDP_HEADERS_HPP
#define VIDEO_STREAMING_UDP_HEADERS_HPP

#include "iostream"

struct udp_fragment {
    uint16_t id = 0;
    uint16_t fragments = 0;
    uint16_t length = 0;
    uint16_t mtu = 0;
    uint32_t height = 0;
    uint32_t width = 0;

    std::string toString() {
        std::string str;
        str.append("ID: ").append(std::to_string(id)).append("\n");
        str.append("Fragments: ").append(std::to_string(fragments)).append("\n");
        str.append("Length: ").append(std::to_string(length)).append("\n");
        str.append("Height: ").append(std::to_string(height)).append("\n");
        str.append("Width: ").append(std::to_string(width)).append("\n");
        return str;
    }

    bool isValid() {
        return (id < fragments);
    }

    bool isBreak() {
        return id == fragments;
    }
};

#endif //VIDEO_STREAMING_UDP_HEADERS_HPP
