cmake_minimum_required(VERSION 3.28)

project(
    SimbaDecoder
    LANGUAGES CXX)

add_executable(SimbaDecoder main.cpp MessageParser.cpp PcapFileParser.cpp SimbaPacketDecoder.cpp Types.cpp Utils.cpp)