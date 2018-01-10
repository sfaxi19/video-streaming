//
// Created by sfaxi19 on 09.01.18.
//

#include "../network.hpp"
#include "../../frame_transform.hpp"
#include "udp_headers.hpp"

void udp_server(uint16_t port) {
    int sockfd;
    uint8_t buffer[65500];
    socklen_t addr_size;
    sockaddr_in_t current_addr{};
    sockaddr_in_t remote_addr{};
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) PERROR("ERROR opening socket");
    addr_init(current_addr, port);
    if (bind(sockfd, (sockaddr_t *) &current_addr, sizeof(current_addr)) < 0) PERROR("ERROR on binding");
    addr_size = sizeof(remote_addr);
    udp_fragment fragment;
    ssize_t len = 0;
    uint8_t *frame = nullptr;
    uint32_t height = 0;
    uint32_t width = 0;
    while (1) {
        do {
            len = recvfrom(sockfd, &buffer, sizeof(buffer), 0, (sockaddr_t *) &remote_addr, &addr_size);
            if (len == 16) break;
            //=============================================================
            //                        Получение заголовка
            //=============================================================
            memcpy(&fragment, &buffer, sizeof(udp_fragment));
            if (len == 0) break;
            if (fragment.isValid()) {
                //printf("\nReceive header:\n%s", fragment.toString().c_str());
            } else if (fragment.isBreak()) {
                printf("\nReceive break header:\n%s", fragment.toString().c_str());
                break;
            } else {
                fprintf(stderr, "WARNING: %s\n%s\n", fragment.toString().c_str(), "Header - not valid");
                break;
            }
            if (frame == nullptr) {
                height = fragment.height;
                width = fragment.width;
                frame = new uint8_t[fragment.height * fragment.width * 3];
                bzero(frame, height * width * 3);
            }
            //=============================================================
            //                     Считывание фрагмента
            //=============================================================
            memcpy(&frame[fragment.id * fragment.mtu], &buffer[sizeof(udp_fragment)], fragment.length);
        } while (1);
        //=============================================================
        //                        Отображение кадра
        //=============================================================
        cv::Mat matFrame = cv::Mat(height, width, CV_8UC3, frame).clone();
        showFrame("receive", matFrame);
        if (cv::waitKey(30) >= 0)
            break;
    };
    delete[] frame;
    close(sockfd);
}