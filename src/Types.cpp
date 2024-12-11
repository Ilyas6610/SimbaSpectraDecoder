#include "Types.h"

#include <sstream>

std::string IPHeader::srcAddrAsString() const{
    char buffer[16];
    auto src = htonl(srcAddr);
    std::sprintf(buffer, "%u.%u.%u.%u",
                 static_cast<uint8_t>(src >> 24),
                 static_cast<uint8_t>(src >> 16),
                 static_cast<uint8_t>(src >> 8),
                 static_cast<uint8_t>(src));
    return buffer;
}

std::string IPHeader::dstAddrAsString() const{
    char buffer[16];
    auto dst = htonl(dstAddr);
    std::sprintf(buffer, "%u.%u.%u.%u",
                 static_cast<uint8_t>(dst >> 24),
                 static_cast<uint8_t>(dst >> 16),
                 static_cast<uint8_t>(dst >> 8),
                 static_cast<uint8_t>(dst));
    return buffer;
}

std::string OrderUpdateMsg::getMessageAsString() const{
    char buffer[512];
    sprintf(buffer,
        "{ \"mdEntryID\": %ld, "
        " \"mdEntryPX\": %ld, "
        " \"mdEntrySize\": %ld, "
        " \"mdFlags\": %lu, "
        " \"securityId\": %d, "
        " \"rptSeq\": %u, "
        " \"mdUpdateAction\": %hhu, "
        " \"mdEntryType\": \"%c\" }",
        mdEntryID,
        mdEntryPx,
        mdEntrySize,
        mdFlags,
        securityId,
        rptSeq,
        mdUpdateAction,
        mdEntryType
    );
    return buffer;
}

std::string OrderUpdateExecutionMsg::getMessageAsString() const{
    char buffer[512];
    sprintf(buffer,
        "{ \"mdEntryID\": %ld, "
        " \"mdEntryPX\": %ld, "
        " \"mdEntrySize\": %ld, "
        " \"lastPx\": %ld, "
        " \"lastQty\": %ld, "
        " \"mdFlags\": %lu, "
        " \"securityId\": %d, "
        " \"rptSeq\": %u, "
        " \"mdUpdateAction\": %hhu, "
        " \"mdEntryType\": \"%c\" }",
        mdEntryID,
        mdEntryPx,
        mdEntrySize,
        lastPx,
        lastQty,
        mdFlags,
        securityId,
        rptSeq,
        mdUpdateAction,
        mdEntryType
    );
    return buffer;
}

std::string OrderBookRepeatingSection::asString() const{
    char buffer[512];
    sprintf(buffer,
        "{ \"mdEntryID\": %ld, "
        " \"transactTime\": %lu, "
        " \"mdEntryPX\": %ld, "
        " \"mdEntrySize\": %ld, "
        " \"tradeId\": %ld, "
        " \"mdFlags\": %lu, "
        " \"mdFlags2\": %lu, "
        " \"mdEntryType\": \"%c\" }",
        mdEntryID,
        transactTime,
        mdEntryPx,
        mdEntrySize,
        tradeId,
        mdFlags,
        mdFlags2,
        mdEntryType
    );
    return buffer;
}

std::string OrderBookSnapshot::getMessageAsString() const{
    std::ostringstream ostr;
    char buffer[512];
    sprintf(buffer,
        "{ \"securityId\": %d, "
        " \"lastMsgSeqNumProcessed\": %u, "
        " \"rptSeq\": %u, "
        " \"exchangeTradingSessionId\": %u, "
        " \"noMDEntries.blockLength\": %hu, "
        " \"noMDEntries.numInGroup\": %hhu, "
        " \"repeatingSection\": [ ",
        rootBlock.securityId,
        rootBlock.lastMsgSeqNumProcessed,
        rootBlock.rptSeq,
        rootBlock.exchangeTradingSessionId,
        noMDEntries.blockLength,
        noMDEntries.numInGroup
    );
    ostr << buffer;
    for(int i = 0; i < orderBookRepeatingSections.size(); ++i){
        ostr << orderBookRepeatingSections[i].asString();
        if(i != orderBookRepeatingSections.size()-1)
            ostr << ", ";
    }
    ostr << " ] }";
    return ostr.str();
}