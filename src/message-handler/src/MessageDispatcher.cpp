#include "MessageDispatcher.h"
#include "Task.h"

MessageDispatcher::MessageDispatcher(
    ICircularQueue<MessageDTO>& hivemindOutputQ,
    ICircularQueue<MessageDTO>& unicastOutputQ,
    ICircularQueue<MessageDTO>& broadcastOutputQ,
    IHiveMindHostDeserializer& deserializer,
    INetworkAPIHandler& networkApiHandler,
    IHiveConnectHiveMindApiMessageHandler& hiveConnectHiveMindApiMessageHandler,
    IBSP& bsp,
    ILogger& logger,
    INetworkManager& manager) :
    m_hivemindOutputQueue(hivemindOutputQ),
    m_unicastOutputQueue(unicastOutputQ),
    m_broadcastOutputQueue(broadcastOutputQ),
    m_deserializer(deserializer),
    m_networkApiHandler(networkApiHandler),
    m_HiveConnectHiveMindMessageHandler(hiveConnectHiveMindApiMessageHandler),
    m_bsp(bsp),
    m_logger(logger),
    m_manager(manager) {}

bool MessageDispatcher::deserializeAndDispatch(bool networkForwarding = true) {
    MessageDTO message;
    if (m_deserializer.deserializeFromStream(message)) {
        // Handle greet
        if (const auto* greeting = std::get_if<GreetingDTO>(&message.getMessage())) {
            m_logger.log(LogLevel::Info, "Received greeting, ID obtained is %d", greeting->getId());
            m_bsp.setHiveMindUUID(greeting->getId());
            return true;
        }
        // Handle network API call
        else if (const auto* apiCall = std::get_if<NetworkApiDTO>(&message.getMessage())) {
            return dispatchNetworkAPI(message, *apiCall, networkForwarding);
        } else if (const auto* apiCall =
                       std::get_if<HiveConnectHiveMindApiDTO>(&message.getMessage())) {
            return dispatchHiveConnectHiveMindAPI(message, *apiCall, networkForwarding);
        } else {
            return forwardMessage(message, networkForwarding);
        }
    }
    m_logger.log(LogLevel::Error, "Error while deserializing message");
    return false;
}

bool MessageDispatcher::dispatchNetworkAPI(const MessageDTO& message,
                                           const NetworkApiDTO& apiCall,
                                           bool networkForwarding) {
    auto reply = m_networkApiHandler.handleApiCall(message.getSourceId(), apiCall);
    if (std::holds_alternative<ErrorNum>(reply)) {
        // An empty optional means an error occured
        m_logger.log(LogLevel::Error, "Failed to handle NetworkApi call");
        return false;
    }

    if (auto* response = std::get_if<std::optional<NetworkApiDTO>>(&reply)) {
        if (!response->has_value()) {
            // If it returns an empty optional, no response needs to be sent
            m_logger.log(LogLevel::Debug, "No action to take after handling request");
            return true;
        }
        // Forward message generated by the handling of the api call in the case it is not an
        // empty optional
        MessageDTO msg(m_bsp.getHiveMindUUID(), message.getSourceId(), response->value());
        return forwardMessage(msg, networkForwarding);
    }
    return false;
}

bool MessageDispatcher::dispatchHiveConnectHiveMindAPI(const MessageDTO& message,
                                                       const HiveConnectHiveMindApiDTO& apiCall,
                                                       bool networkForwarding) {
    if (message.getDestinationId() != m_bsp.getHiveMindUUID()) {
        return forwardMessage(message, networkForwarding);
    } else {
        auto response = m_HiveConnectHiveMindMessageHandler.handleMessage(
            message.getSourceId(), message.getDestinationId(), apiCall);
        if (response.has_value()) {
            return forwardMessage(response.value(), networkForwarding);
        }
        return true;
    }
    m_logger.log(LogLevel::Error, "Failed dispatch HiveConnectHiveMind message");
    return false;
}

bool MessageDispatcher::forwardMessage(const MessageDTO& message, bool networkForwarding) {
    // Message specifically for HiveMind
    if (message.getDestinationId() == m_bsp.getHiveMindUUID()) {
        m_logger.log(LogLevel::Info, "Forwarded message to HiveMind");
        return m_hivemindOutputQueue.push(message);
    }

    if (message.getDestinationId() == 0 && message.getSourceId() == m_bsp.getHiveMindUUID()) {
        // Hivemind is sending message to broadcast
        m_logger.log(LogLevel::Info, "Forwarded message to broadcast from HC %d, msg from %d",
                     m_bsp.getHiveMindUUID(), message.getSourceId());
        return m_broadcastOutputQueue.push(message);
    }

    // Received message from broadcast, forwarding it to HiveMind
    if (message.getDestinationId() == 0 && message.getSourceId() != m_bsp.getHiveMindUUID()) {
        m_logger.log(LogLevel::Info, "Forwarded message to HiveMind from broadcast");
        return m_hivemindOutputQueue.push(message);
    }

    if (m_manager.getIPFromAgentID(message.getDestinationId()).has_value() && networkForwarding) {
        m_logger.log(LogLevel::Info, "Forwarded message to unicast");
        // Fixme: change to unicast queue once socket creation delay is fixed
        return m_broadcastOutputQueue.push(message);
    }
    m_logger.log(LogLevel::Error, "Failed to forward message");
    return false;
}
