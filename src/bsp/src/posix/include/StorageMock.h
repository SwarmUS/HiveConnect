#ifndef HIVE_CONNECT_STORAGEMOCK_H
#define HIVE_CONNECT_STORAGEMOCK_H

#include "bsp/IStorage.h"

class StorageMock : public IStorage {
  public:
    StorageMock() = default;

    bool storeSSID(const char* ssid) override {
        (void) ssid;
        return false;
    }
    bool storePassword(const char* password) override {
        (void) password;
        return false;
    }

    bool storeIsRouter(bool isRouter) override {
        (void) isRouter;
        return false;
    }
    bool storeMeshEnabled(bool meshEnabled) override {
        (void) meshEnabled;
        return false;
    }

    bool getSSID(char* ssid, size_t maxLength) override {
        (void) ssid;
        (void) maxLength;
        return false;
    }
    bool getPassword(char* password, size_t maxLength) override {
        (void) password;
        (void) maxLength;
        return false;
     }
    bool getIsRouter() override {
        return false;
    }
    bool getMeshEnabled() override {
        return false;
    }

    bool clearStorage() override {
        return false;
    }

};

#endif // HIVE_CONNECT_STORAGEMOCK_H
