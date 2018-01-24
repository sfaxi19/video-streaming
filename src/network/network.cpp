//
// Created by sfaxi19 on 04.01.18.
//
#include <src/AVIMaker/AVIMaker.h>
#include "network.hpp"

void addr_init(sockaddr_in_t &addr, uint16_t port) {
    bzero((char *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
}

void addr_init(sockaddr_in_t &addr, const char *host_addr, uint16_t port) {
    bzero((char *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    struct hostent *host = gethostbyname(host_addr);
    bcopy((char *) host->h_addr,
          (char *) &addr.sin_addr.s_addr,
          static_cast<size_t>(host->h_length));
    addr.sin_port = htons(port);
}

namespace vs {
    const char *toString(uint16_t type) {
        switch (type) {
            case vs::Types::TCP_RGBFRAME_TYPE:
                return "Frame";
            case vs::Types::HEADERS_TYPE:
                return "Headers";
            case vs::Types::UNKNOWN_TYPE:
                return "Unknown";
            default:
                return "Invalid type";
        }
    }

}

TRIPLERGB **from1d_to_2d(uint8_t *in, int h, int w) {
    TRIPLERGB **rgb_arr = new TRIPLERGB *[h];
    int id = 0;
    for (int i = 0; i < h; i++) {
        rgb_arr[i] = new TRIPLERGB[w];
        for (int j = 0; j < w; j++) {
            rgb_arr[i][j].blue = in[id++];
            rgb_arr[i][j].green = in[id++];
            rgb_arr[i][j].red = in[id++];
        }
    }
    return rgb_arr;
}

/*
 * The out variable must point to the allocated memory
 */
void from2d_to_1d_NO_malloc(TRIPLERGB **mrx, int h, int w, uint8_t *out) {
    int id = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            out[id++] = mrx[i][j].blue;
            out[id++] = mrx[i][j].green;
            out[id++] = mrx[i][j].red;
        }
    }
}


uint8_t *readVideoFrame(AVIMaker &avi_file, int &h, int &w) {
    static uint32_t frameID = 0;
    static VideoStream *video = avi_file.video();
    TRIPLERGB **frame = video->getFrame(frameID);
    if (frame == nullptr) return nullptr;
    h = video->height();
    w = video->width();
    frameID++;
    return from2d_to_rev_1d_malloc(frame, h, w);
}

uint8_t *from2d_to_1d_malloc(TRIPLERGB **mrx, int h, int w) {
    uint8_t *out = new uint8_t[h * w * 3];
    int id = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            out[id++] = mrx[i][j].blue;
            out[id++] = mrx[i][j].green;
            out[id++] = mrx[i][j].red;
        }
    }
    return out;
}

uint8_t *from2d_to_rev_1d_malloc(TRIPLERGB **mrx, int h, int w) {
    uint8_t *out = new uint8_t[h * w * 3];
    int id = 0;
    for (int i = h - 1; i >= 0; i--) {
        for (int j = 0; j < w; j++) {
            out[id++] = mrx[i][j].blue;
            out[id++] = mrx[i][j].green;
            out[id++] = mrx[i][j].red;
        }
    }
    return out;
}
