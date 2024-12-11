#pragma once

#include "MessageParser.h"
#include "SimbaPacketDecoder.h"
#include "PcapFileParser.h"
#include "Types.h"
#include "Utils.h"

#include <cstring>
#include <iostream>
#include <sstream>

std::optional<SimbaPacket> SimbaPacketDecoder::parsePacket(PacketRecord& record){
    SimbaPacket packet;
    try{
        if(record.packedLength != record.originalPackedLength)
            return std::nullopt;
        auto* packetData = record.packedData.get();
        packetData = parseStruct(record, packetData, packet.etherHeader);
        packetData = parseStruct(record, packetData, packet.ipHeader);
        packetData = parseStruct(record, packetData, packet.udpHeader);
        packetData = parseStruct(record, packetData, packet.mdpHeader);
        if(packet.mdpHeader.msgFlags & static_cast<uint16_t>(mdpHeaderFlags::IncrementalPacket)){
            packet.incrementalPacketHeaderOpt.emplace();
            packetData = parseStruct(record, packetData, *packet.incrementalPacketHeaderOpt);
        }
        while(packetData < record.packedData.get() + record.packedLength){
            SBEHeader sbeHeader;
            packetData = parseStruct(record, packetData, sbeHeader);
            packetData = parseMessage(packet, sbeHeader, packetData);
            if(!packetData)
                return std::nullopt;
        }
        
    }catch(std::runtime_error& re){
        std::cerr << re.what() << std::endl;
        return std::nullopt;
    }

    return packet;
}
    
void SimbaPacketDecoder::checkSize(PacketRecord& record, char* data, unsigned long size){
    if(data + size > record.packedData.get() + record.packedLength)
        throw std::runtime_error("Wrong data");
}

char* SimbaPacketDecoder::parseMessage(SimbaPacket& packet, SBEHeader& header, char* packetData){
    auto parser = getMessageParser(header.templateId);
    if(!parser)
        return nullptr;
    packet.messages.emplace_back(header, parser->parseMessage(packetData));
    return packetData + parser->getMessageSize();
}