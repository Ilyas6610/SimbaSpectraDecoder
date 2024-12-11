#pragma once

#include <iostream>
#include <limits>
#include <memory>
#include <netinet/in.h>
#include <stdint.h>
#include <vector>


enum class MagicNumber: uint32_t{
    SecondsMicroseconds = 0xA1B2C3D4,
    SecondsNanoseconds = 0xA1B23C4D
}__attribute__ ((packed));

struct PcapHeader{
    MagicNumber magicNumber;
    uint16_t majorVersion;
    uint16_t minorVersion;
    uint32_t reservedField1;
    uint32_t reservedField2;
    uint32_t snapLen;
    uint16_t fcs;
    int16_t linkType;
}__attribute__ ((packed));

struct PacketRecord{
    uint32_t timeStampSeconds = 0;
    uint32_t timeStamp = 0;
    uint32_t packedLength = 0;
    uint32_t originalPackedLength = 0;
    std::shared_ptr<char[]> packedData = nullptr;
}__attribute__ ((packed));

struct EtherHeader {
    uint8_t  dstAddr[6];
    uint8_t  srcAddr[6];
    uint16_t llcLen;
}__attribute__ ((packed));

struct IPHeader {
    uint8_t  verIhl; 
    uint8_t  tos;
    uint16_t totalLength;
    uint16_t id;
    uint16_t flagsFo;
    uint8_t  ttl;
    uint8_t  protocol;
    uint16_t checksum;
    uint32_t srcAddr;
    uint32_t dstAddr;

    std::string srcAddrAsString() const;
    std::string dstAddrAsString() const;
}__attribute__ ((packed));

class UDPHeader {
public:
    uint16_t srcPort;
    uint16_t dstPort;
    uint16_t length;
    uint16_t checksum;
}__attribute__ ((packed));

enum class mdpHeaderFlags: uint16_t{
    LastFragment = 0x1,
    StartOfSnapshot = 0x2,
    EndOfSnapshot = 0x4,
    IncrementalPacket = 0x8,
    PosDupFlag = 0x10
}__attribute__ ((packed));

struct MarketDataPacketHeader{
    uint32_t msgSeqNum;
    uint16_t msgSize;
    uint16_t msgFlags;
    uint64_t sendingTime;
}__attribute__ ((packed));

struct IncrementalPacketHeader{
    uint64_t transactTime;
    uint32_t sessionId;
}__attribute__ ((packed));

struct SBEHeader{
    uint16_t blockLength;
    uint16_t templateId;
    uint16_t schemaId;
    uint16_t version;
}__attribute__ ((packed));

using Decimal5Mantissa = int64_t;

struct Message{
    virtual std::string getMessageAsString() const = 0;
    virtual ~Message() = default;
};

struct OrderUpdateMsg: Message{
    int64_t mdEntryID;
    Decimal5Mantissa mdEntryPx;
    int64_t mdEntrySize;
    uint64_t mdFlags;
    int32_t securityId;
    uint32_t rptSeq;
    uint8_t mdUpdateAction;
    char mdEntryType;

    std::string getMessageAsString() const override;
}__attribute__ ((packed));

constexpr uint64_t NullValue = std::numeric_limits<uint64_t>::max();

struct OrderUpdateExecutionMsg: Message{
    int64_t mdEntryID;
    Decimal5Mantissa mdEntryPx;
    int64_t mdEntrySize;
    int64_t lastPx;
    int64_t lastQty;
    uint64_t mdFlags;
    int32_t securityId;
    uint32_t rptSeq;
    uint8_t mdUpdateAction;
    char mdEntryType;
    
    std::string getMessageAsString() const override;
}__attribute__ ((packed));

struct GroupSize{
    uint16_t blockLength;
    uint8_t numInGroup;
}__attribute__ ((packed));

struct OrderBookRepeatingSection{
    int64_t mdEntryID;
    uint64_t transactTime;
    Decimal5Mantissa mdEntryPx;
    int64_t mdEntrySize;
    int64_t tradeId;
    uint64_t mdFlags;
    uint64_t mdFlags2;
    char mdEntryType;

    std::string asString() const;
}__attribute__ ((packed));

struct OrderBookSnapshotRootBlock{
    int32_t securityId;
    uint32_t lastMsgSeqNumProcessed;
    uint32_t rptSeq;
    uint32_t exchangeTradingSessionId;
}__attribute__ ((packed));

struct OrderBookSnapshot: Message{
    OrderBookSnapshotRootBlock rootBlock;
    GroupSize noMDEntries;
    std::vector<OrderBookRepeatingSection> orderBookRepeatingSections;

    std::string getMessageAsString() const override;
}__attribute__ ((packed));
