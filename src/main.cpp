#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv/cv.hpp>
#include <src/AVIMaker/AVIMaker.h>
#include <bits/unique_ptr.h>
//#include "../avi-maker/src/AVIMaker/AVIMaker.h"
#include "network/network.hpp"
#include "frame_transform.hpp"

/*
 *
 */



//std::vector<BITMAPINFOHEADER **> frames;

int main(int argn, char *argv[]) {
    //frames
    // ./prog
    /*int h, w;
    cv::VideoCapture stream(1);
    while (1) {
        uint8_t *frame = readFrame(stream,h, w);
        showFrame("test", frame, h, w);
        if (cv::waitKey(30) >= 0)
            break;
        delete[] frame;
    }*/
    int in_argn = argn - 1;
    printf("Input arguments: %d\n", in_argn);
    if (in_argn < 2) ERROR("Bad input args");
    std::string param = argv[1];
    printf("param: %s\n", param.c_str());
    if (param == "-s") {
        if (in_argn != 2) ERROR("Bad input args for -s param");
        tcp_server((uint16_t) atoi(argv[2]));
    } else if (param == "-c") {
        if (in_argn != 4) ERROR("Bad input args for -c param:\n[2] - host\n[3] - port\n[4] - deviceID\n");
        tcp_client(argv[2], (uint16_t) atoi(argv[3]), (uint16_t) atoi(argv[4]));
    } else {
        ERROR("Bad input args");
    }
    return 0;
}
