#ifndef HIVE_CONNECT_NETWORKMANAGERMOCK_H
#define HIVE_CONNECT_NETWORKMANAGERMOCK_H

#include "AbstractNetworkManager.h"
#include <gmock/gmock.h>

class NetworkManagerMock : public INetworkManager {

  public:
    NetworkManagerMock() = default;
    MOCK_METHOD(void, start, (), (override));
    MOCK_METHOD(void, restart, (), (override));
    MOCK_METHOD(NetworkStatus, getNetworkStatus, (), (const, override));
    MOCK_METHOD(uint32_t, getSelfIP, (), (const, override));
    MOCK_METHOD(bool, registerAgent, (uint16_t agentID, uint32_t ip), (override));
    MOCK_METHOD(bool, unregisterAgent, (uint16_t agentID), (override));
    MOCK_METHOD(std::optional<uint32_t>, getIPFromAgentID, (uint16_t agentID), (const, override));
    MOCK_METHOD(uint16_t,
                getAgentList,
                (uint16_t * agentList, uint8_t maxLength),
                (const, override));
    MOCK_METHOD(const uint32_t, getMaxAgentListLength, (), (const, override));
};

#endif // HIVE_CONNECT_NETWORKMANAGERMOCK_H
