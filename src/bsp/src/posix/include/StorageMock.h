#ifndef HIVE_CONNECT_STORAGEMOCK_H
#define HIVE_CONNECT_STORAGEMOCK_H

#include "bsp/IStorage.h"

class StorageMock : public IStorage {
  public:
    StorageMock() = default;

    bool storeSSID(const char* ssid) override {
        (void)ssid;
        return true;
    }
    bool storePassword(const char* password) override {
        (void)password;
        return true;
    }

    bool storeIsRouter(bool isRouter) override {
        (void)isRouter;
        return true;
    }
    bool storeMeshEnabled(bool meshEnabled) override {
        (void)meshEnabled;
        return true;
    }

    bool getSSID(char* ssid, size_t maxLength) override {
        (void)ssid;
        (void)maxLength;
        return true;
    }
    bool getPassword(char* password, size_t maxLength) override {
        (void)password;
        (void)maxLength;
        return true;
    }
    bool getIsRouter() override { return true; }
    bool getMeshEnabled() override { return true; }

    bool clearStorage() override { return true; }
};

#endif // HIVE_CONNECT_STORAGEMOCK_H
