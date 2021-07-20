#include "HiveConnectHiveMindMessageHandler.h"

HiveConnectHiveMindApiMessageHandler::HiveConnectHiveMindApiMessageHandler(
    ILogger& logger, INetworkManager& networkManager, IStorage& storage) :
    m_logger(logger), m_networkManager(networkManager), m_storage(storage) {}

std::optional<MessageDTO> HiveConnectHiveMindApiMessageHandler::handleMessage(
    uint16_t sourceId, uint16_t destId, const HiveConnectHiveMindApiDTO& message) {
    // Handle the inbound request
    if (std::holds_alternative<GetAgentsListRequestDTO>(message.getMessage())) {
        uint16_t agentList[NEIGHBORS_LIST_SIZE] = {0};
        uint8_t agentListLength = m_networkManager.getAgentList(agentList, NEIGHBORS_LIST_SIZE);
        GetAgentsListResponseDTO agentsListResponse(agentList, agentListLength);
        HiveConnectHiveMindApiDTO response(message.getMessageId(), agentsListResponse);
        // Handled request, swap source and destination
        MessageDTO reply(destId, sourceId, response);
        return reply;
    }

    // Handle the request
    if (std::holds_alternative<HiveConnectNetworkConfigGetRequestDTO>(message.getMessage())) {
        char ssid[NETWORK_SSID_MAX_LENGTH];
        char password[NETWORK_PASSWORD_MAX_LENGTH];
        m_storage.getSSID(ssid, NETWORK_SSID_MAX_LENGTH);
        m_storage.getPassword(password, NETWORK_PASSWORD_MAX_LENGTH);
        HiveConnectNetworkConfigGetResponseDTO responseDto(
            HiveConnectNetworkAccessDTO(ssid, password),
            HiveConnectRootNodeDTO(m_storage.getIsRouter()),
            HiveConnectMeshEnableDTO(m_storage.getMeshEnabled()));
        HiveConnectHiveMindApiDTO reply(message.getMessageId(), responseDto);
        return MessageDTO(destId, sourceId, reply);
    }

    if (const auto* request =
            std::get_if<HiveConnectNetworkConfigSetRequestDTO>(&message.getMessage())) {

        bool ret = true;
        if (request->getSSID().has_value()) {
            ret &= m_storage.storeSSID(request->getSSID().value());
            ret &= m_storage.storePassword(request->getPassword().value());
        }

        if (request->getRootNode().has_value() && ret) {
            ret &= m_storage.storeIsRouter(request->getRootNode().value());
        }

        if (request->getMeshEnable().has_value() && ret) {
            ret &= m_storage.storeMeshEnabled(request->getMeshEnable().value());
        }

        GenericResponseDTO responseDto(ret ? GenericResponseStatusDTO::Ok
                                           : GenericResponseStatusDTO::Error,
                                       ret ? "" : "Error setting network config");
        return MessageDTO(
            destId, sourceId,
            HiveConnectHiveMindApiDTO(message.getMessageId(),
                                      HiveConnectNetworkConfigSetResponseDTO(responseDto)));
    }

    // If it is a response, return it to be forwarded to its destination
    if (std::holds_alternative<GetAgentsListResponseDTO>(message.getMessage()) ||
        std::holds_alternative<HiveConnectNetworkConfigGetResponseDTO>(message.getMessage()) ||
        std::holds_alternative<HiveConnectNetworkConfigSetResponseDTO>(message.getMessage())) {
        MessageDTO reply(sourceId, destId, message);
        return reply;
    }

    m_logger.log(LogLevel::Error, "Could not handle HiveMindHiveConnectDTO");
    return {};
}