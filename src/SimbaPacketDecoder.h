#pragma once

#include "IPacketDecoder.h"
#include "Types.h"

#include <cstring>
#include <iostream>
#include <memory>
#include <optional>

struct SimbaPacket{
    EtherHeader etherHeader;
    IPHeader ipHeader;
    UDPHeader udpHeader;
    MarketDataPacketHeader mdpHeader;
    std::optional<IncrementalPacketHeader> incrementalPacketHeaderOpt;
    std::vector<std::pair<SBEHeader, std::shared_ptr<Message>>> messages;
};

class SimbaPacketDecoder: public IPacketDecoder<SimbaPacket>{
public:
    std::optional<SimbaPacket> parsePacket(PacketRecord& record) override;

private:
    template<typename T>
    char* parseStruct(PacketRecord& record, char* data, T& instance){
        constexpr auto headerSize = sizeof(T);
        checkSize(record, data, headerSize);
        std::memcpy(&instance, data, headerSize);
        return data + headerSize;
    }
    
    void checkSize(PacketRecord& record, char* data, unsigned long size);
    char* parseMessage(SimbaPacket& packet, SBEHeader& header, char* packetData);
};