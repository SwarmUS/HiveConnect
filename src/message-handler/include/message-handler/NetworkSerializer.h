#ifndef HIVE_CONNECT_NETWORKSERIALIZER_H
#define HIVE_CONNECT_NETWORKSERIALIZER_H

#include "INetworkManager.h"
#include "INetworkOutputStream.h"
#include "logger/ILogger.h"
#include "pheromones/HiveMindHostSerializer.h"
#include "pheromones/IProtobufOutputStream.h"

class NetworkSerializer : public IHiveMindHostSerializer {
  public:
    NetworkSerializer(INetworkOutputStream& stream,
                      INetworkManager& networkManager,
                      ILogger& logger);
    virtual ~NetworkSerializer() = default;

    bool serializeToStream(const MessageDTO& message) override;

  private:
    INetworkOutputStream& m_outputStream;
    INetworkManager& m_networkManager;
    HiveMindHostSerializer m_hivemindHostSerializer;
    ILogger& m_logger;
};

#endif // HIVE_CONNECT_NETWORKSERIALIZER_H
