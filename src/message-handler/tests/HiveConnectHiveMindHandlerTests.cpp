#include "message-handler/HiveConnectHiveMindMessageHandler.h"
#include "mocks/BSPMock.h"
#include "mocks/LoggerInterfaceMock.h"
#include "mocks/NetworkManagerMock.h"
#include "mocks/StorageMock.h"
#include "gtest/gtest.h"

class HiveConnectHiveMindHandlerTests : public testing::Test {
  protected:
    LoggerInterfaceMock* m_logger;
    HiveConnectHiveMindApiMessageHandler* m_handler;
    NetworkManagerMock* m_networkManager;
    StorageMock* m_storage;

    uint16_t agentList[3] = {2, 3, 4};
    uint8_t agentListLength = 3;

    void SetUp() override {
        m_logger = new LoggerInterfaceMock();
        m_networkManager = new NetworkManagerMock();
        m_storage = new StorageMock();
        m_handler =
            new HiveConnectHiveMindApiMessageHandler(*m_logger, *m_networkManager, *m_storage);
    }

    void TearDown() override {
        delete m_logger;
        delete m_handler;
        delete m_networkManager;
        delete m_storage;
    }
};

TEST_F(HiveConnectHiveMindHandlerTests, HiveConnectHiveMindHandler_handleLocalRequest) {
    // Given: a inbound request from hivemind
    GetAgentsListRequestDTO agentListRequest;
    HiveConnectHiveMindApiDTO req(1, agentListRequest);
    MessageDTO message(1, 1, req);

    // Then
    EXPECT_CALL(*m_networkManager, getAgentList(testing::_, testing::_))
        .Times(1)
        .WillOnce(
            testing::DoAll(testing::SetArrayArgument<0>(agentList, agentList + agentListLength),
                           testing::Return(agentListLength)));

    // When
    auto ret = m_handler->handleMessage(message.getSourceId(), message.getDestinationId(), req);

    ASSERT_TRUE(ret.has_value());
    ASSERT_EQ(ret.value().getDestinationId(), 1);
    ASSERT_EQ(ret.value().getSourceId(), 1);
    ASSERT_TRUE(std::holds_alternative<HiveConnectHiveMindApiDTO>(ret.value().getMessage()));

    const auto* dto = std::get_if<HiveConnectHiveMindApiDTO>(&ret.value().getMessage());
    ASSERT_TRUE(std::holds_alternative<GetAgentsListResponseDTO>(dto->getMessage()));

    const auto* response = std::get_if<GetAgentsListResponseDTO>(&dto->getMessage());
    ASSERT_EQ(response->getAgentsLength(), agentListLength);
    for (uint16_t i = 0; i < agentListLength; i++) {
        EXPECT_EQ(response->getAgents().at(i), agentList[i]);
    }
}

TEST_F(HiveConnectHiveMindHandlerTests, HiveConnectHiveMindHandler_handleRemoteRequest) {
    // Given: a inbound request from hivemind
    GetAgentsListRequestDTO agentListRequest;
    HiveConnectHiveMindApiDTO req(1, agentListRequest);
    MessageDTO message(2, 1, req);

    // Then
    EXPECT_CALL(*m_networkManager, getAgentList(testing::_, testing::_))
        .Times(1)
        .WillOnce(
            testing::DoAll(testing::SetArrayArgument<0>(agentList, agentList + agentListLength),
                           testing::Return(agentListLength)));

    // When
    auto ret = m_handler->handleMessage(message.getSourceId(), message.getDestinationId(), req);

    ASSERT_TRUE(ret.has_value());
    ASSERT_EQ(ret.value().getDestinationId(), 2);
    ASSERT_EQ(ret.value().getSourceId(), 1);
    ASSERT_TRUE(std::holds_alternative<HiveConnectHiveMindApiDTO>(ret.value().getMessage()));

    const auto* dto = std::get_if<HiveConnectHiveMindApiDTO>(&ret.value().getMessage());
    ASSERT_TRUE(std::holds_alternative<GetAgentsListResponseDTO>(dto->getMessage()));

    const auto* response = std::get_if<GetAgentsListResponseDTO>(&dto->getMessage());
    ASSERT_EQ(response->getAgentsLength(), agentListLength);
    for (uint16_t i = 0; i < agentListLength; i++) {
        EXPECT_EQ(response->getAgents().at(i), agentList[i]);
    }
}

TEST_F(HiveConnectHiveMindHandlerTests, HiveConnectHiveMindHandler_handleNetworkConfigGetRequest) {
    HiveConnectNetworkConfigGetRequestDTO getRequestDto;
    HiveConnectHiveMindApiDTO api(1, getRequestDto);
    MessageDTO msg(1, 1, api);

    auto ret = m_handler->handleMessage(msg.getSourceId(), msg.getDestinationId(), api);

    ASSERT_TRUE(ret.has_value());
    ASSERT_EQ()
}

TEST_F(HiveConnectHiveMindHandlerTests, HiveConnectHiveMindHandler_handleRemoteResponse) {
    // Given: a inbound request from hivemind
    GetAgentsListResponseDTO agentsListResponse(agentList, agentListLength);
    HiveConnectHiveMindApiDTO response(1, agentsListResponse);
    MessageDTO message(2, 1, response);

    // When
    auto ret =
        m_handler->handleMessage(message.getSourceId(), message.getDestinationId(), response);

    ASSERT_TRUE(ret.has_value());
    ASSERT_EQ(ret.value().getDestinationId(), 1);
    ASSERT_EQ(ret.value().getSourceId(), 2);
    ASSERT_TRUE(std::holds_alternative<HiveConnectHiveMindApiDTO>(ret.value().getMessage()));

    const auto* dto = std::get_if<HiveConnectHiveMindApiDTO>(&ret.value().getMessage());
    ASSERT_TRUE(std::holds_alternative<GetAgentsListResponseDTO>(dto->getMessage()));

    const auto* msg = std::get_if<GetAgentsListResponseDTO>(&dto->getMessage());
    ASSERT_EQ(msg->getAgentsLength(), agentListLength);
    for (uint16_t i = 0; i < agentListLength; i++) {
        EXPECT_EQ(msg->getAgents().at(i), agentList[i]);
    }
}

TEST
