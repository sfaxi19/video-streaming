//
// Created by sfaxi19 on 04.01.18.
//

#include <netdb.h>
#include <vector>
#include <opencv2/videoio.hpp>
#include "../network.hpp"
//#include "../avi-maker/src/AVIMaker/AVIMaker.h"
#include "src/AVIMaker/AVIMaker.h"
#include "../../frame_transform.hpp"

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
    packet_info.packet = new uint8_t[packet_info.length];
    memcpy(packet_info.packet, &header, sizeof(header));
}

void fill_data(vs::packet_info_s &packet_info, vs::tcp_header_s &header, TRIPLERGB **mrx) {
    from2d_to_1d_NO_malloc(mrx, header.height, header.width, (packet_info.packet + sizeof(vs::tcp_header_s)));
}

void fill_data(vs::packet_info_s &packet_info, vs::tcp_header_s &header, uint8_t *bytes) {
    memcpy(packet_info.packet + sizeof(vs::tcp_header_s), bytes, header.data_length);
}

void tcp_client(const char *hostname, uint16_t port, uint16_t deviceID) {
    int sockfd;
    sockaddr_in_t addr{};
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) PERROR("ERROR opening socket");
    addr_init(addr, hostname, port);
    if (connect(sockfd, (sockaddr_t *) &addr, sizeof(addr)) != 0) PERROR("ERROR on connect");
    printf("Connection complite.\n");
    ssize_t len = 0;
    printf("DeviceID: %d\n", deviceID);
    cv::VideoCapture stream(deviceID);
    if(!stream.isOpened()) ERROR("Device index %d is not correct", deviceID);
    do {
        //=============================================================
        //                       Считывание кадра
        //=============================================================
        int h, w;
        uint8_t *frame = readFrame(stream, h, w);
        //=============================================================
        //                        Отправка кадра
        //=============================================================
        vs::packet_info_s packet_info;
        vs::tcp_header_s header;
        header.type = vs::Types::TCP_RGBFRAME_TYPE;
        tcp_packet_maker(packet_info, frame, h, w, header);
        /*for (int i = 0; i < 40; i++) {
            if (i == sizeof(vs::tcp_header_s)) printf("\t");
            printf("%d ", packet_info.packet[i]);
        }
        printf("\n");*/
        write(sockfd, packet_info.packet, packet_info.length);
        delete[] frame;
        packet_info.clear();
        //printf("sending %d bytes\n", packet_info.length);
        //=============================================================
        //                       Ожидание ответа
        //=============================================================
        len = read(sockfd, &header, sizeof(vs::tcp_header_s));
        if (len < 0) break;
        switch (header.type) {
            case vs::Types::ACK_TYPE:
                //printf("Sending complite.\n");
                break;
            default:
                //printf("Bad response.\n");
                len = 0;
                break;
        }

    } while (len != 0);
    close(sockfd);
}
