#ifndef HIVE_CONNECT_INETWORKMANAGER_H
#define HIVE_CONNECT_INETWORKMANAGER_H

#include "cpp-common/IHashMap.h"
#include "logger/ILogger.h"
#include <cstdint>
#include <optional>

enum class NetworkStatus { NotConnected = 0, Connecting, Connected };

class INetworkManager {
  public:
    virtual ~INetworkManager() = default;

    /**
     * @brief Starts the network manager and attempts connection
     */
    virtual void start() = 0;

    /**
     * @brief Gets the status of the network
     */
    virtual NetworkStatus getNetworkStatus() const = 0;

    /**
     * @brief Returns ID of the module in the network as a char array. In the wifi network, this
     * will be a tcp port
     * @return True if operation successful, false otherwise
     */
    virtual uint32_t getSelfIP() const = 0;

    /**
     * @brief Returns the IP from a agent ID
     * @param [in] agentID The target agent ID
     * @return True the IP/port if the agent was known or an empty optional otherwise
     * otherwise.
     */
    virtual std::optional<uint32_t> getIPFromAgentID(uint16_t agentID) const = 0;

    /**
     * @brief Add a agent to the the list of known agents
     * @param [in] agentID the id to the agent to register
     * @param [in] ip The ip/port of the agent to register
     * @return true if the agent was added or already present, false if the agent could not be
     * registered
     */
    virtual bool registerAgent(uint16_t agentID, uint32_t ip) = 0;

    /**
     * @brief Get the the agents lists as
     * @param [in] agentList the array to populate with agent IDs
     * @param [in] maxLength The maximum size of the array
     * @return the number of agents added to the array
     */
    virtual uint16_t getAgentList(uint16_t* agentList, uint8_t maxLength);
};

#endif // HIVE_CONNECT_INETWORKMANAGER_H
