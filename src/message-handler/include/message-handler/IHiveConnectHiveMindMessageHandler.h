#ifndef HIVE_CONNECT_IHIVECONNECTHIVEMINDMESSAGEHANDLER_H
#define HIVE_CONNECT_IHIVECONNECTHIVEMINDMESSAGEHANDLER_H

#include "pheromones/MessageDTO.h"
#include <optional>
#include <pheromones/HiveConnectHiveMindApiDTO.h>

/**
 *@brief Handles HiveConnectHiveMindApi messages and makes the proper response
 **/
class IHiveConnectHiveMindApiMessageHandler {
  public:
    virtual ~IHiveConnectHiveMindApiMessageHandler() = default;

    /**
     *@brief handles HiveConnectHiveMindApi
     *@param sourceId the source id of the message
     *@param destId the source id of the message
     *@param message the message dto to handle
     *@return true if the operation was successfull, false if not*/
    virtual std::optional<MessageDTO> handleMessage(uint16_t sourceId,
                                                    uint16_t destId,
                                                    const HiveConnectHiveMindApiDTO& message) = 0;
};

#endif // HIVE_CONNECT_IHIVECONNECTHIVEMINDMESSAGEHANDLER_H
