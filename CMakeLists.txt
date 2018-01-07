cmake_minimum_required(VERSION 3.8)
project(video-streaming)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)


set(SOURCE_FILES
        src/main.cpp
        src/network/tcp_server.cpp
        src/network/tcp_client.cpp
        src/network/network.hpp
        src/network/network.cpp src/frame_transform.hpp src/frame_transform.cpp)

include_directories(avi-maker)

add_executable(video-streaming ${SOURCE_FILES})

add_subdirectory(avi-maker/)

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(video-streaming ${OpenCV_LIBS} avi-maker-lib)