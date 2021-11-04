#ifndef HIVE_CONNECT_NEIGHBORPINGER_H
#define HIVE_CONNECT_NEIGHBORPINGER_H

#include "logger/ILogger.h"
#include "ping/ping_sock.h"
#include <optional>

class NeighborPinger {
  public:
    NeighborPinger(ILogger& logger);
    ~NeighborPinger() = default;

    /**
     * Tries to ping a neighbor
     * @param neighbor Ip adress of the neighbor to ping
     * @return true if neighbor could be reached, false otherwise
     */
    bool pingNeighbor(uint32_t neighborIp);

  private:
    ILogger& m_logger;
    bool m_pingDone;
    bool m_neighborReached;
    esp_ping_handle_t m_pingHandle;
    TaskHandle_t m_currentTaskHandle;

    static void pingSuccess(esp_ping_handle_t pingHandle, void* context);
    static void pingDone(esp_ping_handle_t pingHandle, void* context);
    static void pingFailed(esp_ping_handle_t pingHandle, void* context);
};

#endif // HIVE_CONNECT_NEIGHBORPINGER_H
