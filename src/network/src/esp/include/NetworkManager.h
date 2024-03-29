#ifndef HIVE_CONNECT_NETWORKMANAGER_H
#define HIVE_CONNECT_NETWORKMANAGER_H

#include "../../common/include/AbstractNetworkManager.h"
#include "NeighborPinger.h"
#include "TCPClient.h"
#include "TCPServer.h"
#include "bsp/IUserInterface.h"
#include "esp_wifi.h"
#include "logger/ILogger.h"
#include <BaseTask.h>
#include <Task.h>

/**
 * @brief The network manager class. Handles the connection to the network
 */
class NetworkManager : public AbstractNetworkManager {
  public:
    NetworkManager(ILogger& logger,
                   INetworkInputStream& server,
                   IHashMap<uint16_t, uint32_t>& hashMap,
                   IUserInterface& ui);
    ~NetworkManager() = default;

    void start() override;
    void restart() override;
    NetworkStatus getNetworkStatus() const override;
    uint32_t getSelfIP() const override;

    /**
     * @brief Execution loops, called internally
     */
    void execute();
    void pingNeighbors();

  private:
    INetworkInputStream& m_server;
    IUserInterface& m_ui;
    BaseTask<configMINIMAL_STACK_SIZE * 10> m_networkExecuteTask;
    BaseTask<configMINIMAL_STACK_SIZE * 10> m_neighborPingerTask;
    esp_ip4_addr_t m_ipAddress;
    esp_netif_obj* m_networkInterfaceHandle;
    enum class NetworkManagerState {
        INIT = 0,
        LOOKING_FOR_NETWORK,
        CONNECTED,
        RUNNING,
        DISCONNECTED
    } m_state;

    static void eventHandler(void* context,
                             esp_event_base_t eventBase,
                             int32_t eventId,
                             void* eventData);
    bool initNetworkInterface();

    NeighborPinger m_pinger;
};

#endif // HIVE_CONNECT_NETWORKMANAGER_H
