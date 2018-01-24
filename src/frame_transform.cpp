//
// Created by sfaxi19 on 06.01.18.
//
#include <src/bmp_lib/bmp_headers.hpp>
//#include <opencv2/core/mat.hpp>
#include <iostream>
//#include <opencv2/videoio.hpp>
//#include <opencv/cv.hpp>
#include "frame_transform.hpp"
#include "network/network.hpp"

// Using matFromBytes().clone() !!!
/*
cv::Mat matFromBytes(uint8_t *data, int h, int w) {
    if (data == NULL) ERROR("Null ptr");
    cv::Mat mat = cv::Mat(h, w, CV_8UC3, data);
    return mat;
}

void showFrame(const char *title, uint8_t *data, int h, int w) {
    if (data == NULL) ERROR("Null ptr");
    cv::Mat matFrame = matFromBytes(data, h, w).clone();
    cv::imshow(title, matFrame);
}


void showFrame(const char *title, cv::Mat &mat) {
    cv::imshow(title, mat);
}


void showFrame(const char *title, TRIPLERGB **frame, int h, int w) {
    uint8_t *data = from2d_to_rev_1d_malloc(frame, h, w);
    if (data == NULL) ERROR("Null ptr");
    cv::Mat matFrame = cv::Mat(h, w, CV_8UC3, data).clone();
    cv::imshow(title, matFrame);
}

uint8_t *readFrame(cv::VideoCapture &stream, int &h, int &w) {
    cv::Mat matFrame;
    stream.read(matFrame);
    size_t size = matFrame.total() * matFrame.elemSize();
    uint8_t *bytes = new uint8_t[size];
    h = matFrame.size().height;
    w = matFrame.size().width;
    std::memcpy(bytes, matFrame.data, size);
    return bytes;
}
*/
void testOpenCV(int deviceID) {
    /*AVIMaker avi("../avi-maker/resources/lr1_1.AVI");
    TRIPLERGB **frame = avi.video()->getFrame(0);
    int h = avi.video()->height();
    int w = avi.video()->width();
    uint8_t *data = from2d_to_rev_1d_malloc(frame, h, w);
    if (data == NULL) ERROR("Null ptr");
    while (true) {
        cv::Mat cameraFrame(h, w, CV_8UC3, data);
        cv::imshow("cam", cameraFrame);
        if (cv::waitKey(30) >= 0)
            break;
    }
    cv::VideoCapture stream1(deviceID);

    if (!stream1.isOpened()) { //check if video device has been initialised
        std::cout << "cannot open camera";
    }
    cv::Mat cameraFrame;
    stream1.read(cameraFrame);
    while (true) {
        stream1.read(cameraFrame);
        cv::imshow("cam", cameraFrame);
        if (cv::waitKey(30) >= 0)
            break;
    }*/
}
