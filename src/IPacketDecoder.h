#pragma once
#include "Types.h"

#include <optional>

template<typename T>
class IPacketDecoder{
public:
    virtual std::optional<T> parsePacket(PacketRecord& record) = 0;
};