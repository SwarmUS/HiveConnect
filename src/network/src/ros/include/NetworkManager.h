#ifndef HIVE_CONNECT_NETWORKMANAGER_H
#define HIVE_CONNECT_NETWORKMANAGER_H

#include "../../common/include/AbstractNetworkManager.h"
#include "INetworkBroadcast.h"
#include "bsp/IBSP.h"
#include "bsp/IUserInterface.h"
#include "cpp-common/IHashMap.h"
#include "logger/ILogger.h"

class NetworkManager : public AbstractNetworkManager {
  public:
    NetworkManager(ILogger& logger,
                   IHashMap<uint16_t, uint32_t>& hashMap,
                   IBSP& bsp,
                   INetworkBroadcast& networkBroadcast,
                   IUserInterface& ui);
    ~NetworkManager() override = default;

    void start() override{};
    void restart() override{};

    /**
     * @brief Gets the status of the network
     */
    NetworkStatus getNetworkStatus() const override;

    /**
     * @brief Returns ID the tcp port used for unicast communication
     * @return The networking ID for the module
     */
    uint32_t getSelfIP() const override;

  private:
    IBSP& m_bsp;
    INetworkBroadcast& m_networkBroadcast;
    IUserInterface& m_ui;
};

#endif // HIVE_CONNECT_NETWORKMANAGER_H
