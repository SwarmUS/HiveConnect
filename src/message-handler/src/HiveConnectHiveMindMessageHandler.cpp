#include "HiveConnectHiveMindMessageHandler.h"

HiveConnectHiveMindApiMessageHandler::HiveConnectHiveMindApiMessageHandler(
    ILogger& logger, INetworkManager& networkManager) :
    m_logger(logger), m_networkManager(networkManager) {}

std::optional<MessageDTO> HiveConnectHiveMindApiMessageHandler::handleMessage(
    uint16_t sourceId, uint16_t destId, const HiveConnectHiveMindApiDTO& message) {
    // Handle the inbound request
    if (std::holds_alternative<GetAgentsListRequestDTO>(message.getMessage())) {
        uint16_t agentList[NEIGHBORS_LIST_SIZE];
        uint8_t agentListLength = m_networkManager.getAgentList(agentList, NEIGHBORS_LIST_SIZE);
        GetAgentsListResponseDTO agentsListResponse(agentList, agentListLength);
        HiveConnectHiveMindApiDTO response(sourceId, agentsListResponse);
        // Handled request, swap source and destination
        MessageDTO reply(destId, sourceId, response);
        return reply;
    }
    // If it is a response, return it to be forwarded to its destination
    else if (std::holds_alternative<GetAgentsListResponseDTO>(message.getMessage())) {
        MessageDTO reply(sourceId, destId, message);
    }
    m_logger.log(LogLevel::Error, "Could not handle HiveMindHiveConnectDTO");
    return {};
}