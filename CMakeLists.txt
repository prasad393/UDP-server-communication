
cmake_minimum_required(VERSION 3.10)
project(udp_project)

set(CMAKE_CXX_STANDARD 11)

# Add client
add_executable(udp_sender client/udp_sender.cpp)

# Add server
add_executable(udp_receiver server/udp_receiver.cpp)
