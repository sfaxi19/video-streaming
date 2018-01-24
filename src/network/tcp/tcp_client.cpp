//
// Created by sfaxi19 on 04.01.18.
//

#include <netdb.h>
#include <vector>
//#include <opencv2/videoio.hpp>
#include <src/motion_compensation/motion_compensation.h>
#include "../network.hpp"
//#include "../avi-maker/src/AVIMaker/AVIMaker.h"
#include "src/AVIMaker/AVIMaker.h"
#include "../../frame_transform.hpp"
#include <chrono>
#include <thread>

std::vector<TRIPLERGB **> frames;


void tcp_packet_maker(vs::packet_info_s &packet_info, TRIPLERGB **mrx, size_t h, size_t w, vs::tcp_header_s &header) {
    fill_headers(packet_info, header, h, w);
    fill_data(packet_info, header, mrx);
}

void tcp_packet_maker(vs::packet_info_s &packet_info, uint8_t *bytes, size_t h, size_t w, vs::tcp_header_s &header) {
    fill_headers(packet_info, header, h, w);
    fill_data(packet_info, header, bytes);
}

void fill_headers(vs::packet_info_s &packet_info, vs::tcp_header_s &header, size_t h, size_t w) {
    header.data_length = h * w * 3;
    header.height = h;
    header.width = w;
    packet_info.length = sizeof(vs::tcp_header_s) + header.data_length;
    if (packet_info.packet == nullptr) packet_info.packet = new uint8_t[packet_info.length];
    memcpy(packet_info.packet, &header, sizeof(header));
}

void fill_data(vs::packet_info_s &packet_info, vs::tcp_header_s &header, TRIPLERGB **mrx) {
    from2d_to_1d_NO_malloc(mrx, header.height, header.width, (packet_info.packet + sizeof(vs::tcp_header_s)));
}

void fill_data(vs::packet_info_s &packet_info, vs::tcp_header_s &header, uint8_t *bytes) {
    memcpy(packet_info.packet + sizeof(vs::tcp_header_s), bytes, header.data_length);
}

void tcp_client(const char *hostname, uint16_t port, uint16_t deviceID, bool onlyMotion) {
    int sockfd;
    sockaddr_in_t addr{};
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) PERROR("ERROR opening socket");
    addr_init(addr, hostname, port);
    ssize_t len = 0;
    printf("DeviceID: %d\n", deviceID);
    uint32_t sum = 0;
    uint8_t *base = nullptr;
    AVIMaker avi_file("../avi-maker/resources/video.AVI");
    std::cout << "Press ENTER" << std::endl;
    std::cin.get();
    if (connect(sockfd, (sockaddr_t *) &addr, sizeof(addr)) != 0) PERROR("ERROR on connect");
    printf("Connection complite.\n");
    vs::packet_info_s packet_info;
    vs::tcp_header_s header;
    int id = 0;
    do {
        //=============================================================
        //                       Считывание кадра
        //=============================================================
        int h, w;
        //uint8_t *frame = readFrame(stream, h, w);
	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	uint8_t *frame = readVideoFrame(avi_file,h, w);
	if (frame == nullptr) break;
        //=============================================================
        //                        Отправка кадра
        //=============================================================
        header.type = vs::Types::TCP_RGBFRAME_TYPE;
        //tcp_packet_maker(packet_info, frame, h, w, header);
        if (packet_info.packet == nullptr) fill_headers(packet_info,header,h,w);
	fill_data(packet_info,header,frame);
	write(sockfd, packet_info.packet, packet_info.length);
	printf("%4d\n",id++);
        //packet_info.clear();
        //printf("sending %d bytes\n", packet_info.length);
    } while (1);
    packet_info.clear();
    close(sockfd);
}
