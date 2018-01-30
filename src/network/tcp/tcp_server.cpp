//
// Created by sfaxi19 on 04.01.18.
//
#include <thread>
#include "../network.hpp"
#include "../../frame_transform.hpp"

static uint8_t *frame = nullptr;
static bool viewFrame = false;
static int height = 0;
static int width = 0;
static bool isBreak = false;

void tcpview() {
    while (!isBreak) {
        if ((frame == nullptr) || (!viewFrame)) continue;
        //printf("=============>\n");
        cv::Mat matFrame = cv::Mat(height, width, CV_8UC3, frame).clone();
        showFrame("receive", matFrame);
        viewFrame = false;
        if (cv::waitKey(30) >= 0)
            break;
    }
}

void tcp_server(uint16_t port) {
    char rBuffer[32];
    int sockfd;
    int sockfd_data;
    socklen_t client_len;
    sockaddr_in_t current_addr{};
    sockaddr_in_t client_addr{};
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) PERROR("ERROR opening socket");
    addr_init(current_addr, port);
    if (bind(sockfd, (sockaddr_t *) &current_addr, sizeof(current_addr)) < 0) PERROR("ERROR on binding");
    listen(sockfd, 5);
    client_len = sizeof(client_addr);
    sockfd_data = accept(sockfd, (sockaddr_t *) &client_addr, (socklen_t *) &client_len);
    if (sockfd_data < 0) PERROR("ERROR on accept");
    printf("Connection complite.\n");
    //size_t data_len = strlen(msgInit);
    //printf("data lenght: %lu\n", data_len);
    //n = send(sockfd_data, msgInit, data_len, 0);
    //printf("sending %6lu bytes\n", n);
    //if (n < 0) PERROR("ERROR write msgInit to client");

    vs::tcp_header_s header;
    vs::tcp_header_s header_resp;
    std::thread view_thread(tcpview);
    int id = 0;
    while (1) {
        if (0 == recv(sockfd_data, &header, sizeof(vs::tcp_header_s), MSG_WAITALL)) {
            break;
        }
        if (!header.isValid()) {
            printf("WARNING: %s\n%s\n", header.toString().c_str(), "Header - not valid");
            break;
        }
        if (frame == nullptr) {
            frame = new uint8_t[header.data_length];
            height = header.height;
            width = header.width;
        }

        if (0 == recv(sockfd_data, frame, header.data_length, MSG_WAITALL)) {
            break;
        }
        /*cv::Mat matFrame = matFromBytes(frame, header.height, header.width).clone();
        showFrame("receive", matFrame);
        if (cv::waitKey(30) >= 0)
            break;*/
        viewFrame = true;
        printf("%4d\n", id++);
        //header_resp.type = vs::Types::ACK_TYPE;
        //send(sockfd_data, &header_resp, sizeof(header_resp), 0);
    };
    delete[] frame;
    isBreak = true;
    close(sockfd_data);
    close(sockfd);
}
