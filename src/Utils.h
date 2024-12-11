#pragma once

#include "Types.h"

std::ostream& operator<<(std::ostream& out, const PcapHeader& header);
std::ostream& operator<<(std::ostream& out, const PacketRecord& record);
