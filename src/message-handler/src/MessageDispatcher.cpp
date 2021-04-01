#include "MessageDispatcher.h"
#include "Task.h"

MessageDispatcher::MessageDispatcher(ICircularQueue<MessageDTO>& hivemindOutputQ,
                                     ICircularQueue<MessageDTO>& unicastOutputQ,
                                     ICircularQueue<MessageDTO>& broadcastOutputQ,
                                     IHiveMindHostDeserializer& deserializer,
                                     INetworkAPIHandler& handler,
                                     IBSP& bsp,
                                     ILogger& logger) :
    m_hivemindOutputQueue(hivemindOutputQ),
    m_unicastOutputQueue(unicastOutputQ),
    m_broadcastOutputQueue(broadcastOutputQ),
    m_deserializer(deserializer),
    m_requestHandler(handler),
    m_bsp(bsp),
    m_logger(logger) {}

bool MessageDispatcher::deserializeAndDispatch() {
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
            return dispatchNetworkAPI(message, *apiCall);
        } else {
            return forwardMessage(message);
        }
    }
    m_logger.log(LogLevel::Error, "Error while deserializing message");
    return false;
}

bool MessageDispatcher::dispatchNetworkAPI(const MessageDTO& message,
                                           const NetworkApiDTO& apiCall) {
    auto reply = m_requestHandler.handleApiCall(apiCall);
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
        return forwardMessage(msg);
    }
    return false;
}

bool MessageDispatcher::forwardMessage(const MessageDTO& message) {
    if (message.getDestinationId() == m_bsp.getHiveMindUUID()) {
        return m_hivemindOutputQueue.push(message);
    }

    if (message.getDestinationId() == 0) {
        return m_broadcastOutputQueue.push(message);
    }
    // Maybe add check that ip table contains the robot ID in the future.
    return m_unicastOutputQueue.push(message);
}