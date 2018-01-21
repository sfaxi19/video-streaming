#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv/cv.hpp>
#include <src/AVIMaker/AVIMaker.h>
#include <bits/unique_ptr.h>
//#include "../avi-maker/src/AVIMaker/AVIMaker.h"
#include "network/network.hpp"
#include "frame_transform.hpp"

int main(int argn, char *argv[]) {
    int in_argn = argn - 1;
    printf("Input arguments: %d\n", in_argn);
    if (in_argn < 2) {
        ERROR("Bad input args");
    }
    std::string param = argv[1];
    printf("param: %s\n", param.c_str());
    if (param == "-s-udp") {
        if (in_argn != 2) ERROR("Bad input args for -s param");
        udp_server((uint16_t) atoi(argv[2]));
        return 0;
    }
    if (param == "-s-tcp") {
        if (in_argn != 2) ERROR("Bad input args for -s param");
        tcp_server((uint16_t) atoi(argv[2]));
        return 0;
    }
    if (param == "-c-udp") {
        if (in_argn != 6) ERROR("Bad input args for -c param:\n[2] - host\n[3] - port\n[4] - deviceID\n[5] - mtu\n[6] - onlyMotion");
        udp_client(argv[2], (uint16_t) atoi(argv[3]), (uint16_t) atoi(argv[4]), (uint16_t) atoi(argv[5]), (bool) atoi(argv[6]));
        return 0;
    }
    if (param == "-c-tcp") {
        if (in_argn != 5) ERROR("Bad input args for -c param:\n[2] - host\n[3] - port\n[4] - deviceID\n[5] - onlyMotion");
        tcp_client(argv[2], (uint16_t) atoi(argv[3]), (uint16_t) atoi(argv[4]), (bool) atoi(argv[5]));
        return 0;
    }
    ERROR("Bad input args");
    return 0;
}
