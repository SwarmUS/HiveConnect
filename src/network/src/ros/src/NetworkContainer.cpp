#include "NetworkContainer.h"
#include "NetworkBroadcast.h"
#include "NetworkInputStream.h"
#include "NetworkManager.h"
#include "NetworkOutputStream.h"
#include "TopicDefines.h"
#include "bsp/Container.h"
#include "cpp-common/HashMapStack.h"
#include <ros/ros.h>

static constexpr uint16_t gs_MAX_AGENT_IN_MAP = 64;

INetworkManager& NetworkContainer::getNetworkManager() {
    static HashMapStack<uint16_t, uint32_t, gs_MAX_AGENT_IN_MAP> s_hashMap;
    static NetworkManager s_networkManager(LoggerContainer::getLogger(), s_hashMap,
                                           BspContainer::getBSP(), getNetworkBroadcast(),
                                           BspContainer::getUserInterface());
    return s_networkManager;
}

INetworkInputStream& NetworkContainer::getNetworkInputStream() {
    ros::NodeHandle nodeHandle("~");
    int port = nodeHandle.param("tcp_listen_port", 9001);
    static NetworkInputStream s_inputStream(LoggerContainer::getLogger(), port);

    static std::once_flag s_startOnce;
    std::call_once(s_startOnce, [&]() {
        if (!s_inputStream.start()) {
            LoggerContainer::getLogger().log(LogLevel::Error, "Failed to start tcp server");
        }
    });

    return s_inputStream;
}

INetworkOutputStream& NetworkContainer::getNetworkOutputStream() {
    static NetworkOutputStream s_outputStream(LoggerContainer::getLogger());

    return s_outputStream;
}

INetworkBroadcast& NetworkContainer::getNetworkBroadcast() {
    // Could be supplied by launch file when integrated in simulation
    std::string outputPrefix(BROADCAST_OUTPUT_TOPIC);
    std::string inputPrefix(BROADCAST_INPUT_TOPIC);
    static NetworkBroadcast s_broadcast(LoggerContainer::getLogger(), BspContainer::getBSP(),
                                        outputPrefix.c_str(), inputPrefix.c_str(),
                                        BspContainer::getUserInterface());
    return s_broadcast;
}