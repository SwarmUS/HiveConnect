#ifndef HIVE_CONNECT_NETWORKCONFIG_H
#define HIVE_CONNECT_NETWORKCONFIG_H

#include "esp_event.h"

/**
 * @brief Namespace for functions to get different configuration element for the netowrk
 */
namespace NetworkConfig {
/**
 * @brief Initialize the storage used for the network configuration
 * @return True if successful, false otherwise
 */
bool initNetworkConfig();
/**
 * @brief Get the configured mode for the node (station, access-point, etc...)
 * @return The configured mode
 */
wifi_mode_t getMode();

/**
 * @brief Get the network default configuration (ssid, password, etc...)
 * @return
 */
wifi_config_t* getDefaultNetworkConfig();

/**
 * @brief Get the interface to use for the network
 * @return The network interface
 */
esp_interface_t getInterface();

/**
 * @brief Get the communication port used for unicast sockets in the network
 * @return The port number
 */
uint16_t getCommunicationPort();

/**
 * @brief Get the communication port used for receiving broadcast communications
 * @return The port number
 */
uint16_t getBroadcastInputPort();

/**
 * @brief Get the communication port used for sending broadcast communications
 * @return The port number
 */
uint16_t getBroadcastOutputPort();

/**
 * @brief Persists the current network config
 * @return true if successful
 */
bool persistNetworkConfig();
} // namespace NetworkConfig

#endif // HIVE_CONNECT_NETWORKCONFIG_H
