#ifndef HIVE_CONNECT_HIVECONNECTHIVEMINDHANDLERMOCK_H
#define HIVE_CONNECT_HIVECONNECTHIVEMINDHANDLERMOCK_H

#include "message-handler/IHiveConnectHiveMindMessageHandler.h"
#include "gmock/gmock.h"

class HiveConnectHiveMindHandlerMock : public IHiveConnectHiveMindApiMessageHandler {
  public:
    ~HiveConnectHiveMindHandlerMock() override = default;
    MOCK_METHOD(std::optional<MessageDTO>,
                handleMessage,
                (uint16_t sourceId, uint16_t destId, const HiveConnectHiveMindApiDTO& message),
                (override));
};

#endif // HIVE_CONNECT_HIVECONNECTHIVEMINDHANDLERMOCK_H
