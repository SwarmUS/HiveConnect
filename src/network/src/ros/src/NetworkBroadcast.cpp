#include "NetworkBroadcast.h"
#include <unistd.h>

void NetworkBroadcast::handleReception(const hive_connect::Broadcast& msg) {
    if (msg.data.empty()) {
        m_logger.log(LogLevel::Warn, "Received empty data in broadcast");
        return;
    }
    CircularBuff_put(&m_circularBuffer, msg.data.data(), msg.data.size());
    m_receivedBytes = true;
    m_conditionVariable.notify_one();
}

NetworkBroadcast::NetworkBroadcast(ILogger& logger,
                                   IBSP& bsp,
                                   const char* publishingTopicPrefix,
                                   const char* subscribingTopicPrefix,
                                   IUserInterface& ui) :
    m_logger(logger),
    m_bsp(bsp),
    m_pubTopicPrefix(publishingTopicPrefix),
    m_subTopicPrefix(subscribingTopicPrefix),
    m_isStarted(false),
    m_ui(ui) {

    CircularBuff_init(&m_circularBuffer, m_data.data(), m_data.size());
}

NetworkBroadcast::~NetworkBroadcast() { stop(); }

bool NetworkBroadcast::start() {
    if (m_bsp.getHiveMindUUID() == 0) {
        m_ui.setNetworkRGB(RGBColor::RED);
        m_logger.log(LogLevel::Error, "Trying to start broadcaster without valid uuid");
        return false;
    }
    ros::NodeHandle handle;
    m_publisher = handle.advertise<hive_connect::Broadcast>(
        m_pubTopicPrefix + std::to_string(m_bsp.getHiveMindUUID()), 1000);
    m_subscriber = handle.subscribe(m_subTopicPrefix + std::to_string(m_bsp.getHiveMindUUID()),
                                    1000, &NetworkBroadcast::handleReception, this);
    m_logger.log(LogLevel::Info, "Broadcast interface started for agent %d",
                 m_bsp.getHiveMindUUID());

    m_isStarted = true;
    return true;
}

bool NetworkBroadcast::stop() { return true; }

bool NetworkBroadcast::send(const uint8_t* data, uint16_t length) {
    hive_connect::Broadcast msg;
    msg.source_robot = m_bsp.getHiveMindUUID();
    msg.data.insert(msg.data.end(), data, &data[length]);
    m_publisher.publish(msg);
    return true;
}

bool NetworkBroadcast::receive(uint8_t* data, uint16_t length) {
    std::unique_lock lock(m_mutex);
    while (CircularBuff_getLength(&m_circularBuffer) < length) {
        m_conditionVariable.wait(lock, [&] { return m_receivedBytes; });
    }

    return CircularBuff_get(&m_circularBuffer, data, length) == length;
}

bool NetworkBroadcast::isStarted() const { return m_isStarted; }