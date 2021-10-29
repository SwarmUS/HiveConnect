
#include "DTOMatchers.h"
#include "message-handler/MessageDispatcher.h"
#include "mocks/BSPMock.h"
#include "mocks/HiveConnectHiveMindHandlerMock.h"
#include "mocks/HiveMindHostDeserializerInterfaceMock.h"
#include "mocks/LoggerInterfaceMock.h"
#include "mocks/NetworkAPIHandlerMock.h"
#include "mocks/NetworkManagerMock.h"
#include "mocks/NotificationQueueInterfaceMock.h"

class MessageDispatcherFixture : public testing::Test {
  protected:
    MessageDispatcher* m_messageDispatcher;

    NotificationQueueInterfaceMock<MessageDTO> m_hivemindQueue;
    NotificationQueueInterfaceMock<MessageDTO> m_broadcastQueue;
    NotificationQueueInterfaceMock<MessageDTO> m_unicastQueue;
    HiveMindHostDeserializerInterfaceMock m_deserializer;
    NetworkAPIHandlerMock m_networkApiHandler;
    HiveConnectHiveMindHandlerMock m_hiveConnectHiveMindHandler;
    NetworkManagerMock m_manager;
    BSPMock* m_bsp;
    LoggerInterfaceMock m_logger;
    MessageDTO m_message;

    IPDiscoveryDTO* m_ipDiscovery;
    GreetingDTO* m_greeting;

    FunctionCallRequestDTO* m_fRequest;
    UserCallRequestDTO* m_uRequest;
    RequestDTO* m_request;

    uint16_t m_uuid = 69;
    const uint16_t m_remoteUUID = 42;
    const uint16_t m_remoteIP = 12345;

    void SetUp() override {
        m_bsp = new BSPMock();

        m_ipDiscovery = new IPDiscoveryDTO(m_remoteIP);
        m_greeting = new GreetingDTO(m_uuid);

        m_messageDispatcher = new MessageDispatcher(
            m_hivemindQueue, m_unicastQueue, m_broadcastQueue, m_deserializer, m_networkApiHandler,
            m_hiveConnectHiveMindHandler, *m_bsp, m_logger, m_manager);

        m_fRequest = new FunctionCallRequestDTO(NULL, NULL, 0);
        m_uRequest =
            new UserCallRequestDTO(UserCallTargetDTO::HOST, UserCallTargetDTO::BUZZ, *m_fRequest);
        m_request = new RequestDTO(1, *m_uRequest);
    }

    void TearDown() override {
        delete m_bsp;
        delete m_ipDiscovery;
        delete m_greeting;
        delete m_messageDispatcher;

        delete m_request;
        delete m_uRequest;
        delete m_fRequest;
    }
};

TEST_F(MessageDispatcherFixture, MessageDispatcherFixture_deserializeAndDispatch_validGreeting) {
    // Given
    m_message = MessageDTO(m_uuid, m_uuid, m_uuid);
    EXPECT_CALL(m_deserializer, deserializeFromStream(testing::_))
        .Times(1)
        .WillOnce(testing::DoAll(testing::SetArgReferee<0>(m_message), testing::Return(true)));

    EXPECT_CALL(*m_bsp, setHiveMindUUID(m_greeting->getId())).Times(1);

    // Then
    bool ret = m_messageDispatcher->deserializeAndDispatch(true);

    // Expect
    EXPECT_TRUE(ret);
}

TEST_F(MessageDispatcherFixture,
       MessageDispatcherFixture_deserializeAndDispatch_failDeserialization) {
    // Given
    m_message = MessageDTO(m_uuid, m_uuid, m_uuid);
    EXPECT_CALL(m_deserializer, deserializeFromStream(testing::_))
        .Times(1)
        .WillOnce(testing::DoAll(testing::SetArgReferee<0>(m_message), testing::Return(false)));

    EXPECT_CALL(*m_bsp, setHiveMindUUID(testing::_)).Times(0);

    // Then
    bool ret = m_messageDispatcher->deserializeAndDispatch(true);

    // Expect
    EXPECT_FALSE(ret);
}

