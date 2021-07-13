#ifndef HIVE_CONNECT_STORAGEMOCK_H
#define HIVE_CONNECT_STORAGEMOCK_H

#include "bsp/IStorage.h"
#include <gmock/gmock.h>

class StorageMock : public IStorage {
  public:
    StorageMock() = default;

    MOCK_METHOD(bool, storeSSID, (const char* ssid), (override));
    MOCK_METHOD(bool, storePassword, (const char* password), (override));
    MOCK_METHOD(bool, storeIsRouter, (bool isRouter), (override));
    MOCK_METHOD(bool, storeMeshEnabled, (bool meshEnabled), (override));

    MOCK_METHOD(bool, getSSID, (char* ssid, size_t maxLength), (override));
    MOCK_METHOD(bool, getPassword, (char* password, size_t maxLength), (override));
    MOCK_METHOD(bool, getIsRouter, (), (override));
    MOCK_METHOD(bool, getMeshEnabled, (), (override));
};

#endif // HIVE_CONNECT_STORAGEMOCK_H
