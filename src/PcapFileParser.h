#pragma once

#include "IPacketDecoder.h"
#include "Types.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

template<typename T>
class PcapFile{
public:
    PcapFile(const char* path, std::unique_ptr<IPacketDecoder<T>> decoder);
    PcapHeader getHeader() const;
    std::optional<T> getNextRecord();
    bool eof(){
        return ifs.eof();
    }

private:
    void parseHeader();
    std::optional<T> parseNextPacket();

private:
    PcapHeader header;
    std::ifstream ifs;
    std::unique_ptr<IPacketDecoder<T>> packetDecoder;
};
