//
// Created by sfaxi19 on 06.01.18.
//

#ifndef VIDEO_STREAMING_FRAME_TRANSFORM_HPP
#define VIDEO_STREAMING_FRAME_TRANSFORM_HPP

#include <opencv/cv.hpp>

cv::Mat matFromBytes(uint8_t *data, int h, int w);

void showFrame(const char *title, uint8_t *data, int h, int w);

void showFrame(const char *title, TRIPLERGB **frame, int h, int w);

void showFrame(const char *title, cv::Mat &mat);

uint8_t *readFrame(cv::VideoCapture &stream, int &h, int &w);

#endif //VIDEO_STREAMING_FRAME_TRANSFORM_HPP
