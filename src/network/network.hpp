//
// Created by sfaxi19 on 04.01.18.
//

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <zconf.h>
#include <cstdio>
#include <cstdlib>
#include <memory.h>
#include <netdb.h>
#include <src/bmp_lib/bmp_headers.hpp>
#include "string"

typedef struct sockaddr_in sockaddr_in_t;
typedef struct sockaddr sockaddr_t;

#define PERROR(msg) { perror(msg); exit(1);}
#define ERROR(...) { printf(__VA_ARGS__); exit(1);}

namespace vs {
    enum Types : uint16_t {
        UNKNOWN_TYPE = 0,
        RGBFRAME_TYPE = 1,
        HEADERS_TYPE = 2,
        ACK_TYPE = 3
    };

    const char *toString(uint16_t type);

    struct header_s {
        uint32_t type = Types::UNKNOWN_TYPE;
        uint32_t height = 0;
        uint32_t width = 0;
        uint32_t data_length = 0;

        std::string toString() {
            std::string str;
            str.append("Type: ").append(vs::toString(type)).append("\n");
            str.append("Size: ").append(std::to_string(data_length)).append("\n");
            return str;
        }

        bool isValid() {
            return type < 4;
        }
    };

    struct packet_info_s {
        uint8_t *packet{};
        uint32_t length = 0;

        void clear() {
            delete[] packet;
        }
    };

}


void from2d_to_1d_NO_malloc(TRIPLERGB **mrx, int h, int w, uint8_t *out);

uint8_t *from2d_to_1d_malloc(TRIPLERGB **mrx, int h, int w);

uint8_t *from2d_to_rev_1d_malloc(TRIPLERGB **mrx, int h, int w);

void tcp_server(uint16_t port);

void tcp_client(const char *ip, uint16_t port, uint16_t deviceID);

void addr_init(sockaddr_in_t &addr, uint16_t port);

void addr_init(sockaddr_in_t &addr, const char *host_addr, uint16_t port);


void packet_maker(vs::packet_info_s &packet_info, TRIPLERGB **mrx, size_t h, size_t w, vs::header_s &header);

void packet_maker(vs::packet_info_s &packet_info, uint8_t *bytes, size_t h, size_t w, vs::header_s &header);

void fill_headers(vs::packet_info_s &packet_info, vs::header_s &header, size_t h, size_t w);

void fill_data(vs::packet_info_s &packet_info, vs::header_s &header, TRIPLERGB **mrx);

void fill_data(vs::packet_info_s &packet_info, vs::header_s &header, uint8_t *bytes);

#endif //NETWORK_HPP
