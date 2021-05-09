#include "NetworkConfig.h"

#include "DefaultNetworkConfig.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include <cstring>
#include "esp_system.h"
#include "nvs.h"
#include "nvs_flash.h"

constexpr char SSID_KEY[] = "ssid";
constexpr char PASSWORD_KEY[] = "password";
constexpr char MODE_KEY[] = "mode";

static nvs_handle_t g_networkStorageHandle;

bool NetworkConfig::initNetworkConfig() {
    return nvs_open("storage", NVS_READWRITE, &g_networkStorageHandle) == ESP_OK;
}

wifi_mode_t NetworkConfig::getMode() {
    if (gs_isRouter) {
        return WIFI_MODE_AP;
    }
    return WIFI_MODE_STA;
}

wifi_config_t* NetworkConfig::getDefaultNetworkConfig() {
    static wifi_config_t s_wifiConfig;
    if (gs_isRouter) {
        // +1 to have the \0 for the string
        strlcpy((char*)(s_wifiConfig.ap.ssid), DEFAULT_SSID, strlen(DEFAULT_SSID) + 1);
        strlcpy((char*)(s_wifiConfig.ap.password), DEFAULT_PASSWORD, strlen(DEFAULT_PASSWORD) + 1);
        s_wifiConfig.ap.authmode = DEFAULT_AUTH_MODE;
        s_wifiConfig.ap.max_connection = UINT8_MAX;
    }

    // +1 to have the \0 for the string
    strlcpy((char*)(s_wifiConfig.sta.ssid), DEFAULT_SSID, strlen(DEFAULT_SSID) + 1);
    strlcpy((char*)(s_wifiConfig.sta.password), DEFAULT_PASSWORD, strlen(DEFAULT_PASSWORD) + 1);

    s_wifiConfig.sta.scan_method = WIFI_FAST_SCAN;
    s_wifiConfig.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
    s_wifiConfig.sta.threshold.rssi = INT8_MIN;
    s_wifiConfig.sta.threshold.authmode = DEFAULT_AUTH_MODE;

    return &s_wifiConfig;
}

esp_interface_t NetworkConfig::getInterface() {
    if (gs_isRouter) {
        return ESP_IF_WIFI_AP;
    }
    return ESP_IF_WIFI_STA;
}

uint16_t NetworkConfig::getCommunicationPort() { return (uint16_t)DEFAULT_UNICAST_PORT; }

uint16_t NetworkConfig::getBroadcastInputPort() { return (uint16_t)DEFAULT_BROADCAST_INPUT_PORT; }

uint16_t NetworkConfig::getBroadcastOutputPort() { return (uint16_t)DEFAULT_BROADCAST_OUTPUT_PORT; }

bool NetworkConfig::persistNetworkConfig() {
    nvs_set_str(g_networkStorageHandle, SSID_KEY, DEFAULT_SSID);
    nvs_set_str(g_networkStorageHandle, PASSWORD_KEY, DEFAULT_PASSWORD);
    nvs_set_u8(g_networkStorageHandle, MODE_KEY, gs_isRouter);
    return true;
}
