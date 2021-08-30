#include "Storage.h"
#include "DefaultNetworkConfig.h"

constexpr char SSID_KEY[] = "ssid";
constexpr char PASSWORD_KEY[] = "password";
constexpr char IS_ROUTER_KEY[] = "router";
constexpr char MESH_ENABLED_KEY[] = "mesh_on";

Storage::Storage(ILogger& logger) : m_logger(logger) {
    while (nvs_open("storage", NVS_READWRITE, &m_networkStorageHandle) != ESP_OK) {
        m_logger.log(LogLevel::Error, "Storage cannot open partitions for settings");
    }
}

bool Storage::storeSSID(const char* ssid) {
    return nvs_set_str(m_networkStorageHandle, SSID_KEY, ssid) == ESP_OK;
}

bool Storage::storePassword(const char* password) {
    return nvs_set_str(m_networkStorageHandle, PASSWORD_KEY, password) == ESP_OK;
}

bool Storage::storeIsRouter(bool isRouter) {
    return nvs_set_u8(m_networkStorageHandle, IS_ROUTER_KEY, (uint8_t)isRouter) == ESP_OK;
}

bool Storage::storeMeshEnabled(bool meshEnabled) {
    return nvs_set_u8(m_networkStorageHandle, MESH_ENABLED_KEY, (uint8_t)meshEnabled) == ESP_OK;
}

bool Storage::getSSID(char* ssid, size_t maxLength) {
    esp_err_t err = nvs_get_str(m_networkStorageHandle, SSID_KEY, ssid, &maxLength);
    if (err == ESP_OK) {
        return true;
    }
    if (err == ESP_ERR_NVS_NOT_ENOUGH_SPACE) {
        m_logger.log(LogLevel::Error, "String too short to get SSID");
        return false;
    }
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        m_logger.log(LogLevel::Info, "SSID not found in storage, getting default");
        strlcpy(ssid, DEFAULT_SSID, strlen(DEFAULT_SSID) + 1); // +1 for \0
        return true;
    }
    return false;
}

bool Storage::getPassword(char* password, size_t maxLength) {
    esp_err_t err = nvs_get_str(m_networkStorageHandle, PASSWORD_KEY, password, &maxLength);
    if (err == ESP_OK) {
        return true;
    }
    if (err == ESP_ERR_NVS_NOT_ENOUGH_SPACE) {
        m_logger.log(LogLevel::Error, "String too short to get password");
        return false;
    }
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        m_logger.log(LogLevel::Info, "Password not found in storage, getting default");
        strlcpy(password, DEFAULT_PASSWORD, strlen(DEFAULT_PASSWORD) + 1); // +1 for \0
        return true;
    }
    return false;
}

bool Storage::getIsRouter() {
    uint8_t val;
    esp_err_t err = nvs_get_u8(m_networkStorageHandle, IS_ROUTER_KEY, &val);
    if (err == ESP_OK) {
        return (bool)val;
    }
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        m_logger.log(LogLevel::Info, "IsRouter not found in storage, using default of %d",
                     gs_isRouter);
        return gs_isRouter;
    } else {
        m_logger.log(LogLevel::Warn, "Error occurred while trying to get router flag");
    }
    return gs_isRouter;
}

bool Storage::getMeshEnabled() {
    uint8_t val;
    esp_err_t err = nvs_get_u8(m_networkStorageHandle, MESH_ENABLED_KEY, &val);
    if (err == ESP_OK) {
        return (bool)val;
    }
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        m_logger.log(LogLevel::Info, "IsRouter not found in storage");
        return gs_useMesh;
    } else {
        m_logger.log(LogLevel::Warn, "Error occurred while trying to get router flag");
    }
    return gs_useMesh;
}

bool Storage::clearStorage() { return nvs_erase_all(m_networkStorageHandle) == ESP_OK; }
