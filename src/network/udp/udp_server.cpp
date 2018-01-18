//
// Created by sfaxi19 on 09.01.18.
//

#include "../network.hpp"
#include "../../frame_transform.hpp"
#include "udp_headers.hpp"
#include "thread"

uint8_t *frame = nullptr;
uint32_t height = 0;
uint32_t width = 0;
bool viewFrame = false;

void view() {
    while (1) {
        if ((frame == nullptr) || (!viewFrame)) continue;
        printf("=============>\n");
        cv::Mat matFrame = cv::Mat(height, width, CV_8UC3, frame).clone();
        showFrame("receive", matFrame);
        viewFrame = false;
        if (cv::waitKey(30) >= 0)
            break;
    }
}

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
    udp_fragment *fragment;
    ssize_t len = 0;
    uint32_t currentFrameID = 0;
    fragment = (udp_fragment *) buffer;
    std::thread view_thread(view);
    while (1) {
        do {
            len = recvfrom(sockfd, &buffer, sizeof(buffer), 0, (sockaddr_t *) &remote_addr, &addr_size);
            if (len < sizeof(udp_fragment)) {
                printf("Bad fragment\n");
                exit(1);
            }
            //=============================================================
            //                       Получение заголовка
            //=============================================================
            //memcpy(&fragment, &buffer, sizeof(udp_fragment));
            //printf("CurFrameID: %d\n", currentFrameID);
            //printf("Fragment: %s\n", fragment->toString().c_str());
            if (!fragment->isValid()) {
                fprintf(stderr, "WARNING: %s\n%s\n", fragment->toString().c_str(), "Header - not valid");
                break;
            }
            if (frame == nullptr) {
                currentFrameID = fragment->frameID;
                height = fragment->height;
                width = fragment->width;
                frame = new uint8_t[fragment->height * fragment->width * 3];
                bzero(frame, height * width * 3);
            }
            if (fragment->frameID > currentFrameID) {
                viewFrame = true;
                printf("%5d | %3d /%3d   ---- new frame\n", fragment->frameID, fragment->id, fragment->fragments);
                currentFrameID = fragment->frameID;
                break;
            } else if (fragment->frameID < currentFrameID) {
                printf("\t\t-->%5d | %3d /%3d\n", fragment->frameID, fragment->id, fragment->fragments);
                continue;
            } else {
                printf("%5d | %3d /%3d \t len = %d\n", fragment->frameID, fragment->id, fragment->fragments,
                       fragment->length);
            }
            //=============================================================
            //                     Считывание фрагмента
            //=============================================================
            memcpy(frame + fragment->id * fragment->mtu, &buffer[sizeof(udp_fragment)], fragment->length);
        } while (1);
        //=============================================================
        //                        Отображение кадра
        //=============================================================
    };
    delete[] frame;
    close(sockfd);
}