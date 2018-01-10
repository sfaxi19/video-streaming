//
// Created by sfaxi19 on 04.01.18.
//
#include "../network.hpp"
#include "../../frame_transform.hpp"

void tcp_server(uint16_t port) {
    char rBuffer[32];
    int sockfd;
    int sockfd_data;
    socklen_t client_len;
    sockaddr_in_t current_addr{};
    sockaddr_in_t client_addr{};
    ssize_t n;
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
    ssize_t len = 0;
    while (1) {
        //len = read(sockfd_data, &header, sizeof(vs::header_s));
        len = recv(sockfd_data, &header, sizeof(vs::tcp_header_s), 0);
        printf("\t\t\t\treceive %lu bytes.\n", len);
        if (len == 0) break;

        if (header.isValid()) printf("\nReceive header:\n%s", header.toString().c_str());
        else {
            //ERROR("%s\n%s\n", header.toString().c_str(), "Header - not valid");
            printf("WARNING: %s\n%s\n", header.toString().c_str(), "Header - not valid");
            break;
        }
        uint8_t *frame = new uint8_t[header.data_length];

        //len = read(sockfd_data, frame, header.data_length);
        len = recv(sockfd_data, frame, header.data_length, MSG_WAITALL);
        if (len == 0) break;
        printf("\t\t\t\treceive %lu bytes\n", len);
        for (int i = 0; i < 40; i++) {
            printf("%d ", frame[i]);
        }
        printf("\n");
        cv::Mat matFrame = matFromBytes(frame, header.height, header.width).clone();
        showFrame("receive", matFrame);
        if (cv::waitKey(30) >= 0)
            break;
        delete[] frame;
        header_resp.type = vs::Types::ACK_TYPE;
        send(sockfd_data, &header_resp, sizeof(header_resp), 0);
    };
    close(sockfd_data);
    close(sockfd);
}
