#include "NeighborPinger.h"

NeighborPinger::NeighborPinger(ILogger& logger) : m_logger(logger) {}

void NeighborPinger::pingSuccess(esp_ping_handle_t pingHandle, void* context) {
    auto* pinger = static_cast<NeighborPinger*>(context);
    pinger->m_logger.log(LogLevel::Info, "Ping successful");
    pinger->m_neighborReached = true;
    esp_ping_stop(pingHandle);
    esp_ping_delete_session(pingHandle);
    xTaskNotifyGive(pinger->m_currentTaskHandle);
}

void NeighborPinger::pingFailed(esp_ping_handle_t pingHandle, void* context) {
    auto* pinger = static_cast<NeighborPinger*>(context);
    pinger->m_logger.log(LogLevel::Info, "Ping failed");
    pinger->m_neighborReached = false;
    xTaskNotifyGive(pinger->m_currentTaskHandle);
}

void NeighborPinger::pingDone(esp_ping_handle_t pingHandle, void* context) {
    auto* pinger = static_cast<NeighborPinger*>(context);
    esp_ping_stop(pingHandle);
    esp_ping_delete_session(pingHandle);
    pinger->m_pingDone = true;
    pinger->m_logger.log(LogLevel::Info, "Ping done");
}

void test(esp_ping_handle_t pingHandle, void* context) {
    auto* status = static_cast<bool*>(context);
    *status = true;
}

bool NeighborPinger::pingNeighbor(uint32_t neighborIp) {
    m_logger.log(LogLevel::Info, "Pinging neighbor!");
    ip_addr_t targetAddr;
    targetAddr.u_addr.ip4.addr = htonl(neighborIp);
    targetAddr.type = 0; // 0 for IPv4, 6 for IPv6

    esp_ping_config_t pingConfig = ESP_PING_DEFAULT_CONFIG(); // 5 pings, 1 second timeout
    pingConfig.data_size = 8;
    pingConfig.target_addr = targetAddr;
    pingConfig.task_prio = tskIDLE_PRIORITY + 10;

    esp_ping_callbacks_t pingCallbacks;
    pingCallbacks.on_ping_success = pingSuccess;
    pingCallbacks.on_ping_timeout = pingFailed;
    pingCallbacks.on_ping_end = pingDone;
    pingCallbacks.cb_args = this;

    m_pingDone = false;
    m_neighborReached = false;
    m_currentTaskHandle = xTaskGetCurrentTaskHandle();
    esp_ping_new_session(&pingConfig, &pingCallbacks, &m_pingHandle);
    esp_ping_start(m_pingHandle);
    while (!m_pingDone) {
        ulTaskNotifyTake(pdTRUE, 500);
    }
    return m_neighborReached;
}