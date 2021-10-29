#ifndef HIVE_CONNECT_ABSTRACTNETWORKMANAGER_H
#define HIVE_CONNECT_ABSTRACTNETWORKMANAGER_H

#include "INetworkManager.h"
#include "cpp-common/IHashMap.h"
#include "logger/ILogger.h"
#include <optional>
#include <string>

class AbstractNetworkManager : public INetworkManager {
  public:
    AbstractNetworkManager(ILogger& logger, IHashMap<uint16_t, uint32_t>& hashMap) :
        m_logger(logger), m_hashMap(hashMap) {}
    virtual ~AbstractNetworkManager() = default;

    virtual std::optional<uint32_t> getIPFromAgentID(uint16_t agentID) const final override;
    virtual bool registerAgent(uint16_t agentID, uint32_t ip) final override;
    virtual bool unregisterAgent(uint16_t agentID) final override;
    uint16_t getAgentList(uint16_t* agentList, uint8_t maxLength) const final override;

  protected:
    ILogger& m_logger;
    IHashMap<uint16_t, uint32_t>& m_hashMap;
};

#endif // HIVE_CONNECT_ABSTRACTNETWORKMANAGER_H
