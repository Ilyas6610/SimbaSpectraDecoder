#include "MessageParser.h"

#include <cstring>
#include <memory>

std::shared_ptr<Message> OrderUpdateMessageParser::parseMessage(char* data) const{
    OrderUpdateMsg message;
    std::memcpy(&message, data, getMessageSize());
    return std::make_shared<OrderUpdateMsg>(message);
}

uint32_t OrderUpdateMessageParser::getMessageSize() const{
    return sizeof(OrderUpdateMsg);
}

std::shared_ptr<Message> OrderUpdateExecutionMessageParser::parseMessage(char* data) const{
    OrderUpdateExecutionMsg message;
    std::memcpy(&message, data, getMessageSize());
    return std::make_shared<OrderUpdateExecutionMsg>(message);
}

uint32_t OrderUpdateExecutionMessageParser::getMessageSize() const{
    return sizeof(OrderUpdateExecutionMsg);
}

std::shared_ptr<Message> OrderBookSnapshotParser::parseMessage(char* data) const{
    OrderBookSnapshot message;
    std::memcpy(&message.rootBlock, data, sizeof(OrderBookSnapshotRootBlock));
    data += sizeof(OrderBookSnapshotRootBlock);
    std::memcpy(&message.noMDEntries, data, sizeof(GroupSize));
    data += sizeof(GroupSize);
    for(int i = 0; i < message.noMDEntries.numInGroup; ++i){
        OrderBookRepeatingSection orderBook;
        std::memcpy(&orderBook, data, sizeof(OrderBookRepeatingSection));
        data += sizeof(OrderBookRepeatingSection);
        message.orderBookRepeatingSections.push_back(std::move(orderBook));
    }
    return std::make_shared<OrderBookSnapshot>(message);
}

uint32_t OrderBookSnapshotParser::getMessageSize() const{
    return sizeof(OrderBookSnapshotRootBlock) + sizeof(OrderBookRepeatingSection)*orderBookSize;
}

std::unique_ptr<MessageParser> getMessageParser(uint16_t msgId){
    switch(msgId){
        case 15: 
            return std::make_unique<OrderUpdateMessageParser>();
        case 16: 
            return std::make_unique<OrderUpdateExecutionMessageParser>();
        case 17:
            return std::make_unique<OrderBookSnapshotParser>();
    }
    return nullptr;
}