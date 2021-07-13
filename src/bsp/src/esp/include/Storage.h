#ifndef HIVE_CONNECT_STORAGE_H
#define HIVE_CONNECT_STORAGE_H

#include "bsp/IStorage.h"
#include "logger/ILogger.h"
#include "nvs.h"

class Storage : public IStorage {
  public:
    Storage(ILogger& logger);

    bool storeSSID(const char* ssid) override;
    bool storePassword(const char* password) override;
    bool storeIsRouter(bool isRouter) override;
    bool storeMeshEnabled(bool meshEnabled) override;

    bool getSSID(char* ssid, size_t maxLength) override;
    bool getPassword(char* password, size_t maxLength) override;
    bool getIsRouter() override;
    bool getMeshEnabled() override;

  private:
    ILogger& m_logger;
    nvs_handle_t m_networkStorageHandle;
};

#endif // HIVE_CONNECT_STORAGE_H
