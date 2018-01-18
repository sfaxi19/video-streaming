//
// Created by sfaxi19 on 10.01.18.
//

#ifndef VIDEO_STREAMING_UDP_HEADERS_HPP
#define VIDEO_STREAMING_UDP_HEADERS_HPP

#include "iostream"

struct udp_fragment {
    uint32_t frameID;
    uint16_t id;
    uint16_t fragments;
    uint16_t length;
    uint16_t mtu;
    uint32_t height;
    uint32_t width;

    std::string toString() {
        std::string str = "\n";
        str.append("FrameID:    ").append(std::to_string(frameID)).append("\n");
        str.append("FragmentID: ").append(std::to_string(id)).append("\n");
        str.append("Fragments:  ").append(std::to_string(fragments)).append("\n");
        str.append("Length:     ").append(std::to_string(length)).append("\n");
        str.append("Height:     ").append(std::to_string(height)).append("\n");
        str.append("Width:      ").append(std::to_string(width)).append("\n");
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
