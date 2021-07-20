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

static wifi_config_t g_wifiConfig;

bool NetworkConfig::initNetworkConfig() { return true; }

wifi_mode_t NetworkConfig::getMode() {
    if (BspContainer::getStorage().getIsRouter()) {
        return WIFI_MODE_AP;
    }
    return WIFI_MODE_STA;
}

wifi_config_t* NetworkConfig::getDefaultNetworkConfig() {

    IStorage& storage = BspContainer::getStorage();

    if (storage.getIsRouter()) {
        // +1 to have the \0 for the string
        storage.getSSID((char*)g_wifiConfig.ap.ssid, sizeof(g_wifiConfig.ap.ssid));
        storage.getPassword((char*)g_wifiConfig.ap.password, sizeof(g_wifiConfig.ap.password));
        g_wifiConfig.ap.authmode = DEFAULT_AUTH_MODE;
        g_wifiConfig.ap.max_connection = UINT8_MAX;
    }
    else {
        g_wifiConfig.ap.ssid[0] = '\0'; // Used to reset value in case of change
    }

    // +1 to have the \0 for the string
    storage.getSSID((char*)g_wifiConfig.sta.ssid, sizeof(g_wifiConfig.ap.ssid));
    storage.getPassword((char*)g_wifiConfig.sta.password, sizeof(g_wifiConfig.ap.password));

    g_wifiConfig.sta.scan_method = WIFI_FAST_SCAN;
    g_wifiConfig.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
    g_wifiConfig.sta.threshold.rssi = INT8_MIN;
    g_wifiConfig.sta.threshold.authmode = DEFAULT_AUTH_MODE;

    return &g_wifiConfig;
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

bool NetworkConfig::persistNetworkConfig() {
    bool ret = BspContainer::getStorage().storeSSID((const char*)(g_wifiConfig.sta.ssid));
    ret &= BspContainer::getStorage().storePassword((const char*)(g_wifiConfig.sta.password));
    ret &= BspContainer::getStorage().storeIsRouter(strlen((const char *)g_wifiConfig.ap.ssid) > 1);
    // TODO: flag mesh usage and store it
    return ret;
}
