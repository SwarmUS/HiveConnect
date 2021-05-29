#ifndef HIVE_CONNECT_MESSAGEDISPATCHER_H
#define HIVE_CONNECT_MESSAGEDISPATCHER_H

#include "IHiveConnectHiveMindMessageHandler.h"
#include "IMessageDispatcher.h"
#include "INetworkAPIHandler.h"
#include "INetworkManager.h"
#include "bsp/IBSP.h"
#include "cpp-common/ICircularQueue.h"
#include "logger/ILogger.h"
#include "pheromones/IHiveMindHostDeserializer.h"
#include "pheromones/MessageDTO.h"

class MessageDispatcher : public IMessageDispatcher {
  public:
    MessageDispatcher(ICircularQueue<MessageDTO>& hivemindOutputQ,
                      ICircularQueue<MessageDTO>& unicastOutputQ,
                      ICircularQueue<MessageDTO>& broadcastOutputQ,
                      IHiveMindHostDeserializer& deserializer,
                      INetworkAPIHandler& networkApiHandler,
                      IHiveConnectHiveMindApiMessageHandler& hiveConnectHiveMindApiMessageHandler,
                      IBSP& bsp,
                      ILogger& logger,
                      INetworkManager& manager);
    ~MessageDispatcher() override = default;

    bool deserializeAndDispatch() override;

  private:
    ICircularQueue<MessageDTO>& m_hivemindOutputQueue;
    ICircularQueue<MessageDTO>& m_unicastOutputQueue;
    ICircularQueue<MessageDTO>& m_broadcastOutputQueue;

    IHiveMindHostDeserializer& m_deserializer;
    INetworkAPIHandler& m_networkApiHandler;
    IHiveConnectHiveMindApiMessageHandler& m_HiveConnectHiveMindMessageHandler;

    IBSP& m_bsp;
    ILogger& m_logger;
    INetworkManager& m_manager;

    bool dispatchNetworkAPI(const MessageDTO& message, const NetworkApiDTO& apiCall);
    bool dispatchHiveConnectHiveMindAPI(const MessageDTO& message,
                                        const HiveConnectHiveMindApiDTO& apiCall);
    bool forwardMessage(const MessageDTO& message);
};

#endif // HIVE_CONNECT_MESSAGEDISPATCHER_H
