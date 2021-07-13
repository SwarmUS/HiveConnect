#include "NetworkConfig.h"
#include "DefaultNetworkConfig.h"
#include "bsp/Container.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"
#include <cstring>

bool NetworkConfig::initNetworkConfig() { return true; }

wifi_mode_t NetworkConfig::getMode() {
    if (BspContainer::getStorage().getIsRouter()) {
        return WIFI_MODE_AP;
    }
    return WIFI_MODE_STA;
}

wifi_config_t* NetworkConfig::getDefaultNetworkConfig() {
    static wifi_config_t s_wifiConfig;
    IStorage& storage = BspContainer::getStorage();

    if (storage.getIsRouter()) {
        // +1 to have the \0 for the string
        storage.getSSID((char*)s_wifiConfig.ap.ssid, sizeof(s_wifiConfig.ap.ssid));
        storage.getSSID((char*)s_wifiConfig.ap.password, sizeof(s_wifiConfig.ap.password));
        s_wifiConfig.ap.authmode = DEFAULT_AUTH_MODE;
        s_wifiConfig.ap.max_connection = UINT8_MAX;
    }

    // +1 to have the \0 for the string
    storage.getSSID((char*)s_wifiConfig.sta.ssid, sizeof(s_wifiConfig.ap.ssid));
    storage.getSSID((char*)s_wifiConfig.sta.password, sizeof(s_wifiConfig.ap.password));

    s_wifiConfig.sta.scan_method = WIFI_FAST_SCAN;
    s_wifiConfig.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
    s_wifiConfig.sta.threshold.rssi = INT8_MIN;
    s_wifiConfig.sta.threshold.authmode = DEFAULT_AUTH_MODE;

    return &s_wifiConfig;
}

esp_interface_t NetworkConfig::getInterface() {
    if (BspContainer::getStorage().getIsRouter()) {
        return ESP_IF_WIFI_AP;
    }
    return ESP_IF_WIFI_STA;
}

uint16_t NetworkConfig::getCommunicationPort() { return (uint16_t)DEFAULT_UNICAST_PORT; }

uint16_t NetworkConfig::getBroadcastInputPort() { return (uint16_t)DEFAULT_BROADCAST_INPUT_PORT; }

uint16_t NetworkConfig::getBroadcastOutputPort() { return (uint16_t)DEFAULT_BROADCAST_OUTPUT_PORT; }
