#include "NetworkSerializer.h"
#include <pb_encode.h>

NetworkSerializer::NetworkSerializer(INetworkOutputStream& stream, INetworkManager& manager, ILogger& logger) :
    m_outputStream(stream), m_networkManager(manager), m_hivemindHostSerializer(stream), m_logger(logger) {}

bool NetworkSerializer::serializeToStream(const MessageDTO& message) {
    Message msg;
    message.serialize(msg);

    auto ip = m_networkManager.getIPFromAgentID(message.getDestinationId());
    if (!ip.has_value()) {
        m_logger.log(LogLevel::Error, "Could not get IP from destination");
        return false;
    }

    if (!m_outputStream.setDestination(ip.value())) {
        m_logger.log(LogLevel::Error, "Could not set destination from IP");
        return false;
    }

    bool ret = m_hivemindHostSerializer.serializeToStream(message);
    m_outputStream.close();
    return ret;
}