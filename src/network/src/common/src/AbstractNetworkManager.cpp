#include "AbstractNetworkManager.h"

bool callback(const uint16_t& key, uint32_t& value, void* context) {
    (void)value;
    auto* tuple = static_cast<std::tuple<uint16_t*, uint8_t, uint8_t>*>(context);
    auto& [array, index, maxLength] = *tuple;
    if (index >= maxLength) {
        return false;
    }
    array[index] = key;
    index++;
    return true;
}

std::optional<uint32_t> AbstractNetworkManager::getIPFromAgentID(uint16_t agentID) const {
    auto agent = m_hashMap.at(agentID);
    if (agent) {
        return agent.value().get();
    }
    return {};
}

bool AbstractNetworkManager::registerAgent(uint16_t agentID, uint32_t ip) {
    if (m_hashMap.upsert(agentID, ip)) {
        unsigned char bytes[4];
        bytes[0] = ip & 0xFF;
        bytes[1] = (ip >> 8) & 0xFF;
        bytes[2] = (ip >> 16) & 0xFF;
        bytes[3] = (ip >> 24) & 0xFF;
        m_logger.log(LogLevel::Info, "Registered agent %d with value %d.%d.%d.%d", agentID,
                     bytes[0], bytes[1], bytes[2], bytes[3]);
        return true;
    }
    return false;
}

bool AbstractNetworkManager::unregisterAgent(uint16_t agentID) { return m_hashMap.remove(agentID); }

uint16_t AbstractNetworkManager::getAgentList(uint16_t* agentList, uint8_t maxLength) const {
    uint8_t index = 0;
    std::tuple<uint16_t*, uint8_t, uint8_t> tuple = std::make_tuple(agentList, index, maxLength);
    if (!m_hashMap.forEach(callback, &tuple)) {
        m_logger.log(LogLevel::Warn,
                     "Too many neighbors in hash map to place them in array of %d agents",
                     maxLength);
    }
    return std::get<1>(tuple);
}
