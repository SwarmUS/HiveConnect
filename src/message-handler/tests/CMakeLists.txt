set(TEST_NAME test-message-handler)

set(TEST_SOURCES
        MessageSenderUnitTests.cpp
        NetworkApiHandlerUnitTests.cpp
        MessageDispatcherUnitTests.cpp
        NetworkSerializerAndDeserializerIntegrationTests.cpp
        HiveConnectHiveMindHandlerTests.cpp
        )

set(TEST_HEADERS
        mocks/BSPMock.h
        mocks/NotificationQueueInterfaceMock.h
        mocks/HiveMindHostDeserializerInterfaceMock.h
        mocks/HiveMindHostSerializerInterfaceMock.h
        mocks/LoggerInterfaceMock.h
        mocks/NetworkAPIHandlerMock.h
        mocks/NetworkManagerMock.h
        mocks/HiveConnectHiveMindHandlerMock.h
        mocks/StorageMock.h

        DTOMatchers.h
        )

add_executable(${TEST_NAME} ${TEST_SOURCES} ${TEST_HEADERS})

target_link_libraries(${TEST_NAME}
        PUBLIC
        gtest
        gtest_main
        gmock

        SwarmUS::HiveConnect::MessageHandler
        SwarmUS::HiveConnect::Network
        )

add_test(NAME ${TEST_NAME}
        COMMAND ${TEST_NAME})
