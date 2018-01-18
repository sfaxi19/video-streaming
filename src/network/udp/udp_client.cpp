//
// Created by sfaxi19 on 09.01.18.
//
#include <opencv2/videoio.hpp>
#include "../network.hpp"
#include "../../frame_transform.hpp"
#include "udp_headers.hpp"

void sendBreak(int sockfd, sockaddr_in_t &remote_addr, socklen_t &addr_size) {
    udp_fragment fragment;
    fragment.length = 0;
    fragment.id = fragment.fragments;
    int res = (int) sendto(sockfd, &fragment, sizeof(udp_fragment), 0, (sockaddr_t *) &remote_addr, addr_size);
    if (res == -1) PERROR("Send error:");
}

void udp_client(const char *host, uint16_t port, uint16_t deviceID) {
    int sockfd;
    sockaddr_in_t remote_addr{};
    sockaddr_in_t current_addr{};
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) PERROR("ERROR opening socket");
    socklen_t addr_size = sizeof(remote_addr);
    ssize_t len = 0;
    addr_init(remote_addr, host, port);
    addr_init(current_addr, 0);
    if (bind(sockfd, (sockaddr_t *) &current_addr, addr_size) < 0) PERROR("ERROR on binding");
    printf("DeviceID: %d\n", deviceID);
    cv::VideoCapture stream(deviceID);
    if (!stream.isOpened()) ERROR("Device index %d is not correct", deviceID);
    int h, w;
    uint16_t mtu = 50000;
    vs::packet_info_s packet_info;
    uint32_t frameID = 0;
    do {
        //=============================================================
        //                       Считывание кадра
        //=============================================================
        uint8_t *frame = readFrame(stream, h, w);
        //=============================================================
        //                        Отправка кадра
        //=============================================================
        uint32_t length = h * w * 3;
        uint16_t n_fragments = (uint16_t) ceil((double) length / mtu);
        udp_fragment fragment;
        fragment.frameID = frameID;
        fragment.height = h;
        fragment.width = w;
        fragment.mtu = mtu;
        for (int repeat = 0; repeat < 10; repeat++) {
            for (uint16_t i = 0; i < n_fragments; i++) {
                fragment.id = i;
                fragment.fragments = n_fragments;
                fragment.length = (i != n_fragments - 1) ? mtu : length % mtu;
                packet_info.length = sizeof(udp_fragment) + fragment.length;
                packet_info.packet = new uint8_t[packet_info.length];
                memcpy(packet_info.packet, &fragment, sizeof(udp_fragment));
                memcpy(packet_info.packet + sizeof(udp_fragment), frame + i * mtu, fragment.length);
                int res = (int) sendto(sockfd, packet_info.packet, packet_info.length, 0,
                                       (sockaddr_t *) &remote_addr, addr_size);
                if (res == -1) PERROR("Send error:");
                packet_info.clear();
            }
        }
        delete[]frame;
        frameID++;
    } while (1);
    close(sockfd);
}