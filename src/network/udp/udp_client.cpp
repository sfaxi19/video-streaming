//
// Created by sfaxi19 on 09.01.18.
//
//#include <opencv2/videoio.hpp>
#include <src/motion_compensation/motion_compensation.h>
#include "../network.hpp"
#include "../../frame_transform.hpp"
#include "udp_headers.hpp"
#include "cmath"
#include "src/AVIMaker/AVIMaker.h"
#include <chrono>
#include <thread>

void sendBreak(int sockfd, sockaddr_in_t &remote_addr, socklen_t &addr_size) {
    udp_fragment fragment;
    fragment.length = 0;
    fragment.id = fragment.fragments;
    int res = (int) sendto(sockfd, &fragment, sizeof(udp_fragment), 0, (sockaddr_t *) &remote_addr, addr_size);
    if (res == -1) PERROR("Send error:");
}

void udp_client(const char *host, uint16_t port, uint16_t deviceID, uint16_t mtu, bool onlyMotion) {
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
    //cv::VideoCapture stream(deviceID);
    //if (!stream.isOpened()) ERROR("Device index %d is not correct", deviceID);
    int h, w;
    vs::packet_info_s packet_info;
    uint32_t frameID = 0;
    uint8_t *base = nullptr;
    uint32_t sum = 0;
    AVIMaker avi_file("../avi-maker/resources/video.AVI");    
    std::cout << "Press SPACE" << std::endl;
    std::cin.get();
    do {
        //=============================================================
        //                       Считывание кадра
        //=============================================================
        //uint8_t *frame = readFrame(stream, h, w);
	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	uint8_t *frame = readVideoFrame(avi_file,h, w);
        if (frame == nullptr) break;
	if (onlyMotion) {
            //TRIPLERGB *target = (TRIPLERGB *) frame;
            //if ((base != nullptr) && (target != nullptr))
                //sum = mc::sumAbsDiffFrame((TRIPLERGB *) base, target, h, w);
        }
        //=============================================================
        //                        Отправка кадра
        //=============================================================
        if ((sum > 600000) || (!onlyMotion)) {
            //printf("sum: %d =========>\n", sum);
            uint32_t length = h * w * 3;
            uint16_t n_fragments = (uint16_t) ceil((double) length / mtu);
            udp_fragment fragment;
            fragment.frameID = frameID;
            fragment.height = h;
            fragment.width = w;
            fragment.mtu = mtu;
	    fragment.fragments = n_fragments;
            for (int repeat = 0; repeat < 1; repeat++) {
                for (uint16_t i = 0; i < n_fragments; i++) {
                    fragment.id = i;
                    //fragment.fragments = n_fragments;
                    fragment.length = (i != n_fragments - 1) ? mtu : length % mtu;
                    packet_info.length = sizeof(udp_fragment) + fragment.length;
                    if (packet_info.packet == nullptr) packet_info.packet = new uint8_t[packet_info.length];
                    memcpy(packet_info.packet, &fragment, sizeof(udp_fragment));
                    memcpy(packet_info.packet + sizeof(udp_fragment), frame + i * mtu, fragment.length);
                    int res = (int) sendto(sockfd, packet_info.packet, packet_info.length, 0,
                                           (sockaddr_t *) &remote_addr, addr_size);
                    if (res == -1) PERROR("Send error:");
                    //packet_info.clear();
                }
            }
        }
        //if (base != nullptr) delete[] base;
        base = frame;
        frameID++;
    } while (1);
    close(sockfd);
}
