#pragma once

#include "Types.h"

class MessageParser{
public:
    virtual std::shared_ptr<Message> parseMessage(char* data) const = 0;
    virtual uint32_t getMessageSize() const = 0;
};

class OrderUpdateMessageParser: public MessageParser{
public: 
    std::shared_ptr<Message> parseMessage(char* data) const override;
    uint32_t getMessageSize() const override;
};

class OrderUpdateExecutionMessageParser: public MessageParser{
public: 
    std::shared_ptr<Message> parseMessage(char* data) const override;
    uint32_t getMessageSize() const override;
};

class OrderBookSnapshotParser: public MessageParser{
public: 
    std::shared_ptr<Message> parseMessage(char* data) const override;
    uint32_t getMessageSize() const override;

private:
    uint32_t orderBookSize = 0;
};

std::unique_ptr<MessageParser> getMessageParser(uint16_t msgId);