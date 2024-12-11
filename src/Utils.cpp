#include "Utils.h"

#include <iostream>

std::ostream& operator<<(std::ostream& out, const PcapHeader& header){
    out << std::hex << static_cast<uint32_t>(header.magicNumber) << ' '
        << std::dec << header.majorVersion << ' '
        << header.minorVersion << ' '
        << header.reservedField1 << ' '
        << header.reservedField2 << ' '
        << header.snapLen << ' '
        << header.fcs << ' ' << header.linkType;
    return out;
}

std::ostream& operator<<(std::ostream& out, const PacketRecord& record){
    out << record.timeStampSeconds << ' '
        << record.timeStamp << ' '
        << record.packedLength << ' '
        << record.originalPackedLength << ' ';
    return out;
}