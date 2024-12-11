#include "SimbaPacketDecoder.h"
#include "PcapFileParser.h"
#include "Types.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

template class PcapFile<SimbaPacket>;

template<typename T>
PcapFile<T>::PcapFile(const char* path, std::unique_ptr<IPacketDecoder<T>> decoder): ifs(path), packetDecoder(std::move(decoder)){
    parseHeader();
}

template<typename T>
PcapHeader PcapFile<T>::getHeader() const{
    return header;
}

template<typename T>
std::optional<T> PcapFile<T>::getNextRecord(){
    if(ifs.eof())
        return std::nullopt;
    
    return parseNextPacket();
}

template<typename T>
void PcapFile<T>::parseHeader(){
    constexpr uint32_t HeaderSize = sizeof(PcapHeader);
    char buffer[HeaderSize];
    ifs.read(buffer, HeaderSize);
    std::memcpy(&header, buffer, HeaderSize);
}

template<typename T>
std::optional<T> PcapFile<T>::parseNextPacket(){
    constexpr auto PacketHeaderSize = 16;
    PacketRecord record;
    char buffer[PacketHeaderSize];
    ifs.read(buffer, PacketHeaderSize);
    std::memcpy(&record, buffer, PacketHeaderSize);
    record.packedData = std::shared_ptr<char[]>(new char[record.packedLength], std::default_delete<char[]>());
    ifs.read(record.packedData.get(), record.packedLength);
    return packetDecoder->parsePacket(record); 
}