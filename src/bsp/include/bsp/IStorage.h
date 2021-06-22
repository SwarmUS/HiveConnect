#ifndef HIVE_CONNECT_ISTORAGE_H
#define HIVE_CONNECT_ISTORAGE_H

#include <cstdint>
#include <cstring>
class IStorage {
  public:
    virtual ~IStorage() = default;

    virtual bool storeSSID(char* ssid) = 0;
    virtual bool storePassword(char* password) = 0;
    virtual bool storeIsRouter(bool isRouter) = 0;
    virtual bool storeMeshEnabled(bool meshEnabled) = 0;

    virtual bool getSSID(char* ssid, size_t maxLength) = 0;
    virtual bool getPassword(char* password, size_t maxLength) = 0;
    virtual bool getIsRouter() = 0;
    virtual bool getMeshEnabled() = 0;
};

#endif // HIVE_CONNECT_ISTORAGE_H
