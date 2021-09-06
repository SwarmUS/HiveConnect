#ifndef HIVE_CONNECT_HIVECONNECTHIVEMINDMESSAGEHANDLER_H
#define HIVE_CONNECT_HIVECONNECTHIVEMINDMESSAGEHANDLER_H

#include "IHiveConnectHiveMindMessageHandler.h"
#include "INetworkManager.h"
#include "bsp/IStorage.h"
#include "logger/ILogger.h"

class HiveConnectHiveMindApiMessageHandler : public IHiveConnectHiveMindApiMessageHandler {
  public:
    HiveConnectHiveMindApiMessageHandler(ILogger& logger,
                                         INetworkManager& networkManager,
                                         IStorage& storage);
    ~HiveConnectHiveMindApiMessageHandler() override = default;

    std::optional<MessageDTO> handleMessage(uint16_t sourceId,
                                            uint16_t destID,
                                            const HiveConnectHiveMindApiDTO& message) override;

  private:
    ILogger& m_logger;
    INetworkManager& m_networkManager;
    IStorage& m_storage;
};
#endif // HIVE_CONNECT_HIVECONNECTHIVEMINDMESSAGEHANDLER_H
