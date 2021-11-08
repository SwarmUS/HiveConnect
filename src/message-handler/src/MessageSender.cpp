#include "MessageSender.h"

MessageSender::MessageSender(INotificationQueue<MessageDTO>& inputQueue,
                             IHiveMindHostSerializer& serializer,
                             IBSP& bsp,
                             ILogger& logger) :
    m_inputQueue(inputQueue), m_serializer(serializer), m_bsp(bsp), m_logger(logger) {}

bool MessageSender::greet() {
    uint16_t uuid = m_bsp.getHiveMindUUID();
    GreetingDTO greet(uuid);
    MessageDTO msg(uuid, uuid, greet);
    return m_serializer.serializeToStream(msg);
}

bool MessageSender::processAndSerialize() {
    if (m_inputQueue.isEmpty()) {
        m_inputQueue.wait(1000);
    }
    const std::optional<std::reference_wrapper<const MessageDTO>> message = m_inputQueue.peek();
    if (message) {
        bool ret = m_serializer.serializeToStream(message.value());
        m_inputQueue.pop();
        return ret;
    }

    return true;
}