TEST_F(MessageDispatcherFixture, MessageDispatcherFixture_deserializeAndDispatch_validIpDisovery) {
    // Given
    const NetworkApiDTO apiCall(*m_ipDiscovery);
    m_message = MessageDTO(m_uuid, m_uuid, apiCall);
    EXPECT_CALL(m_deserializer, deserializeFromStream(testing::_))
        .Times(1)
        .WillOnce(testing::DoAll(testing::SetArgReferee<0>(m_message), testing::Return(true)));

    EXPECT_CALL(m_networkApiHandler, handleApiCall(testing::_, testing::_))
        .With(testing::AllOf(NetworkApiDTOMatcher(apiCall)))
        .WillOnce(testing::Return(
            std::optional<NetworkApiDTO>({}))); // Behavior could change in the future.

    // Then
    bool ret = m_messageDispatcher->deserializeAndDispatch(false);

    // Expect
    EXPECT_TRUE(ret);
}

TEST_F(MessageDispatcherFixture,
       MessageDispatcherFixture_deserializeAndDispatch_ForwardToUnicast_knownIP) {
    // Given
    UserCallRequestDTO uRequest(UserCallTargetDTO::BUZZ, UserCallTargetDTO::HOST, *m_fRequest);
    RequestDTO request(1, uRequest);
    m_message = MessageDTO(m_uuid, m_remoteUUID, request);

    EXPECT_CALL(m_deserializer, deserializeFromStream(testing::_))
        .Times(1)
        .WillOnce(testing::DoAll(testing::SetArgReferee<0>(m_message), testing::Return(true)));
    EXPECT_CALL(*m_bsp, getHiveMindUUID);
    // Fixme: replace this with unicast queue once socket creation delay is fixed
    EXPECT_CALL(m_broadcastQueue, push(testing::_)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(m_manager, getIPFromAgentID(m_remoteUUID))
        .WillOnce(testing::Return(std::optional<uint32_t>(m_remoteUUID)));

    // Then
    bool ret = m_messageDispatcher->deserializeAndDispatch(true);

    // Expect
    EXPECT_TRUE(ret);
}

TEST_F(MessageDispatcherFixture,
       MessageDispatcherFixture_deserializeAndDispatch_ForwardToUnicast_unknownIP) {
    // Given
    UserCallRequestDTO uRequest(UserCallTargetDTO::BUZZ, UserCallTargetDTO::HOST, *m_fRequest);
    RequestDTO request(1, uRequest);
    m_message = MessageDTO(m_uuid, m_remoteUUID, request);

    EXPECT_CALL(m_deserializer, deserializeFromStream(testing::_))
        .Times(1)
        .WillOnce(testing::DoAll(testing::SetArgReferee<0>(m_message), testing::Return(true)));
    EXPECT_CALL(*m_bsp, getHiveMindUUID);
    EXPECT_CALL(m_manager, getIPFromAgentID(m_remoteUUID)).WillOnce(testing::Return(std::nullopt));

    // Then
    bool ret = m_messageDispatcher->deserializeAndDispatch(true);

    // Expect
    EXPECT_FALSE(ret);
}

TEST_F(MessageDispatcherFixture,
       MessageDispatcherFixture_deserializeAndDispatch_HandlingInboundBroadcast) {
    // Given
    UserCallRequestDTO uRequest(UserCallTargetDTO::BUZZ, UserCallTargetDTO::HOST, *m_fRequest);
    RequestDTO request(1, uRequest);
    m_message = MessageDTO(m_uuid, 0, request);

    EXPECT_CALL(m_deserializer, deserializeFromStream(testing::_))
        .Times(1)
        .WillOnce(testing::DoAll(testing::SetArgReferee<0>(m_message), testing::Return(true)));
    EXPECT_CALL(*m_bsp, getHiveMindUUID);
    EXPECT_CALL(m_hivemindQueue, push(testing::_)).WillOnce(testing::Return(true));

    // Then
    bool ret = m_messageDispatcher->deserializeAndDispatch(false);

    // Expect
    EXPECT_TRUE(ret);
}

TEST_F(MessageDispatcherFixture,
       MessageDispatcherFixture_deserializeAndDispatch_HiveConnectHiveMindHandler_local_request) {
    // Given
    GetAgentsListRequestDTO agentRequest;
    HiveConnectHiveMindApiDTO call(1, agentRequest);
    m_message = MessageDTO(m_uuid, m_uuid, call);

    EXPECT_CALL(m_deserializer, deserializeFromStream(testing::_))
        .Times(1)
        .WillOnce(testing::DoAll(testing::SetArgReferee<0>(m_message), testing::Return(true)));
    EXPECT_CALL(*m_bsp, getHiveMindUUID).WillRepeatedly(testing::Return(m_uuid));

    uint16_t agents[] = {1, 2};
    GetAgentsListResponseDTO agentsListResponse(agents, 2);
    HiveConnectHiveMindApiDTO response(1, agentsListResponse);
    MessageDTO msg(m_uuid, m_uuid, response);
    EXPECT_CALL(m_hiveConnectHiveMindHandler, handleMessage(testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(msg));

    EXPECT_CALL(m_hivemindQueue, push(testing::_)).WillOnce(testing::Return(true));

    // Then
    bool ret = m_messageDispatcher->deserializeAndDispatch(true);

    // Expect
    EXPECT_TRUE(ret);
}

TEST_F(MessageDispatcherFixture,
       MessageDispatcherFixture_deserializeAndDispatch_HiveConnectHiveMindHandler_remote_request) {
    // Given
    GetAgentsListRequestDTO agentRequest;
    HiveConnectHiveMindApiDTO call(1, agentRequest);
    m_message = MessageDTO(m_remoteUUID, m_uuid, call);

    EXPECT_CALL(m_deserializer, deserializeFromStream(testing::_))
        .Times(1)
        .WillOnce(testing::DoAll(testing::SetArgReferee<0>(m_message), testing::Return(true)));
    EXPECT_CALL(*m_bsp, getHiveMindUUID).WillRepeatedly(testing::Return(m_uuid));

    uint16_t agents[] = {1, 2};
    GetAgentsListResponseDTO agentsListResponse(agents, 2);
    HiveConnectHiveMindApiDTO response(1, agentsListResponse);
    MessageDTO msg(m_uuid, m_remoteUUID, response);
    EXPECT_CALL(m_hiveConnectHiveMindHandler, handleMessage(testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(msg));
    EXPECT_CALL(m_manager, getIPFromAgentID(m_remoteUUID))
        .WillOnce(testing::Return(std::optional<uint32_t>(m_remoteUUID)));
    // Fixme: replace this with unicast queue once socket creation delay is fixed
    EXPECT_CALL(m_broadcastQueue, push(testing::_)).WillOnce(testing::Return(true));

    // Then
    bool ret = m_messageDispatcher->deserializeAndDispatch(false);

    // Expect
    EXPECT_TRUE(ret);
}

TEST_F(MessageDispatcherFixture,
       MessageDispatcherFixture_deserializeAndDispatch_HiveConnectHiveMindHandler_remote_response) {
    // Given
    uint16_t agents[] = {1, 2};
    GetAgentsListResponseDTO agentsListResponse(agents, 2);
    HiveConnectHiveMindApiDTO call(1, agentsListResponse);
    m_message = MessageDTO(m_remoteUUID, m_uuid, call);

    EXPECT_CALL(m_deserializer, deserializeFromStream(testing::_))
        .Times(1)
        .WillOnce(testing::DoAll(testing::SetArgReferee<0>(m_message), testing::Return(true)));
    EXPECT_CALL(*m_bsp, getHiveMindUUID).WillRepeatedly(testing::Return(m_uuid));

    EXPECT_CALL(m_hiveConnectHiveMindHandler, handleMessage(testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(m_message));

    EXPECT_CALL(m_hivemindQueue, push(testing::_)).WillOnce(testing::Return(true));

    // Then
    bool ret = m_messageDispatcher->deserializeAndDispatch(false);

    // Expect
    EXPECT_TRUE(ret);
}